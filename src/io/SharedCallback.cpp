#include "shared/graphics/SharedCallback.hpp"
#include "shared/core/OpenGLIOHandler.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace shs
{



SharedCallback::SharedCallback( OpenGLIOHandler *pHandler )
  : shg::Callback( )
  , pHandler_( pHandler )
  , shiftDown_( false )
  , ctrlDown_ ( false )
{}



SharedCallback::~SharedCallback( )
{}



////////////////////////////////////////////////////////
/// \brief HeightsCallback::handleWindowSize
/// \param pWindow
/// \param width
/// \param height
////////////////////////////////////////////////////////
void
SharedCallback::handleWindowSize(
                                 GLFWwindow*,
                                 int width,
                                 int height
                                 )
{
  if ( pHandler_ )
  {
    pHandler_->resize( width, height );
  }
}



////////////////////////////////////////////////////////
/// \brief SharedCallback::handleKey
/// \param pWindow
/// \param key
/// \param scancode
/// \param action
/// \param mods
////////////////////////////////////////////////////////
void
SharedCallback::handleKey(
                          GLFWwindow *pWindow,
                          int         key,
                          int,
                          int         action,
                          int
                          )
{
  if ( key == GLFW_KEY_ESCAPE )
  {
    glfwSetWindowShouldClose( pWindow, true );
    return;
  }

  switch ( key )
  {
  case GLFW_KEY_LEFT_SHIFT:
  case GLFW_KEY_RIGHT_SHIFT:
    shiftDown_ = ( action != GLFW_RELEASE );
    break;

  case GLFW_KEY_LEFT_CONTROL:
  case GLFW_KEY_RIGHT_CONTROL:
    ctrlDown_ = ( action != GLFW_RELEASE );
    break;

  default:
    break;
  }   // switch

} // SharedCallback::handleKey



} // namespace shs
