// ImguiOpenGLIOHandler.hpp
#pragma once


#include <memory>
#include "io/OpenGLIOHandler.hpp"



namespace shared
{


class World;
class ImguiCallback;

typedef ImguiCallback*ImguiCallback_t;


/////////////////////////////////////////////
/// \brief The ImguiOpenGLIOHandler class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class ImguiOpenGLIOHandler : public OpenGLIOHandler
{

public:

  ///////////////////////////////////////////////////////////////
  /// \brief ImguiOpenGLIOHandler
  ///////////////////////////////////////////////////////////////
  ImguiOpenGLIOHandler(
                       World &world,
                       bool   printInfo = true,
                       int    width     = 640,
                       int    height    = 480,
                       bool   resizable = true,
                       int    aaSamples = 8
                       );


  ///////////////////////////////////////////////////////////////
  /// \brief ~ImguiOpenGLIOHandler
  ///////////////////////////////////////////////////////////////
  virtual
  ~ImguiOpenGLIOHandler( );


  ///////////////////////////////////////////////////////////////
  /// \brief showWorld
  /// \param alpha
  ///////////////////////////////////////////////////////////////
  virtual
  void showWorld ( const double alpha = 1.0 );


protected:

  ImguiCallback_t imguiCallback_;


private:

  virtual
  void _onRender ( const double alpha );

  virtual
  void _onGuiRender ( );

};


} // namespace shared

