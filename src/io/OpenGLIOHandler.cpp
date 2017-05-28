#include "shared/core/OpenGLIOHandler.hpp"

// shared
#include "shared/graphics/GlfwWrapper.hpp"
#include "shared/graphics/OpenGLHelper.hpp"
#include "shared/graphics/TCamera.hpp"
#include "shared/graphics/SharedCallback.hpp"
#include <glad/glad.h>

// system
#include <iostream>


namespace shs
{



///////////////////////////////////////////////////////////////
/// \brief OpenGLIOHandler::OpenGLIOHandler
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
OpenGLIOHandler::OpenGLIOHandler(
                                 World &world,
                                 bool  printInfo,
                                 int   width,
                                 int   height,
                                 bool  resizable,
                                 int   aaSamples
                                 )
  : IOHandler     ( world, false )
  , upGlfwWrapper_( new shg::GlfwWrapper )
  , upCamera_     ( new shg::Camera )
  , windowWidth_  ( width )
  , windowHeight_ ( height )
{
  if ( printInfo )
  {
    std::cout << "Press 'ESC' to exit" << std::endl;
  }

  upGlfwWrapper_->createNewWindow(
                                  "OpenGL Window",
                                  windowWidth_,
                                  windowHeight_,
                                  resizable,
                                  true,
                                  aaSamples
                                  );

  std::unique_ptr< shg::Callback > upCallback( new shs::SharedCallback( ) );
  upGlfwWrapper_->setCallback( std::move( upCallback ) );
  shg::OpenGLHelper::setDefaults( );

  upCamera_->setAspectRatio( windowWidth_ * 1.0f / windowHeight_ );
}



///////////////////////////////////////////////////////////////
/// \brief OpenGLIOHandler::~OpenGLIOHandler
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
OpenGLIOHandler::~OpenGLIOHandler( )
{}



///////////////////////////////////////////////////////////////
/// \brief OpenGLIOHandler::showWorld
/// \param alpha
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
void
OpenGLIOHandler::showWorld( const double alpha )
{
  glViewport( 0, 0, windowWidth_, windowHeight_ );
  _onRender( alpha );
  upGlfwWrapper_->swapBuffers( );
} // OpenGLIOHandler::showWorld



///////////////////////////////////////////////////////////////
/// \brief OpenGLIOHandler::updateIO
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
void
OpenGLIOHandler::updateIO( )
{
  upGlfwWrapper_->pollEvents( );
  exitRequested_ |= ( upGlfwWrapper_->windowShouldClose( ) != 0 );
}



///////////////////////////////////////////////////////////////
/// \brief OpenGLIOHandler::waitForIO
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
void
OpenGLIOHandler::waitForIO( )
{
  upGlfwWrapper_->waitEvents( );
  exitRequested_ |= ( upGlfwWrapper_->windowShouldClose( ) != 0 );
}



///////////////////////////////////////////////////////////////
/// \brief OpenGLIOHandler::resize
/// \param width
/// \param height
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
void
OpenGLIOHandler::resize(
                        const int width,
                        const int height
                        )
{
  windowWidth_  = width;
  windowHeight_ = height;
  upCamera_->setAspectRatio( windowWidth_ * 1.0f / windowHeight_ );
}



} // namespace shs
