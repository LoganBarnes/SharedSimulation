#include "shared/core/OpenGLIOHandler.hpp"

// system
#include <iostream>

// shared
#include "glad/glad.h"
#include "shared/graphics/GlfwWrapper.hpp"
#include "shared/graphics/OpenGLWrapper.hpp"
#include "shared/graphics/Camera.hpp"
#include "shared/graphics/GlmCamera.hpp"
#include "shared/graphics/SharedCallback.hpp"


namespace shared
{



/////////////////////////////////////////////
/// \brief OpenGLIOHandler::OpenGLIOHandler
///
/// \author Logan Barnes
/////////////////////////////////////////////
OpenGLIOHandler::OpenGLIOHandler(
                                 World &world,
                                 bool   printInfo,
                                 int    width,
                                 int    height,
                                 bool   resizable,
                                 int    aaSamples
                                 )
  : IOHandler( world, false )
  , upGlfwWrapper_( new graphics::GlfwWrapper( ) )
  , upGLWrapper_  ( new graphics::OpenGLWrapper( ) )
  , upCamera_     ( new graphics::Camera< float >( ) )
  , upGlmCamera_  ( new graphics::GlmCamera< float >( ) )
{

  if ( printInfo )
  {
    std::cout << "Press 'ESC' to exit" << std::endl;
  }

  upGlfwWrapper_->createNewWindow( "OpenGL Window", width, height, resizable, true, aaSamples );

  std::unique_ptr< graphics::Callback > upCallback( new shared::SharedCallback( ) );
  upGlfwWrapper_->setCallback( std::move( upCallback ) );

  upGLWrapper_->setCurrentContext( upGlfwWrapper_->getWindow( ) ); // optional with only one window
  upGLWrapper_->initContext( width, height );

  upCamera_->setAspectRatio( width * 1.0f / height );
  upGlmCamera_->setAspectRatio( width * 1.0f / height );
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

  _onRender( alpha );

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



/////////////////////////////////////////////
/// \brief OpenGLIOHandler::waitForIO
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
OpenGLIOHandler::waitForIO( )
{

  upGlfwWrapper_->waitEvents( );

  exitRequested_ |= ( upGlfwWrapper_->windowShouldClose( ) != 0 );

}



/////////////////////////////////////////////
/// \brief OpenGLIOHandler::resize
/// \param width
/// \param height
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
OpenGLIOHandler::resize(
                        const int width,
                        const int height
                        )
{
  upCamera_->setAspectRatio( width * 1.0f / height );
  upGlmCamera_->setAspectRatio( width * 1.0 / height );
  upGLWrapper_->setViewportSize( width, height );
}



} // namespace shared
