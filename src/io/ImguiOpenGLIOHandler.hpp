#ifndef ImguiOpenGLIOHandler_hpp
#define ImguiOpenGLIOHandler_hpp


#include <memory>
#include "io/OpenGLIOHandler.hpp"


namespace graphics
{

class GlfwWrapper;
class OpenGLWrapper;

template< typename T >
class Camera;

}


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
                       int    width     = 1080,
                       int    height    = 720,
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
  void _onRender ( const double alpha ) = 0;

  virtual
  void _onGuiRender ( ) = 0;

};


} // namespace shared


#endif // ImguiOpenGLIOHandler_hpp
