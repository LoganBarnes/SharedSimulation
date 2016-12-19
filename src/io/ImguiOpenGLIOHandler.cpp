#include "ImguiOpenGLIOHandler.hpp"

#include <iostream>

#include "glad/glad.h"
#include "graphics/glfw/GlfwWrapper.hpp"
#include "io/SharedCallback.hpp"
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

  // std::unique_ptr< graphics::Callback > upCallback( new shared::SharedCallback() );
  // upGlfwWrapper_->setCallback( std::move( upCallback ) );

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

  onRender( alpha );


  ImGui_ImplGlfwGL3_NewFrame();

  onGuiRender( );

  ImGui::Render();

  upGlfwWrapper_->swapBuffers( );

} // ImguiOpenGLIOHandler::showWorld




} // namespace shared
