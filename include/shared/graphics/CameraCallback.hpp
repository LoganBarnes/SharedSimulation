// TCameraCallback.hpp
#pragma once

#include <shared/graphics/SharedCallback.hpp>
#include <shared/graphics/GraphicsForwardDeclarations.hpp>

namespace shs
{

template< typename T >
class TCameraCallback : public shs::SharedCallback
{
public:

  TCameraCallback( shg::TCameraMover< T > &cameraMover );

  virtual
  ~TCameraCallback( );


  virtual
  void handleKey (
                  GLFWwindow *pWindow,
                  int         key,
                  int         scancode,
                  int         action,
                  int         mods
                  );

  virtual
  void handleCursorPosition (
                             GLFWwindow *pWindow,
                             double      xpos,
                             double      ypos
                             );

  virtual
  void handleScroll (
                     GLFWwindow *pWindow,
                     double      xoffset,
                     double      yoffset
                     );


protected:

  shg::TCameraMover< T > &cameraMover_;

};

using CameraCallback  = TCameraCallback< float >;
using CameraCallbackD = TCameraCallback< double >;

} // namespace shs
