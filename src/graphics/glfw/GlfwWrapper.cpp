#include "GlfwWrapper.hpp"

#include <iostream>

#include "glad/glad.h"

#define GLFW_INCLUDE_GL_3
#include "GLFW/glfw3.h"

#include "CallbackSingleton.hpp"


namespace graphics
{



///////////////////////////////////////////////////////////////////////////////////
//
//  Initialization functions
//
///////////////////////////////////////////////////////////////////////////////////


GlfwWrapper::GlfwWrapper( bool opengl )
  : glfwInitialized_ ( false )
  , pWindow_         ( nullptr )
{

  if ( !_initGlfw( opengl ) )
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


#ifdef USE_VULKAN

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

#endif



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
                             const int          height,
                             const bool         resizable,
                             const bool         initOpengl
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

  if ( resizable )
  {

    glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );

  }
  else
  {

    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

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


  if ( initOpengl )
  {

    glfwMakeContextCurrent( pWindow_ );
    glfwSwapInterval( 0 );


    if ( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) ) )
    {

      throw std::runtime_error( "Failed to initialize OpenGL context" );

    }

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
GlfwWrapper::_initGlfw( bool opengl )
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

  if ( opengl )
  {

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR,                 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR,                 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,           GL_TRUE );
#endif

  }
  else
  {

    //
    // not using openGL so tell glfw
    //
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

  }


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