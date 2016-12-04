#include "TerrainCallback.hpp"
#include "GLFW/glfw3.h"


namespace srt
{



TerrainCallback::TerrainCallback( )
  : graphics::Callback( )
{}



TerrainCallback::~TerrainCallback( )
{}



////////////////////////////////////////////////////////
/// \brief TerrainCallback::handleKey
/// \param pWindow
/// \param key
/// \param scancode
/// \param action
/// \param mods
////////////////////////////////////////////////////////
void
TerrainCallback::handleKey(
                           GLFWwindow *pWindow,
                           int         key,
                           int         scancode,
                           int         action,
                           int         mods
                           )
{

  if ( key == GLFW_KEY_ESCAPE )
  {

      glfwSetWindowShouldClose( pWindow, true );

  }

}


} // namespace srt
