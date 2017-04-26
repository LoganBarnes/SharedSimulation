#include "CallbackSingleton.hpp"
#include "shared/graphics/Callback.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace graphics
{

//
// Private contructor and destructor
//

///
/// \brief CallbackSingleton::CallbackSingleton
///
CallbackSingleton::CallbackSingleton( )
  : upDefaultCallbacks_( new Callback( ) )
{}



///
/// \brief CallbackSingleton::~CallbackSingleton
///
CallbackSingleton::~CallbackSingleton( ) noexcept
{}



///
/// \brief CallbackSingleton::getInstance
/// \return
///
CallbackSingleton&
CallbackSingleton::getInstance( ) // Singleton is accessed via getInstance()
{
  static CallbackSingleton instance; // lazy singleton, instantiated on first use

  return instance;
}



//
//
// STATIC CALLBACKS
//
//

///
/// \brief CallbackSingleton::errorCallback
/// \param error
/// \param description
///
void
CallbackSingleton::errorCallback(
                                 int         error,
                                 const char *description
                                 )
{
  CallbackSingleton::getInstance( ).defaultErrorCallback( error, description );
}



///
/// \brief CallbackSingleton::windowSizeCallback
/// \param pWindow
/// \param width
/// \param height
///
void
CallbackSingleton::windowSizeCallback(
                                      GLFWwindow *pWindow,
                                      int         width,
                                      int         height
                                      )
{
  CallbackSingleton::getInstance( ).defaultWindowSizeCallback( pWindow, width, height );
}



///
/// \brief CallbackSingleton::windowFocusCallback
/// \param pWindow
/// \param focus
///
void
CallbackSingleton::windowFocusCallback(
                                       GLFWwindow *pWindow,
                                       int         focus
                                       )
{
  CallbackSingleton::getInstance( ).defaultWindowFocusCallback( pWindow, focus );
}



///
/// \brief CallbackSingleton::keyCallback
/// \param pWindow
/// \param key
/// \param scancode
/// \param action
/// \param mods
///
void
CallbackSingleton::keyCallback(
                               GLFWwindow *pWindow,
                               int         key,
                               int         scancode,
                               int         action,
                               int         mods
                               )
{
  CallbackSingleton::getInstance( ).defaultKeyCallback( pWindow, key, scancode, action, mods );
}



///
/// \brief CallbackSingleton::cursorPositionCallback
/// \param pWindow
/// \param xpos
/// \param ypos
///
void
CallbackSingleton::cursorPositionCallback(
                                          GLFWwindow *pWindow,
                                          double      xpos,
                                          double      ypos
                                          )
{

  CallbackSingleton::getInstance( ).defaultCursorPositionCallback( pWindow, xpos, ypos );

}



///
/// \brief CallbackSingleton::mouseButtonCallback
/// \param pWindow
/// \param button
/// \param action
/// \param mods
///
void
CallbackSingleton::mouseButtonCallback(
                                       GLFWwindow *pWindow,
                                       int         button,
                                       int         action,
                                       int         mods
                                       )
{
  CallbackSingleton::getInstance( ).defaultMouseButtonCallback( pWindow, button, action, mods );
}



///
/// \brief CallbackSingleton::scrollCallback
/// \param pWindow
/// \param xoffset
/// \param yoffset
///
void
CallbackSingleton::scrollCallback(
                                  GLFWwindow *pWindow,
                                  double      xoffset,
                                  double      yoffset
                                  )
{
  CallbackSingleton::getInstance( ).defaultScrollCallback( pWindow, xoffset, yoffset );
}



///
/// \brief CallbackSingleton::charCallback
/// \param pWindow
/// \param codepoint
///
void
CallbackSingleton::charCallback(
                                GLFWwindow *pWindow,
                                unsigned    codepoint
                                )
{
  CallbackSingleton::getInstance( ).defaultCharCallback( pWindow, codepoint );
}



void
CallbackSingleton::windowRefreshCallback( GLFWwindow *pWindow )
{
  CallbackSingleton::getInstance( ).defaultWindowRefreshCallback( pWindow );
}



//
//
// ACTUAL MEMBER IMPLEMENTATIONS
//
//


///
/// \brief CallbackSingleton::defaultErrorCallback
/// \param error
/// \param description
///
void
CallbackSingleton::defaultErrorCallback(
                                        int         error,
                                        const char *description
                                        )
{
  upDefaultCallbacks_->handleError( error, description );
}



///
/// \brief CallbackSingleton::defaultWindowSizeCallback
/// \param pWindow
/// \param width
/// \param height
///
void
CallbackSingleton::defaultWindowSizeCallback(
                                             GLFWwindow *pWindow,
                                             int         width,
                                             int         height
                                             )
{
  Callback *pCallback = reinterpret_cast< Callback* >( glfwGetWindowUserPointer( pWindow ) );

  if ( pCallback )
  {
    pCallback->handleWindowSize( pWindow, width, height );
  }
  else
  {
    upDefaultCallbacks_->handleWindowSize( pWindow, width, height );
  }
} // CallbackSingleton::defaultWindowSizeCallback



///
/// \brief CallbackSingleton::defaultWindowFocusCallback
/// \param pWindow
/// \param focus
///
void
CallbackSingleton::defaultWindowFocusCallback(
                                              GLFWwindow *pWindow,
                                              int         focus
                                              )
{
  Callback *pCallback = reinterpret_cast< Callback* >( glfwGetWindowUserPointer( pWindow ) );

  if ( pCallback )
  {
    pCallback->handleWindowFocus( pWindow, focus );
  }
  else
  {
    upDefaultCallbacks_->handleWindowFocus( pWindow, focus );
  }
} // CallbackSingleton::defaultWindowFocusCallback



///
/// \brief CallbackSingleton::defaultKeyCallback
/// \param pWindow
/// \param key
/// \param scancode
/// \param action
/// \param mods
///
void
CallbackSingleton::defaultKeyCallback(
                                      GLFWwindow *pWindow,
                                      int         key,
                                      int         scancode,
                                      int         action,
                                      int         mods
                                      )
{
  Callback *pCallback = reinterpret_cast< Callback* >( glfwGetWindowUserPointer( pWindow ) );

  if ( pCallback )
  {
    pCallback->handleKey( pWindow, key, scancode, action, mods );
  }
  else
  {
    upDefaultCallbacks_->handleKey( pWindow, key, scancode, action, mods );
  }
} // CallbackSingleton::defaultKeyCallback



///
/// \brief CallbackSingleton::defaultCursorPositionCallback
/// \param pWindow
/// \param xpos
/// \param ypos
///
void
CallbackSingleton::defaultCursorPositionCallback(
                                                 GLFWwindow *pWindow,
                                                 double      xpos,
                                                 double      ypos
                                                 )
{
  Callback *pCallback = reinterpret_cast< Callback* >( glfwGetWindowUserPointer( pWindow ) );

  if ( pCallback )
  {
    pCallback->handleCursorPosition( pWindow, xpos, ypos );
  }
  else
  {
    upDefaultCallbacks_->handleCursorPosition( pWindow, xpos, ypos );
  }
} // CallbackSingleton::defaultCursorPositionCallback



///
/// \brief CallbackSingleton::defaultMouseButtonCallback
/// \param pWindow
/// \param button
/// \param action
/// \param mods
///
void
CallbackSingleton::defaultMouseButtonCallback(
                                              GLFWwindow *pWindow,
                                              int         button,
                                              int         action,
                                              int         mods
                                              )
{
  Callback *pCallback = reinterpret_cast< Callback* >( glfwGetWindowUserPointer( pWindow ) );

  if ( pCallback )
  {
    pCallback->handleMouseButton( pWindow, button, action, mods );
  }
  else
  {
    upDefaultCallbacks_->handleMouseButton( pWindow, button, action, mods );
  }
} // CallbackSingleton::defaultMouseButtonCallback



///
/// \brief CallbackSingleton::defaultScrollCallback
/// \param pWindow
/// \param xoffset
/// \param yoffset
///
void
CallbackSingleton::defaultScrollCallback(
                                         GLFWwindow *pWindow,
                                         double      xoffset,
                                         double      yoffset
                                         )
{
  Callback *pCallback = reinterpret_cast< Callback* >( glfwGetWindowUserPointer( pWindow ) );

  if ( pCallback )
  {
    pCallback->handleScroll( pWindow, xoffset, yoffset );
  }
  else
  {
    upDefaultCallbacks_->handleScroll( pWindow, xoffset, yoffset );
  }
} // CallbackSingleton::defaultScrollCallback



void
CallbackSingleton::defaultCharCallback(
                                       GLFWwindow *pWindow,
                                       unsigned    codepoint
                                       )
{
  Callback *pCallback = reinterpret_cast< Callback* >( glfwGetWindowUserPointer( pWindow ) );

  if ( pCallback )
  {
    pCallback->handleChar( pWindow, codepoint );
  }
  else
  {
    upDefaultCallbacks_->handleChar( pWindow, codepoint );
  }
} // CallbackSingleton::defaultCharCallback



///
/// \brief CallbackSingleton::defaultWindowRefreshCallback
/// \param pWindow
///
void
CallbackSingleton::defaultWindowRefreshCallback( GLFWwindow *pWindow )
{
  Callback *pCallback = reinterpret_cast< Callback* >( glfwGetWindowUserPointer( pWindow ) );

  if ( pCallback )
  {
    pCallback->handleWindowRefresh( pWindow );
  }
  else
  {
    upDefaultCallbacks_->handleWindowRefresh( pWindow );
  }
} // CallbackSingleton::defaultWindowRefreshCallback



///
/// \brief CallbackSingleton::setDefaultCallback
/// \param upCallback
///
void
CallbackSingleton::setDefaultCallback( std::unique_ptr< Callback > upCallback )
{
  upDefaultCallbacks_ = std::move( upCallback );
}



} // namespace graphics
