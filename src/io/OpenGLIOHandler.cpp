#include "shared/core/OpenGLIOHandler.hpp"

// shared
#include "shared/graphics/GlfwWrapper.hpp"
#include "shared/graphics/OpenGLHelper.hpp"
#include "shared/graphics/Camera.hpp"
#include "shared/graphics/CameraMover.hpp"
#include "shared/graphics/CameraCallback.hpp"
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
                                 World      &world,
                                 bool        printInfo,
                                 std::string title,
                                 int         width,
                                 int         height,
                                 bool        resizable,
                                 int         aaSamples
                                 )
  : IOHandler     ( world, false )
  , upGlfwWrapper_( new shg::GlfwWrapper )
  , upCamera_     ( new shg::Camera )
  , upCameraMover_( new shg::CameraMover( *upCamera_ ) )
{
  if ( printInfo )
  {
    std::cout << "Press 'ESC' to exit" << std::endl;
  }

  upGlfwWrapper_->createNewWindow(
                                  title,
                                  width,
                                  height,
                                  resizable,
                                  true,
                                  aaSamples
                                  );

  auto upCameraCallback = std::make_unique< shs::CameraCallback >( *upCameraMover_ );
  upGlfwWrapper_->setCallback( std::move( upCameraCallback ) );

  shg::OpenGLHelper::setDefaults( );

  upGlfwWrapper_->getWindowSize( &width, &height );
  upCamera_->setAspectRatio( width * 1.0f / height );
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
  int w, h;

  upGlfwWrapper_->getWindowSize( &w, &h );
  glViewport( 0, 0, w, h );
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
  upCameraMover_->update( );
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
  upCameraMover_->update( );
  exitRequested_ |= ( upGlfwWrapper_->windowShouldClose( ) != 0 );
}



///////////////////////////////////////////////////////////////
/// \brief OpenGLIOHandler::resize
///
/// \author Logan Barnes
///////////////////////////////////////////////////////////////
void
OpenGLIOHandler::resize(
                        const int width, ///< new window width
                        const int height ///< new window height
                        )
{
  upCamera_->setAspectRatio( width * 1.0f / height );
}



} // namespace shs
