#include "shared/graphics/SharedCallback.hpp"
#include "shared/core/OpenGLIOHandler.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace shs
{



SharedCallback::SharedCallback( OpenGLIOHandler &handler )
  : shg::Callback( )
  , handler_  ( handler )
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
  handler_.resize( width, height );
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

  if ( action == GLFW_PRESS )
  {
    switch ( key )
    {
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
      shiftDown_ = true;
      break;

    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
      ctrlDown_ = true;
      break;

    default:
      break;
    } // switch

  }
  else
  if ( action == GLFW_RELEASE )
  {
    switch ( key )
    {
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
      shiftDown_ = false;
      break;

    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
      ctrlDown_ = false;
      break;

    default:
      break;
    } // switch

  }
} // SharedCallback::handleKey



} // namespace shs
