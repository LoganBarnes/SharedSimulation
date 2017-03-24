// OpenGLIOHandler.hpp
#pragma once


#include <memory>
#include "shared/core/IOHandler.hpp"

#include "shared/graphics/GraphicsForwardDeclarations.hpp"


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
                  bool   printInfo = true,
                  int    width     = 640,
                  int    height    = 480,
                  bool   resizable = true,
                  int    aaSamples = 8
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
  ///        images, or buffer swaps not handled by showWorld().
  ///
  ///////////////////////////////////////////////////////////////
  virtual
  void updateIO ( );


  ///////////////////////////////////////////////////////////////
  /// \brief waitForIO
  ///
  ///        Waits for keystrokes or mouse movement (or
  ///        alternate controllers) and updates output text,
  ///        images, or buffer swaps not handled by showWorld().
  ///
  ///////////////////////////////////////////////////////////////
  virtual
  void waitForIO ( );


  ///////////////////////////////////////////////////////////////
  /// \brief resize
  ///
  ///        Updates the camera aspect ratio and OpenGL viewport.
  ///
  ///////////////////////////////////////////////////////////////
  virtual
  void resize (
               const int width,
               const int height
               );


protected:

  std::unique_ptr< graphics::GlfwWrapper >     upGlfwWrapper_;
  std::unique_ptr< graphics::OpenGLWrapper >   upGLWrapper_;
  std::unique_ptr< graphics::Camera< float > > upCamera_;


private:

  virtual
  void _onRender ( const double alpha ) = 0;

};


} // namespace shared
