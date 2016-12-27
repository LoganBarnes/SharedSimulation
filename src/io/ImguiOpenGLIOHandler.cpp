#include "ImguiOpenGLIOHandler.hpp"

// system
#include <iostream>

// shared
#include "glad/glad.h"
#include "graphics/glfw/GlfwWrapper.hpp"
#include "io/ImguiCallback.hpp"
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
                                           bool  printInfo,
                                           int   width,
                                           int   height,
                                           bool  resizable
                                           )
  : OpenGLIOHandler( world, printInfo, width, height, resizable )
{

  imguiCallback_ = new shared::ImguiCallback();

  // imguiCallback no longer has ownership of memory
  std::unique_ptr< graphics::Callback > upCallback( imguiCallback_ );
  upGlfwWrapper_->setCallback( std::move( upCallback ) );

  ImGui_ImplGlfwGL3_Init( upGlfwWrapper_->getWindow( ), false ); // false for no callbacks

}



/////////////////////////////////////////////
/// \brief ImguiOpenGLIOHandler::~ImguiOpenGLIOHandler
///
/// \author Logan Barnes
/////////////////////////////////////////////
ImguiOpenGLIOHandler::~ImguiOpenGLIOHandler( )
{

  ImGui_ImplGlfwGL3_Shutdown();

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

  _onRender( alpha );


  ImGui_ImplGlfwGL3_NewFrame();

  _onGuiRender( );

  ImGui::Render();

  upGlfwWrapper_->swapBuffers( );

} // ImguiOpenGLIOHandler::showWorld




} // namespace shared
