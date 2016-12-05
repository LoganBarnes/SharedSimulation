#ifndef CallbackSingleton_hpp
#define CallbackSingleton_hpp


#include <memory>


struct GLFWwindow;
typedef GLFWwindow GLFWwindow;


namespace graphics
{

class Callback;

///////////////////////////////////////////////////////////////
/// \brief The CallbackSingleton class
///
///        Singleton callback class for glfw input functions
///
/// \author LogiBear
///////////////////////////////////////////////////////////////
class CallbackSingleton
{

public:

  //
  // Singleton is accessed via getInstance()
  //
  static
  CallbackSingleton &getInstance ( );

  //
  // basic static callback functions
  //
  static
  void errorCallback (
                      int         error,
                      const char *description
                      );

  static
  void windowSizeCallback (
                           GLFWwindow *pWindow,
                           int         width,
                           int         height
                           );

  static
  void windowFocusCallback (
                            GLFWwindow *pWindow,
                            int         focus
                            );

  //
  // input static callback functions
  //
  static
  void mouseButtonCallback (
                            GLFWwindow *pWindow,
                            int         button,
                            int         action,
                            int         mods
                            );

  static
  void keyCallback (
                    GLFWwindow *pWindow,
                    int         key,
                    int         scancode,
                    int         action,
                    int         mods
                    );

  static
  void cursorPositionCallback (
                               GLFWwindow *pWindow,
                               double      xpos,
                               double      ypos
                               );

  static
  void scrollCallback (
                       GLFWwindow *pWindow,
                       double      xoffset,
                       double      yoffset
                       );

  //
  // the actual implementations of the callback methods
  //
  void defaultErrorCallback (
                             int         error,
                             const char *description
                             );

  void defaultWindowSizeCallback (
                                  GLFWwindow *pWindow,
                                  int         width,
                                  int         height
                                  );

  void defaultWindowFocusCallback (
                                   GLFWwindow *pWindow,
                                   int         focus
                                   );

  void defaultMouseButtonCallback (
                                   GLFWwindow *pWindow,
                                   int         button,
                                   int         action,
                                   int         mods
                                   );

  void defaultKeyCallback (
                           GLFWwindow *pWindow,
                           int         key,
                           int         scancode,
                           int         action,
                           int         mods
                           );

  void defaultCursorPositionCallback (
                                      GLFWwindow *pWindow,
                                      double      xpos,
                                      double      ypos
                                      );

  void defaultScrollCallback (
                              GLFWwindow *pWindow,
                              double      xoffset,
                              double      yoffset
                              );


  void setDefaultCallback (std::unique_ptr<Callback> upCallback );


private:

  //
  // private constructor necessary to allow only 1 instance
  //
  CallbackSingleton(  );

  virtual
  ~CallbackSingleton( ) noexcept;


  std::unique_ptr< Callback > upDefaultCallbacks_;

  //
  // Delete copy and move functions
  //
  CallbackSingleton( const CallbackSingleton& ) = delete;
  void operator=( const CallbackSingleton& )    = delete;

  CallbackSingleton( const CallbackSingleton&& ) = delete;
  void operator=( const CallbackSingleton&& )    = delete;

};


} // namespace graphics


#endif // CallbackSingleton_hpp
