#include "OpenGLIOHandler.hpp"

#include <iostream>

#include "glad/glad.h"
#include "graphics/glfw/GlfwWrapper.hpp"
#include "io/SharedCallback.hpp"


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
  , upCallback_   ( new shared::SharedCallback( ) )
{

  if ( printInfo )
  {

    std::cout << "Press 'ESC' to exit" << std::endl;

  }

  upGlfwWrapper_->createNewWindow( "OpenGL Window", 1080, 720 );

  upGlfwWrapper_->setCallback( upCallback_.get( ) );

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
