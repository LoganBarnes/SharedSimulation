#include "shared/core/VulkanIOHandler.hpp"

#include "shared/graphics/VulkanGlfwWrapper.hpp"
#include "shared/graphics/SharedCallback.hpp"
#include "ThirdpartyDefinesConfig.hpp"
#include "SharedSimulationConfig.hpp"

#include <iostream>


namespace shs
{



/////////////////////////////////////////////
/// \brief Renderer::Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
VulkanIOHandler::VulkanIOHandler(
                                 World &world,
                                 bool  printInfo
                                 )
  : IOHandler( world, false )
  , upVulkanWrapper_( new shg::VulkanGlfwWrapper( ) )
{

  if ( printInfo )
  {

    std::cout << "Press 'ESC' to exit" << std::endl;

  }

  std::unique_ptr< shs::SharedCallback  > upCallback_( new shs::SharedCallback( ) );
  upVulkanWrapper_->setCallback( std::move( upCallback_ ) );


  upVulkanWrapper_->createNewWindow( "VulkanWindow", 1024, 720 );

  upVulkanWrapper_->createRenderPass( );

  upVulkanWrapper_->createGraphicsPipeline(
                                           shs::SHADER_PATH + "vulkan/screenSpace/vert.spv",
                                           shs::SHADER_PATH + "vulkan/default/frag.spv"
                                           );

  upVulkanWrapper_->createFrameBuffer( );

  upVulkanWrapper_->createCommandPool( );

  upVulkanWrapper_->createCommandBuffers( );

  upVulkanWrapper_->createSemaphores( );
}



/////////////////////////////////////////////
/// \brief Renderer::~Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
VulkanIOHandler::~VulkanIOHandler( )
{}



/////////////////////////////////////////////
/// \brief Renderer::render
/// \param alpha
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
VulkanIOHandler::showWorld( const double alpha )
{
  onRender( alpha );
} // VulkanIOHandler::showWorld



/////////////////////////////////////////////
/// \brief VulkanIOHandler::updateIO
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
VulkanIOHandler::updateIO( )
{
  upVulkanWrapper_->checkInputEvents( );

  //
  // check windows for exit requests
  //
  exitRequested_ |= upVulkanWrapper_->checkWindowShouldClose( );
}



/////////////////////////////////////////////
/// \brief Renderer::onRender
/// \param alpha
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
VulkanIOHandler::onRender( const double )
{
  upVulkanWrapper_->drawFrame( );
} // TerrainIOHandler::onRender



/////////////////////////////////////////////
/// \brief IOHandler::onLoopExit
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
VulkanIOHandler::onLoopExit( )
{
  upVulkanWrapper_->syncDevice( );
}



} // namespace shs
