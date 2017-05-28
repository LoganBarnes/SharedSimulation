#include "shared/core/ImguiOpenGLIOHandler.hpp"

// shared
#include "shared/graphics/GlfwWrapper.hpp"
#include "shared/graphics/OpenGLHelper.hpp"
#include "shared/graphics/ImguiCallback.hpp"
#include "shared/graphics/CameraCallback.hpp"
#include "shared/graphics/CameraMover.hpp"
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glad/glad.h>

// system
#include <iostream>


namespace shs
{



///////////////////////////////////////////////////////////////
/// \brief ImguiOpenGLIOHandler::ImguiOpenGLIOHandler
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
ImguiOpenGLIOHandler::ImguiOpenGLIOHandler(
                                           World      &world,
                                           bool        printInfo,
                                           std::string title,
                                           int         width,
                                           int         height,
                                           bool        resizable,
                                           int         aaSamples
                                           )
  : OpenGLIOHandler( world, printInfo, title, width, height, resizable, aaSamples )
{
  imguiCallback_ = new shs::ImguiCallback( );

  // imguiCallback no longer has ownership of memory
  std::unique_ptr< shg::Callback > upCallback( imguiCallback_ );
  upGlfwWrapper_->setCallback( std::move( upCallback ) );

  // default callback
  auto upCameraCallback = std::make_unique< shs::CameraCallback >( *upCameraMover_ );
  imguiCallback_->setCallback ( std::move( upCameraCallback ) );

  ImGui_ImplGlfwGL3_Init( upGlfwWrapper_->getWindow( ), false ); // false for no callbacks
}



///////////////////////////////////////////////////////////////
/// \brief ImguiOpenGLIOHandler::~ImguiOpenGLIOHandler
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
ImguiOpenGLIOHandler::~ImguiOpenGLIOHandler( )
{
  ImGui_ImplGlfwGL3_Shutdown( );
}



///////////////////////////////////////////////////////////////
/// \brief ImguiOpenGLIOHandler::showWorld
/// \param alpha
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
void
ImguiOpenGLIOHandler::showWorld( const double alpha )
{
  int w, h;

  upGlfwWrapper_->getWindowSize( &w, &h );
  glViewport( 0, 0, w, h );

  //
  // ImGui handles automatic resizing and event changes in the
  // frame following the one where the event took place.
  //
  // When rendering in an event based driver it isn't guaranteed
  // that another frame will be drawn immediately after the current
  // one. Therefore, we render the gui twice to ensure all changes
  // are captured in the current frame.
  //
  if ( eventBased_ )
  {
    ImGui_ImplGlfwGL3_NewFrame( );
    _onGuiRender( );
    ImGui::Render( );
  }

  ImGui_ImplGlfwGL3_NewFrame( );

  _onGuiRender( );

  _onRender( alpha );

  ImGui::Render( );

  upGlfwWrapper_->swapBuffers( );

} // ImguiOpenGLIOHandler::showWorld



void
ImguiOpenGLIOHandler::_onRender( const double )
{
  shg::OpenGLHelper::clearFramebuffer( );
}



void
ImguiOpenGLIOHandler::_onGuiRender( )
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
}



} // namespace shs
