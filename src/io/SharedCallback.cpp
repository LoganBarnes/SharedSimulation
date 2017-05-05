#include "shared/graphics/SharedCallback.hpp"
#include "shared/core/OpenGLIOHandler.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace shs
{



SharedCallback::SharedCallback( OpenGLIOHandler &handler )
  : shg::Callback( )
  , handler_( handler )
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
                          int,
                          int
                          )
{

  if ( key == GLFW_KEY_ESCAPE )
  {

    glfwSetWindowShouldClose( pWindow, true );

  }

}



} // namespace shs
