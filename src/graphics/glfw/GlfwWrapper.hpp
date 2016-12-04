#ifndef GlfwWrapper_hpp
#define GlfwWrapper_hpp


#include <string>
#include "SharedSimulationConfig.hpp"


#ifdef USE_VULKAN

#include <vulkan/vulkan.h>

#endif


struct GLFWwindow;
typedef GLFWwindow GLFWwindow;


namespace graphics
{

class Callback;


///
/// \brief The GlfwWrapper class
///
class GlfwWrapper
{


public:

  ///////////////////////////////////////////////////////////////////////////////////
  //
  //  Initialization functions
  //
  ///////////////////////////////////////////////////////////////////////////////////

  GlfwWrapper( bool opengl = true );

  ~GlfwWrapper( );


  ///
  /// \brief createNewWindow
  /// \param title
  /// \param width
  /// \param height
  ///
  void createNewWindow (
                        const std::string &title,
                        const int          width,
                        const int          height,
                        const bool         resizable  = true,
                        const bool         initOpengl = true
                        );


  ///
  /// \brief getWindow
  /// \return
  ///
  GLFWwindow *getWindow ( );


  ///
  /// \brief getRequiredInstanceExtensions
  /// \param pCount
  /// \return
  ///
  static
  const char **getRequiredInstanceExtensions ( unsigned int *pCount );


#ifdef USE_VULKAN

  VkResult createWindowSurface (
                                VkInstance                   instance,
                                const VkAllocationCallbacks *pAllocator,
                                VkSurfaceKHR                *pSurface
                                );

#endif

#ifdef USE_GLAD

#endif


  ///////////////////////////////////////////////////////////////////////////////////
  //
  //  Render loop functions
  //
  ///////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////
  /// \brief pollEvents
  //////////////////////////////////////////////////
  static
  void pollEvents ( );


  //////////////////////////////////////////////////
  /// \brief swapBuffers
  //////////////////////////////////////////////////
  void swapBuffers ( );


  //////////////////////////////////////////////////
  /// \brief windowShouldClose
  /// \return
  //////////////////////////////////////////////////
  int windowShouldClose ( );


  //////////////////////////////////////////////////
  /// \brief setCallback
  /// \param pCallback
  //////////////////////////////////////////////////
  void setCallback( Callback *pCallback );


private:

  //////////////////////////////////////////////////
  /// \brief _initGlfw
  //////////////////////////////////////////////////
  bool _initGlfw ( bool opengl = true );


  //////////////////////////////////////////////////
  /// \brief _terminateGlfw
  //////////////////////////////////////////////////
  void _terminateGlfw ( );



  //
  // member vars
  //
  bool glfwInitialized_;

  GLFWwindow *pWindow_;


};


} // namespace graphics


#endif // GlfwWrapper_hpp
