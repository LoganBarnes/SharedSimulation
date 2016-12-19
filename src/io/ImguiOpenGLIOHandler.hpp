#ifndef ImguiOpenGLIOHandler_hpp
#define ImguiOpenGLIOHandler_hpp


#include <memory>
#include "io/OpenGLIOHandler.hpp"


namespace graphics
{

class GlfwWrapper;
class OpenGLWrapper;
class Camera;

}


namespace shared
{


class World;
class SharedCallback;


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
                       bool   resizable = true
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
  void showWorld ( const double alpha );


protected:

private:

  virtual
  void onRender ( const double alpha ) = 0;

  virtual
  void onGuiRender( ) = 0;

};


} // namespace shared


#endif // ImguiOpenGLIOHandler_hpp
