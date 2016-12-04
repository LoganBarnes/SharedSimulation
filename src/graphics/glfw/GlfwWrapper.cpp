#include "GlfwWrapper.hpp"

#include <iostream>

#include "GLFW/glfw3.h"

#include "CallbackSingleton.hpp"


namespace graphics
{



///////////////////////////////////////////////////////////////////////////////////
//
//  Initialization functions
//
///////////////////////////////////////////////////////////////////////////////////


GlfwWrapper::GlfwWrapper(  )
  : glfwInitialized_        ( false )
  , pWindow_                ( nullptr )
{

  if ( !_initGlfw( ) )
  {
    throw std::runtime_error( "Failed to initialize GLFW" );

  }


}



///
/// \brief GlfwWrapper::~GlfwWrapper
///
GlfwWrapper::~GlfwWrapper( )
{

  if ( pWindow_ )
  {

    glfwDestroyWindow( pWindow_ );
    pWindow_ = nullptr;

  }

  _terminateGlfw( );

}



///
/// \brief GlfwWrapper::getWindow
/// \return
///
GLFWwindow*
GlfwWrapper::getWindow( )
{

  return pWindow_;

}



///
/// \brief GlfwWrapper::getRequiredInstanceExtensions
/// \param pCount
/// \return
///
const char**
GlfwWrapper::getRequiredInstanceExtensions( unsigned int *pCount )
{

  return glfwGetRequiredInstanceExtensions( pCount );

}



///
/// \brief GlfwWrapper::createWindowSurface
/// \param instance
/// \param pAllocator
/// \param pSurface
/// \return
///
VkResult
GlfwWrapper::createWindowSurface(
                                 VkInstance                   instance,
                                 const VkAllocationCallbacks *pAllocator,
                                 VkSurfaceKHR                *pSurface
                                 )
{

  return glfwCreateWindowSurface( instance, pWindow_, pAllocator, pSurface );

}



///
/// \brief GlfwWrapper::createNewWindow
/// \param title
/// \param width
/// \param height
///
void
GlfwWrapper::createNewWindow(
                             const std::string &title,
                             const int          width,
                             const int          height
                             )
{

  //
  // no title means no visibility
  //
  if ( title.length( ) )
  {

    glfwWindowHint( GLFW_VISIBLE, GL_TRUE );

  }
  else
  {

    glfwWindowHint( GLFW_VISIBLE, GL_FALSE );

  }

  pWindow_ = glfwCreateWindow(
                              width,
                              height,
                              title.c_str( ),
                              nullptr,
                              nullptr
                              );

  if ( !pWindow_ )
  {

    throw std::runtime_error( "GLFW window creation failed" );

  }

  // set default callback functions
  glfwSetWindowSizeCallback ( pWindow_, CallbackSingleton::windowSizeCallback  );
  glfwSetWindowFocusCallback( pWindow_, CallbackSingleton::windowFocusCallback );

  glfwSetMouseButtonCallback( pWindow_, CallbackSingleton::mouseButtonCallback    );
  glfwSetKeyCallback        ( pWindow_, CallbackSingleton::keyCallback            );
  glfwSetCursorPosCallback  ( pWindow_, CallbackSingleton::cursorPositionCallback );
  glfwSetScrollCallback     ( pWindow_, CallbackSingleton::scrollCallback         );

} // GlfwWrapper::createNewWindow



///////////////////////////////////////////////////////////////////////////////////
//
//  Render loop functions
//
///////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
/// \brief GlfwWrapper::pollEvents
//////////////////////////////////////////////////
void
GlfwWrapper::pollEvents( )
{

  glfwPollEvents( );

}



///
/// \brief GlfwWrapper::swapBuffers
///
void
GlfwWrapper::swapBuffers( )
{

  glfwSwapBuffers( pWindow_ );

}



///
/// \brief GlfwWrapper::windowShouldClose
///
int
GlfwWrapper::windowShouldClose( )
{

  return glfwWindowShouldClose( pWindow_ );

}


///
/// \brief GlfwWrapper::setCallback
/// \param pCallback
///
void
GlfwWrapper::setCallback( Callback *pCallback )
{

  CallbackSingleton::getInstance( ).setDefaultCallback( pCallback );

}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////                                        ////////////////////
///////////////////////        START PRIVATE FUNCTIONS         ////////////////////
///////////////////////                                        ////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////



///
/// \brief GlfwWrapper::_initGlfw
/// \return
///
bool
GlfwWrapper::_initGlfw( )
{

  //
  // set error callback before anything else to get
  // error messages from future calls
  //
  glfwSetErrorCallback( CallbackSingleton::errorCallback );

  //
  // If we aren't already initialized then try
  // to initialized GLFW.
  //
  if ( !glfwInitialized_ && !glfwInit( ) )
  {

    //
    // init failed
    //
    return false;

  }

  std::cout << "Initialized GLFW Version: ";
  std::cout << glfwGetVersionString( ) << std::endl;


  glfwInitialized_ = true;

  //
  // using vulkan so we don't need OpenGL
  //
  glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

  //
  // temporary until we handle resizing
  //
  glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

  return true;

} // GlfwWrapper::_initGlfw



///
/// \brief GlfwWrapper::_terminateGlfw
///
void
GlfwWrapper::_terminateGlfw( )
{

  if ( glfwInitialized_ )
  {

    glfwTerminate( );
    glfwInitialized_ = false;
    //
    // ready to be initialized again if necessary
    //
    std::cout << "Terminated GLFW" << std::endl;

  }

}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////                                        ////////////////////
///////////////////////         END PRIVATE FUNCTIONS          ////////////////////
///////////////////////                                        ////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


} // namespace graphics
