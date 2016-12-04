#include "OpenGLIOHandler.hpp"

#include <iostream>

#include "glad/glad.h"
#include "graphics/glfw/GlfwWrapper.hpp"


namespace shared
{



/////////////////////////////////////////////
/// \brief Renderer::Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
OpenGLIOHandler::OpenGLIOHandler(
                     World &world,
                     bool   printInfo
                     )
  :
    IOHandler( world, false )
  , upGlfwWrapper_( new graphics::GlfwWrapper( ) )
{

  if ( printInfo )
  {

    std::cout << "Press 'ESC' to exit" << std::endl;

  }

  upGlfwWrapper_->createNewWindow( "Blah", 509, 590 );

  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

}



/////////////////////////////////////////////
/// \brief Renderer::~Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
OpenGLIOHandler::~OpenGLIOHandler( )
{}



/////////////////////////////////////////////
/// \brief Renderer::render
/// \param alpha
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
OpenGLIOHandler::showWorld( const double alpha )
{

  onRender( alpha );

  upGlfwWrapper_->swapBuffers();

} // OpenGLIOHandler::showWorld



/////////////////////////////////////////////
/// \brief OpenGLIOHandler::updateIO
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
OpenGLIOHandler::updateIO( )
{

  upGlfwWrapper_->pollEvents();

  exitRequested_ |= upGlfwWrapper_->windowShouldClose();

}



} // namespace shared
