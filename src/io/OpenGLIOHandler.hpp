#ifndef OpenGLIOHandler_hpp
#define OpenGLIOHandler_hpp


#include <memory>
#include "io/IOHandler.hpp"


namespace graphics
{

class GlfwWrapper;
class OpenGLWrapper;
class Camera;

}


namespace shared
{


class World;


/////////////////////////////////////////////
/// \brief The OpenGLIOHandler class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class OpenGLIOHandler : public IOHandler
{

public:

  ///////////////////////////////////////////////////////////////
  /// \brief OpenGLIOHandler
  ///////////////////////////////////////////////////////////////
  OpenGLIOHandler(
                  World &world,
                  bool  printInfo = true,
                  int   width     = 1080,
                  int   height    = 720,
                  bool  resizable = true
                  );


  ///////////////////////////////////////////////////////////////
  /// \brief ~OpenGLIOHandler
  ///////////////////////////////////////////////////////////////
  virtual
  ~OpenGLIOHandler( );


  ///////////////////////////////////////////////////////////////
  /// \brief showWorld
  /// \param alpha
  ///////////////////////////////////////////////////////////////
  virtual
  void showWorld ( const double alpha );


  ///////////////////////////////////////////////////////////////
  /// \brief updateIO
  ///
  ///        Checks for keystrokes or mouse movement (or
  ///        alternate controllers) and updates output text,
  ///        images, or buffer swaps not handled by showWorld()
  ///
  ///////////////////////////////////////////////////////////////
  virtual
  void updateIO ( );


protected:

  std::unique_ptr< graphics::GlfwWrapper >   upGlfwWrapper_;
  std::unique_ptr< graphics::OpenGLWrapper > upGLWrapper_;
  std::unique_ptr< graphics::Camera >        upCamera_;


private:

  virtual
  void _onRender ( const double alpha ) = 0;

};


} // namespace shared


#endif // OpenGLIOHandler_hpp
