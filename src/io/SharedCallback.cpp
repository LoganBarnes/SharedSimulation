#include "shared/graphics/SharedCallback.hpp"
#include "shared/core/OpenGLIOHandler.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace shs
{



SharedCallback::SharedCallback( )
  : shg::Callback    ( )
  , shiftDown_     ( false )
  , ctrlDown_      ( false )
  , leftMouseDown_ ( false )
  , rightMouseDown_( false )
{}



////////////////////////////////////////////////////////
/// \brief CameraCallback<T>::handleMouseButton
/// \param pWindow
/// \param button
/// \param action
/// \param button
/// \param mods
////////////////////////////////////////////////////////
void
SharedCallback::handleMouseButton(
                                  GLFWwindow *pWindow,
                                  int         button,
                                  int         action,
                                  int
                                  )
{
  if ( button == GLFW_MOUSE_BUTTON_1 )
  {
    if ( action == GLFW_PRESS )
    {
      leftMouseDown_ = true;
      glfwGetCursorPos( pWindow, &prevX_, &prevY_ );
    }
    else
    {
      leftMouseDown_ = false;
    }
  }
  else
  if ( button == GLFW_MOUSE_BUTTON_2 )
  {
    if ( action == GLFW_PRESS )
    {
      rightMouseDown_ = true;
      glfwGetCursorPos( pWindow, &prevX_, &prevY_ );
    }
    else
    {
      rightMouseDown_ = false;
    }
  }
} // handleMouseButton



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
