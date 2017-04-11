#include "shared/core/ImguiOpenGLIOHandler.hpp"

// system
#include <iostream>

// shared
#include "glad/glad.h"
#include "shared/graphics/GlfwWrapper.hpp"
#include "shared/graphics/OpenGLWrapper.hpp"
#include "shared/graphics/ImguiCallback.hpp"
#include "shared/graphics/SharedCallback.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"


namespace shared
{



/////////////////////////////////////////////
/// \brief ImguiOpenGLIOHandler::ImguiOpenGLIOHandler
///
/// \author Logan Barnes
/////////////////////////////////////////////
ImguiOpenGLIOHandler::ImguiOpenGLIOHandler(
                                           World &world,
                                           bool   printInfo,
                                           int    width,
                                           int    height,
                                           bool   resizable,
                                           int    aaSamples
                                           )
  : OpenGLIOHandler( world, printInfo, width, height, resizable, aaSamples )
{
  imguiCallback_ = new shared::ImguiCallback( );

  // imguiCallback no longer has ownership of memory
  std::unique_ptr< graphics::Callback > upCallback( imguiCallback_ );
  upGlfwWrapper_->setCallback( std::move( upCallback ) );

  // default callback
  std::unique_ptr< shared::SharedCallback > upSharedCallback( new shared::SharedCallback( ) );
  imguiCallback_->setCallback ( std::move( upSharedCallback ) );

  ImGui_ImplGlfwGL3_Init( upGlfwWrapper_->getWindow( ), false ); // false for no callbacks
}



/////////////////////////////////////////////
/// \brief ImguiOpenGLIOHandler::~ImguiOpenGLIOHandler
///
/// \author Logan Barnes
/////////////////////////////////////////////
ImguiOpenGLIOHandler::~ImguiOpenGLIOHandler( )
{
  ImGui_ImplGlfwGL3_Shutdown( );
}



/////////////////////////////////////////////
/// \brief ImguiOpenGLIOHandler::showWorld
/// \param alpha
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
ImguiOpenGLIOHandler::showWorld( const double alpha )
{
  ImGui_ImplGlfwGL3_NewFrame( );

  _onGuiRender( );

  _onRender( alpha );

  ImGui::Render( );

  upGlfwWrapper_->swapBuffers( );
} // ImguiOpenGLIOHandler::showWorld



void
ImguiOpenGLIOHandler::_onRender ( const double )
{
  upGLWrapper_->clearWindow( );
};


void
ImguiOpenGLIOHandler::_onGuiRender ( )
{
  bool alwaysOpen = true;

  ImGui::SetNextWindowSize( ImVec2( 0, 0 ), ImGuiSetCond_FirstUseEver ); // auto scale size
  ImGui::SetNextWindowPos ( ImVec2( 0, 0 ), ImGuiSetCond_FirstUseEver );

  ImGui::Begin( "Example Window", &alwaysOpen );

  if ( ImGui::CollapsingHeader( "Controls", "controls", false, true ) )
  {
    ImGui::Text( "Press ESC to exit sim  \n" );
  }

  ImGui::End( );
};


} // namespace shared
