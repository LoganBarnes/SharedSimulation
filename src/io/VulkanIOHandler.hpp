#ifndef VulkanIOHandler_hpp
#define VulkanIOHandler_hpp


#include <memory>
#include "io/IOHandler.hpp"


namespace graphics
{

class VulkanGlfwWrapper;

}


namespace shared
{


class World;
class SharedCallback;


/////////////////////////////////////////////
/// \brief The VulkanIOHandler class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class VulkanIOHandler : public IOHandler
{

public:

  ///////////////////////////////////////////////////////////////
  /// \brief Renderer
  ///////////////////////////////////////////////////////////////
  VulkanIOHandler(
                  World &world,
                  bool   printInfo = true
                  );


  ///////////////////////////////////////////////////////////////
  /// \brief ~Renderer
  ///////////////////////////////////////////////////////////////
  virtual
  ~VulkanIOHandler( );


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

  std::unique_ptr< graphics::VulkanGlfwWrapper > upVulkanWrapper_;
  std::unique_ptr< shared::SharedCallback  >     upCallback_;


private:

  virtual
  void onRender( const double alpha ) = 0;


};


} // namespace shared


#endif // VulkanIOHandler_hpp
