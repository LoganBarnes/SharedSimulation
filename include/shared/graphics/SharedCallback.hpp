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
  SharedCallback( OpenGLIOHandler *pHandler = nullptr );

  virtual
  ~SharedCallback( );

  ////////////////////////////////////////////////////////
  /// \brief handleWindowSize
  /// \param pWindow
  /// \param width
  /// \param height
  ////////////////////////////////////////////////////////
  virtual
  void handleWindowSize (
                         GLFWwindow *pWindow,
                         int         width,
                         int         height
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


private:

  OpenGLIOHandler *pHandler_;

};


} // namespace shs
