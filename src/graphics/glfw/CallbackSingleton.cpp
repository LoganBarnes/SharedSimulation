#include "CallbackSingleton.hpp"
#include <istream>
#include "Callback.hpp"
#include "GLFW/glfw3.h"


namespace graphics
{

//
// Private contructor and destructor
//

///
/// \brief CallbackSingleton::CallbackSingleton
///
CallbackSingleton::CallbackSingleton( )
  : defaultCallbacks_( new Callback( ) )
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
CallbackSingleton::getInstance( )      // Singleton is accessed via getInstance()
{

  static CallbackSingleton instance;   // lazy singleton, instantiated on first use

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

  defaultCallbacks_->handleError( error, description );

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

    defaultCallbacks_->handleWindowSize( pWindow, width, height );

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

    defaultCallbacks_->handleWindowFocus( pWindow, focus );

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

    defaultCallbacks_->handleKey( pWindow, key, scancode, action, mods );

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

    defaultCallbacks_->handleCursorPosition( pWindow, xpos, ypos );

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

    defaultCallbacks_->handleMouseButton( pWindow, button, action, mods );

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

    defaultCallbacks_->handleScroll( pWindow, xoffset, yoffset );

  }

} // CallbackSingleton::defaultScrollCallback



///
/// \brief CallbackSingleton::setDefaultCallback
/// \param callback
///
void
CallbackSingleton::setDefaultCallback( Callback *pCallback )
{

  if ( pCallback )
  {

    delete defaultCallbacks_;
    defaultCallbacks_ = pCallback;

  }

}



} // namespace graphics
