#ifndef GlfwWrapper_hpp
#define GlfwWrapper_hpp


#include <string>
#include <vulkan/vulkan.h>


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

  GlfwWrapper( );

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
                        const int          height
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


  VkResult createWindowSurface (
                                VkInstance                   instance,
                                const VkAllocationCallbacks *pAllocator,
                                VkSurfaceKHR                *pSurface
                                );


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
  bool _initGlfw ( );


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
