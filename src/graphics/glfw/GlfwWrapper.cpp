// GlfwWrapper.hpp
#include "shared/graphics/GlfwWrapper.hpp"

#include "shared/graphics/Callback.hpp"
#include "graphics/glfw/CallbackSingleton.hpp"

#ifdef USE_OPENGL
#include <glad/glad.h>
#endif

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <iostream>


namespace shg
{



///////////////////////////////////////////////////////////////////////////////////
//
//  Initialization functions
//
///////////////////////////////////////////////////////////////////////////////////


GlfwWrapper::GlfwWrapper(
                         const bool print,
                         bool       opengl
                         )
  : glfwInitialized_ ( false )
  , pWindow_         ( nullptr )
  , print_           ( print )
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



#endif // ifdef USE_VULKAN



///
/// \brief GlfwWrapper::createNewWindow
/// \param title
/// \param width
/// \param height
///
void
GlfwWrapper::createNewWindow(
                             const std::string &title,
                             int                width,
                             int                height,
                             const bool         resizable,
                             const bool         initOpengl,
                             const int          sampleCount
                             )
{
  //
  // no title means no visibility
  //
  if ( title.length( ) )
  {
    glfwWindowHint( GLFW_VISIBLE, GLFW_TRUE );
  }
  else
  {
    glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );
  }


  if ( resizable )
  {
    glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );
  }
  else
  {
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
  }


  glfwWindowHint( GLFW_SAMPLES, sampleCount );

  GLFWmonitor *monitor = nullptr;

  if ( width == 0 || height == 0 )
  {
    monitor = glfwGetPrimaryMonitor( );

    const GLFWvidmode *mode = glfwGetVideoMode( monitor );

    glfwWindowHint( GLFW_RED_BITS,     mode->redBits );
    glfwWindowHint( GLFW_GREEN_BITS,   mode->greenBits );
    glfwWindowHint( GLFW_BLUE_BITS,    mode->blueBits );
    glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );

    width  = mode->width;
    height = mode->height;
  }

  pWindow_ = glfwCreateWindow(
                              width,
                              height,
                              title.c_str( ),
                              monitor,
                              nullptr
                              );

  if ( !pWindow_ )
  {
    throw std::runtime_error( "GLFW window creation failed" );
  }

  if ( initOpengl )
  {
    glfwMakeContextCurrent( pWindow_ );
    glfwSwapInterval( 1 );

#ifdef USE_OPENGL

    if ( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) ) )
    {
      throw std::runtime_error( "Failed to initialize OpenGL context" );
    }

#endif
  }

  // set default callback functions
  glfwSetWindowSizeCallback ( pWindow_, CallbackSingleton::windowSizeCallback  );
  glfwSetWindowFocusCallback( pWindow_, CallbackSingleton::windowFocusCallback );

  glfwSetMouseButtonCallback( pWindow_, CallbackSingleton::mouseButtonCallback    );
  glfwSetKeyCallback        ( pWindow_, CallbackSingleton::keyCallback            );
  glfwSetCursorPosCallback  ( pWindow_, CallbackSingleton::cursorPositionCallback );
  glfwSetScrollCallback     ( pWindow_, CallbackSingleton::scrollCallback         );
  glfwSetCharCallback       ( pWindow_, CallbackSingleton::charCallback           );
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



//////////////////////////////////////////////////
/// \brief GlfwWrapper::waitEvents
//////////////////////////////////////////////////
void
GlfwWrapper::waitEvents( )
{
  glfwWaitEvents( );
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
GlfwWrapper::setCallback( std::unique_ptr< Callback > upCallback )
{
  CallbackSingleton::getInstance( ).setDefaultCallback( std::move( upCallback ) );
}


void
GlfwWrapper::getWindowSize( int *pWidth, int *pHeight )
{
  glfwGetWindowSize( pWindow_, pWidth, pHeight );
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

  if ( print_ )
  {
    std::cout << "Initialized GLFW Version: ";
    std::cout << glfwGetVersionString( ) << std::endl;
  }

  glfwInitialized_ = true;

  if ( opengl )
  {
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint( GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE );
#ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
#endif // __APPLE__
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
    if ( print_ )
    {
      std::cout << "Terminated GLFW" << std::endl;
    }
  }
}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////                                        ////////////////////
///////////////////////         END PRIVATE FUNCTIONS          ////////////////////
///////////////////////                                        ////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


} // namespace shg
