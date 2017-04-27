// CubeCallback.hpp
#pragma once

#include "shared/graphics/SharedCallback.hpp"

namespace example
{

class CubeImguiOpenGLIOHandler;


class CubeCallback : public shs::SharedCallback
{
public:

  CubeCallback( CubeImguiOpenGLIOHandler &handler );

  virtual
  ~CubeCallback( );


  virtual
  void handleWindowSize (
                         GLFWwindow *pWindow,
                         int         width,
                         int         height
                         );

  virtual
  void handleKey (
                  GLFWwindow *pWindow,
                  int         key,
                  int         scancode,
                  int         action,
                  int         mods
                  );


private:

  CubeImguiOpenGLIOHandler &handler_;

};


} //  namespace example
