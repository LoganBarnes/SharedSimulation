#ifndef SharedCallback_hpp
#define SharedCallback_hpp


#include "graphics/glfw/Callback.hpp"


namespace shared
{


class SharedCallback : public graphics::Callback
{

public:


  SharedCallback( );

  ~SharedCallback( );


  ////////////////////////////////////////////////////////
  /// \brief handleKey
  /// \param pWindow
  /// \param key
  /// \param scancode
  /// \param action
  /// \param mods
  ////////////////////////////////////////////////////////
  virtual
  void  handleKey(
                  GLFWwindow *pWindow,
                  int         key,
                  int         scancode,
                  int         action,
                  int         mods
                  );


};


} // namespace shared


#endif // SharedCallback_hpp
