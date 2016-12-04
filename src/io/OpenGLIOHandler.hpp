#ifndef OpenGLIOHandler_hpp
#define OpenGLIOHandler_hpp


#include <memory>
#include "io/IOHandler.hpp"


namespace graphics
{

class GlfwWrapper;

}


namespace shared
{


class World;
class SharedCallback;


/////////////////////////////////////////////
/// \brief The OpenGLIOHandler class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class OpenGLIOHandler : public IOHandler
{

public:

  ///////////////////////////////////////////////////////////////
  /// \brief Renderer
  ///////////////////////////////////////////////////////////////
  OpenGLIOHandler(
                  World &world,
                  bool   printInfo = true
                  );


  ///////////////////////////////////////////////////////////////
  /// \brief ~Renderer
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

  std::unique_ptr< graphics::GlfwWrapper >  upGlfwWrapper_;
  std::unique_ptr< shared::SharedCallback > upCallback_;


private:

  virtual
  void onRender( const double alpha ) = 0;

};


} // namespace shared


#endif // OpenGLIOHandler_hpp
