#include "CubeCallback.hpp"
#include "GLFW/glfw3.h"
#include "CubeImguiOpenGLIOHandler.hpp"


namespace example
{


CubeCallback::CubeCallback( CubeImguiOpenGLIOHandler &handler )
  : shared::SharedCallback( )
  , handler_       ( handler )
{}



CubeCallback::~CubeCallback( )
{}



///
/// \brief CubeCallback::handleWindowSize
/// \param pWindow
/// \param width
/// \param height
///
void
CubeCallback::handleWindowSize(
                               GLFWwindow*,
                               int width,
                               int height
                               )
{
  handler_.resize( width, height );
}



///
/// \brief CubeCallback::handleKey
/// \param pWindow
/// \param key
/// \param scancode
/// \param action
/// \param mods
///
void
CubeCallback::handleKey(
                        GLFWwindow *pWindow,
                        int         key,
                        int,
                        int         action,
                        int
                        )
{

  if ( action == GLFW_RELEASE )
  {

    switch ( key )
    {

    case GLFW_KEY_ESCAPE:

      glfwSetWindowShouldClose( pWindow, GL_TRUE );

      break;


    case GLFW_KEY_A:

      handler_.addRandomCube( );

      break;


    case GLFW_KEY_R:

      handler_.removeOldestCube( );

      break;


    default:

      break;

    } // switch

  }

} // handleKey



} //  namespace example
