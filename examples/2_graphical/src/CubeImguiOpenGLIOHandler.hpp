// CubeImguiOpenGLIoHandler.hpp
#pragma once

#include "shared/core/ImguiOpenGLIOHandler.hpp"
#include  "shared/graphics/GraphicsForwardDeclarations.hpp"


namespace example
{


class CubeWorld;


/////////////////////////////////////////////
/// \brief The CubeImguiOpenGLIOHandler class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class CubeImguiOpenGLIOHandler : public shs::ImguiOpenGLIOHandler
{

public:

  ///////////////////////////////////////////////////////////////
  /// \brief Renderer
  ///////////////////////////////////////////////////////////////
  CubeImguiOpenGLIOHandler( CubeWorld &cubeWorld );


  ///////////////////////////////////////////////////////////////
  /// \brief ~Renderer
  ///////////////////////////////////////////////////////////////
  virtual
  ~CubeImguiOpenGLIOHandler( ) final;


  void addRandomCube ( );

  void removeOldestCube ( );


private:

  virtual
  void _onRender ( const double alpha ) final;

  virtual
  void _onGuiRender ( ) final;

  CubeWorld &cubeWorld_;

  shg::StandardPipeline glIds_;

};


} // namespace example
