#include "VulkanIOHandler.hpp"

#include <iostream>

#include "graphics/vulkan/VulkanGlfwWrapper.hpp"
#include "io/SharedCallback.hpp"


namespace shared
{



/////////////////////////////////////////////
/// \brief Renderer::Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
VulkanIOHandler::VulkanIOHandler(
                     World &world,
                     bool   printInfo
                     )
  :
    IOHandler( world, false )
  , upVulkanWrapper_( new graphics::VulkanGlfwWrapper( ) )
  , upCallback_     ( new shared::SharedCallback( ) )
{

  if ( printInfo )
  {

    std::cout << "Press 'ESC' to exit" << std::endl;

  }

  upVulkanWrapper_->setCallback( upCallback_.get( ) );

  upVulkanWrapper_->createNewWindow( "VulkanWindow", 1024, 720 );


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



} // namespace shared
