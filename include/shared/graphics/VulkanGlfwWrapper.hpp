// VulkanGlfwWrapper.hpp
#pragma once


#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <vulkan/vulkan.h>

#include "VDeleter.hpp"


namespace graphics
{

class GlfwWrapper;
class Callback;


///
/// \brief The VulkanGlfwWrapper class
///
class VulkanGlfwWrapper
{

public:

  ///////////////////////////////////////////////////////////////////////////////////
  //
  //  Initialization functions
  //
  ///////////////////////////////////////////////////////////////////////////////////

  VulkanGlfwWrapper( );

  ~VulkanGlfwWrapper( );


  ///
  /// \brief createNewWindow
  /// \param title
  /// \param width
  /// \param height
  ///
  virtual
  void createNewWindow (
                        const std::string &title,
                        const int          width,
                        const int          height,
                        const bool         resizable = false
                        );


  ///
  /// \brief createRenderPass
  ///
  virtual
  void createRenderPass ( );


  ///
  /// \brief createGraphicsPipeline
  /// \param vertexFile
  /// \param fragmentFile
  ///
  virtual
  void createGraphicsPipeline (
                               const std::string &vertFile,
                               const std::string &fragFile
                               );
  ///
  /// \brief createFrameBuffer
  ///
  virtual
  void createFrameBuffer ( );


  ///
  /// \brief createCommandPool
  ///
  virtual
  void createCommandPool ( );


  ///
  /// \brief createCommandBuffers
  ///
  virtual
  void createCommandBuffers ( );


  ///
  /// \brief createSemaphores
  ///
  virtual
  void createSemaphores ( );



  ///////////////////////////////////////////////////////////////////////////////////
  //
  //  Render loop functions
  //
  ///////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////
  /// \brief checkInputEvents
  //////////////////////////////////////////////////
  virtual
  void checkInputEvents ( );


  //////////////////////////////////////////////////
  /// \brief drawFrame
  //////////////////////////////////////////////////
  virtual
  void drawFrame ( );


  //////////////////////////////////////////////////
  /// \brief checkWindowShouldClose
  /// \return
  //////////////////////////////////////////////////
  virtual
  bool checkWindowShouldClose ( );



  ///////////////////////////////////////////////////////////////////////////////////
  //
  //  Exit functions
  //
  ///////////////////////////////////////////////////////////////////////////////////

  virtual
  void syncDevice ( );



  ///////////////////////////////////////////////////////////////////////////////////
  //
  //  Util functions
  //
  ///////////////////////////////////////////////////////////////////////////////////

  virtual
  void setCallback ( Callback *pCallback );


private:

  //////////////////////////////////////////////////
  /// \brief _initVulkan
  /// \param title
  //////////////////////////////////////////////////
  void _initVulkan (
                    const std::string &title,
                    const int          width,
                    const int          height
                    );

  //////////////////////////////////////////////////
  /// \brief _createVulkanInstance
  /// \param title
  //////////////////////////////////////////////////
  virtual
  void _createVulkanInstance ( const std::string &title );

  //////////////////////////////////////////////////
  /// \brief _setUpVulkanDebugCallback
  //////////////////////////////////////////////////
  virtual
  void _setUpVulkanDebugCallback ( );

  //////////////////////////////////////////////////
  /// \brief _createVulkanSurface
  //////////////////////////////////////////////////
  virtual
  void _createVulkanSurface ( );

  //////////////////////////////////////////////////
  /// \brief _setUpVulkanPhysicalDevice
  //////////////////////////////////////////////////
  virtual
  void _setUpVulkanPhysicalDevice ( );

  //////////////////////////////////////////////////
  /// \brief _createVulkanLogicalDevice
  //////////////////////////////////////////////////
  virtual
  void _createVulkanLogicalDevice ( );

  //////////////////////////////////////////////////
  /// \brief _createSwapChain
  /// \param width
  /// \param height
  //////////////////////////////////////////////////
  virtual
  void _createSwapChain (
                         const int width,
                         const int height
                         );

  //////////////////////////////////////////////////
  /// \brief _createImageViews
  //////////////////////////////////////////////////
  virtual
  void _createImageViews ( );


  //
  // member vars
  //
  std::unique_ptr< graphics::GlfwWrapper > upGlfw_;


protected:

  VDeleter< VkInstance > instance_ {
    vkDestroyInstance
  };
  VDeleter< VkDebugReportCallbackEXT > callback_ {
    instance_, DestroyDebugReportCallbackEXT
  };
  VDeleter< VkSurfaceKHR > surface_ {
    instance_, vkDestroySurfaceKHR
  };

  VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
  VDeleter< VkDevice > device_ {
    vkDestroyDevice
  };

  VkQueue graphicsQueue_;
  VkQueue presentQueue_;

  VDeleter< VkSwapchainKHR > swapChain_ {
    device_, vkDestroySwapchainKHR
  };
  std::vector< VkImage > swapChainImages_;
  VkFormat swapChainImageFormat_;
  VkExtent2D swapChainExtent_;
  std::vector< VDeleter< VkImageView > > swapChainImageViews_;
  std::vector< VDeleter< VkFramebuffer > > swapChainFramebuffers_;

  VDeleter< VkRenderPass > renderPass_ {
    device_, vkDestroyRenderPass
  };
  VDeleter< VkPipelineLayout > pipelineLayout_ {
    device_, vkDestroyPipelineLayout
  };
  VDeleter< VkPipeline > graphicsPipeline_ {
    device_, vkDestroyPipeline
  };

  VDeleter< VkCommandPool > commandPool_ {
    device_, vkDestroyCommandPool
  };
  std::vector< VkCommandBuffer > commandBuffers_;

  VDeleter< VkSemaphore > imageAvailableSemaphore_ {
    device_, vkDestroySemaphore
  };
  VDeleter< VkSemaphore > renderFinishedSemaphore_ {
    device_, vkDestroySemaphore
  };

};


} // namespace graphics
