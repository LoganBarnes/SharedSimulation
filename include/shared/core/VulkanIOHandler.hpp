// VulkanIOHandler.hpp
#pragma once


#include <memory>
#include "shared/core/IOHandler.hpp"


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


  ///////////////////////////////////////////////////////////////
  /// \brief onLoopExit
  ///
  ///        Allows for any synchronization or clean up that
  ///        needs to be completed before destruction
  ///
  ///////////////////////////////////////////////////////////////
  virtual
  void onLoopExit ( );


protected:

  std::unique_ptr< graphics::VulkanGlfwWrapper > upVulkanWrapper_;


private:

  virtual
  void onRender ( const double alpha );


};


} // namespace shared

