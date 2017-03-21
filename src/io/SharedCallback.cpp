#include "shared/SharedCallback.hpp"
#include "GLFW/glfw3.h"


namespace shared
{



SharedCallback::SharedCallback( )
  : graphics::Callback( )
{}



SharedCallback::~SharedCallback( )
{}



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



} // namespace shared
