// SharedCallback.hpp
#pragma once


#include "shared/graphics/Callback.hpp"


namespace shs
{

class OpenGLIOHandler;


class SharedCallback : public shg::Callback
{

public:

  explicit
  SharedCallback( );

  virtual
  ~SharedCallback( ) = default;


  ////////////////////////////////////////////////////////
  /// \brief handleMouseButton
  /// \param pWindow
  /// \param button
  /// \param action
  /// \param mods
  ////////////////////////////////////////////////////////
  virtual
  void handleMouseButton (
                          GLFWwindow *pWindow,
                          int         button,
                          int         action,
                          int         mods
                          );

  ////////////////////////////////////////////////////////
  /// \brief handleKey
  /// \param pWindow
  /// \param key
  /// \param scancode
  /// \param action
  /// \param mods
  ////////////////////////////////////////////////////////
  virtual
  void  handleKey (
                   GLFWwindow *pWindow,
                   int         key,
                   int         scancode,
                   int         action,
                   int         mods
                   );


protected:

  bool shiftDown_;
  bool ctrlDown_;

  bool leftMouseDown_;
  bool rightMouseDown_;

  double prevX_;
  double prevY_;

};


} // namespace shs
