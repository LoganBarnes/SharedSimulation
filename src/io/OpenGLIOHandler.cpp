#include "OpenGLIOHandler.hpp"

#include <iostream>

#include "glad/glad.h"
#include "graphics/glfw/GlfwWrapper.hpp"
#include "graphics/opengl/OpenGLWrapper.hpp"
#include "graphics/Camera.hpp"
#include "io/SharedCallback.hpp"


namespace shared
{



/////////////////////////////////////////////
/// \brief OpenGLIOHandler::OpenGLIOHandler
///
/// \author Logan Barnes
/////////////////////////////////////////////
OpenGLIOHandler::OpenGLIOHandler(
                                 World &world,
                                 bool  printInfo,
                                 int   width,
                                 int   height,
                                 bool  resizable
                                 )
  : IOHandler( world, false )
  , upGlfwWrapper_( new graphics::GlfwWrapper( ) )
  , upGLWrapper_  ( new graphics::OpenGLWrapper( width, height ) )
  , upCamera_     ( new graphics::Camera( ) )
{

  if ( printInfo )
  {

    std::cout << "Press 'ESC' to exit" << std::endl;

  }

  upGlfwWrapper_->createNewWindow( "OpenGL Window", width, height, resizable );

  std::unique_ptr< graphics::Callback > upCallback( new shared::SharedCallback() );
  upGlfwWrapper_->setCallback( std::move( upCallback ) );

}



/////////////////////////////////////////////
/// \brief OpenGLIOHandler::~OpenGLIOHandler
///
/// \author Logan Barnes
/////////////////////////////////////////////
OpenGLIOHandler::~OpenGLIOHandler( )
{}



/////////////////////////////////////////////
/// \brief OpenGLIOHandler::showWorld
/// \param alpha
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
OpenGLIOHandler::showWorld( const double alpha )
{

  onRender( alpha );

  upGlfwWrapper_->swapBuffers( );

} // OpenGLIOHandler::showWorld



/////////////////////////////////////////////
/// \brief OpenGLIOHandler::updateIO
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
OpenGLIOHandler::updateIO( )
{

  upGlfwWrapper_->pollEvents( );

  exitRequested_ |= ( upGlfwWrapper_->windowShouldClose( ) != 0 );

}



} // namespace shared
