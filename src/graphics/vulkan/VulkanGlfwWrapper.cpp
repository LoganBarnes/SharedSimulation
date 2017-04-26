#include "shared/graphics/VulkanGlfwWrapper.hpp"

#include "shared/graphics/GlfwWrapper.hpp"
#include "shared/graphics/Callback.hpp"

#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include <cstring>


namespace graphics
{




///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////                                        ////////////////////
///////////////////////             START HELPERS              ////////////////////
///////////////////////                                        ////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

//
// Vulkan helper functions and variables for initialization
//
namespace
{


std::vector< char >
readFile( const std::string &filename )
{

  std::ifstream file( filename, std::ios::ate | std::ios::binary );

  if ( !file.is_open( ) )
  {

    throw std::runtime_error( "Failed to open file" );

  }

  size_t fileSize = static_cast< size_t >( file.tellg( ) );
  std::vector< char > buffer( fileSize );

  file.seekg( 0 );

  file.read(
            buffer.data( ),
            static_cast< std::streamsize >( fileSize )
            );

  file.close( );

  return buffer;

}


//
// requested error checking layers
//
const std::vector< const char* > validationLayers =
{

  "VK_LAYER_LUNARG_standard_validation"

};

//
// requested GPU extensions
//
const std::vector< const char* > deviceExtensions =
{

  VK_KHR_SWAPCHAIN_EXTENSION_NAME

};

//
// only enable validation for debug builds
//
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif





///
/// \brief checkValidationLayerSupport
///
///        Check for the availability of the requested
///        Vulkan validation layers
///
/// \return
///
bool
checkValidationLayerSupport( )
{

  uint32_t layerCount;

  vkEnumerateInstanceLayerProperties( &layerCount, nullptr );

  std::vector< VkLayerProperties > availableLayers( layerCount );
  vkEnumerateInstanceLayerProperties( &layerCount, availableLayers.data( ) );

  for ( const char *layerName : validationLayers )
  {

    bool layerFound = false;

    for ( const auto & layerProperties : availableLayers )
    {

      if ( std::strcmp( layerName, layerProperties.layerName ) == 0 )
      {

        layerFound = true;
        break;

      }

    }

    if ( !layerFound )
    {

      return false;

    }

  }

  return true;

} // checkValidationLayerSupport



bool
checkDeviceExtensionSupport( VkPhysicalDevice device )
{

  uint32_t extensionCount;

  vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, nullptr );

  std::vector< VkExtensionProperties > availableExtensions( extensionCount );
  vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, availableExtensions.data( ) );

  std::set< std::string > requiredExtensions( deviceExtensions.begin(), deviceExtensions.end( ) );

  for ( const auto & extension : availableExtensions )
  {

    requiredExtensions.erase( extension.extensionName );

  }

  return requiredExtensions.empty( );

}



///
/// \brief getRequiredExtensions
///
///        Get the required list of extensions based on
///        whether validation layers are enabled or not
///
/// \return
///
std::vector< const char* >
getRequiredExtensions( )
{

  std::vector< const char* > extensions;

  unsigned int glfwExtensionCount = 0;
  const char **glfwExtensions( GlfwWrapper::getRequiredInstanceExtensions( &glfwExtensionCount ) );

  for ( unsigned int i = 0; i < glfwExtensionCount; i++ )
  {

    extensions.push_back( glfwExtensions[ i ] );

  }

  if ( enableValidationLayers )
  {

    extensions.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );

  }

  return extensions;

} // getRequiredExtensions



///
/// \brief debugCallback
/// \param flags
/// \param objType
/// \param obj
/// \param location
/// \param code
/// \param layerPrefix
/// \param msg
/// \param userData
///
VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(
              VkDebugReportFlagsEXT,
              VkDebugReportObjectTypeEXT,
              uint64_t,
              size_t,
              int32_t,
              const char*,
              const char *msg,
              void*
              )
{

  std::cerr << "Vulkan validation layer: " << msg << std::endl;

  return VK_FALSE;

}



///
/// \brief The QueueFamilyIndices struct
///
struct QueueFamilyIndices
{

  int graphicsFamily_ = -1;
  int presentFamily_  = -1;

  bool
  isComplete( )
  {

    return graphicsFamily_ >= 0 && presentFamily_ >= 0;

  }

};



///
/// \brief The SwapChainSupportDetails struct
///
struct SwapChainSupportDetails
{

  VkSurfaceCapabilitiesKHR          capabilities;
  std::vector< VkSurfaceFormatKHR > formats;
  std::vector< VkPresentModeKHR >   presentModes;

};



///
/// \brief querySwapChainSupport
/// \param device
/// \return
///
SwapChainSupportDetails
querySwapChainSupport(
                      VkPhysicalDevice device,
                      VkSurfaceKHR     surface
                      )
{

  SwapChainSupportDetails details;

  //
  // these functions all take the device and surface into account when
  // determining capabilites
  //
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, surface, &details.capabilities );

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &formatCount, nullptr );

  if ( formatCount != 0 )
  {

    details.formats.resize( formatCount );
    vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &formatCount, details.formats.data( ) );

  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &presentModeCount, nullptr );

  if ( presentModeCount != 0 )
  {

    details.presentModes.resize( presentModeCount );
    vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &presentModeCount, details.presentModes.data( ) );

  }

  return details;

}



///
/// \brief findQueueFamilies
/// \param device
/// \return
///
QueueFamilyIndices
findQueueFamilies(
                  VkPhysicalDevice device,
                  VkSurfaceKHR     surface
                  )
{

  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;

  vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, nullptr );

  std::vector< VkQueueFamilyProperties > queueFamilies( queueFamilyCount );
  vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies.data( ) );


  int i = 0;

  for ( const auto & queueFamily : queueFamilies )
  {

    if ( queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT )
    {

      indices.graphicsFamily_ = i;

    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(
                                         device,
                                         static_cast< uint32_t >( i ),
                                         surface,
                                         &presentSupport
                                         );

    if ( queueFamily.queueCount > 0 && presentSupport )
    {

      indices.presentFamily_ = i;

    }

    if ( indices.isComplete( ) )
    {

      break;

    }

    ++i;

  }

  return indices;

} // findQueueFamilies



///
/// \brief isDeviceSuitable
/// \param device
/// \return
///
bool
isDeviceSuitable(
                 VkPhysicalDevice device,
                 VkSurfaceKHR     surface
                 )
{

  QueueFamilyIndices indices = findQueueFamilies( device, surface );

  bool extensionsSupported = checkDeviceExtensionSupport( device );

  bool swapChainAdequate = false;
  if ( extensionsSupported )
  {

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport( device, surface );

    swapChainAdequate = !swapChainSupport.formats.empty( ) && ! swapChainSupport.presentModes.empty( );

  }

  return indices.isComplete( ) && swapChainAdequate;

}



///
/// \brief chooseSwapSurfaceFormat
/// \param availableFormats
/// \return
///
VkSurfaceFormatKHR
chooseSwapSurfaceFormat( const std::vector< VkSurfaceFormatKHR > &availableFormats )
{

  //
  // each VkSurfaceFormatKHR struct contains a format and colorSpace member.
  // format specifies color channels and types.
  // colorSpace indicates if the SRGB color space is suported (more accurate perceived colors).
  //

  //
  // surface has no preferred format
  //
  if ( availableFormats.size( ) == 1 && availableFormats[ 0 ].format == VK_FORMAT_UNDEFINED )
  {

      return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

  }

  //
  // look for preferred combination if we can't choose our own
  //
  for ( const auto &availableFormat : availableFormats )
  {

    if ( availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM
         && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
    {

      return availableFormat;

    }

  }

  //
  // all attempts to use our format failed. Just use first available option
  //
  return availableFormats[ 0 ];

}



///
/// \brief chooseSwapPresentMode
/// \param availablePresentModes
/// \return
///
VkPresentModeKHR
chooseSwapPresentMode( const std::vector< VkPresentModeKHR > &availablePresentModes )
{

  for ( const auto &availablePresentMode : availablePresentModes )
  {

    //
    // triple buffering to avoid tearing with low latency
    //
    if ( availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR )
    {

      return availablePresentMode;

    }

  }

  //
  // guaranteed to be available
  //
  return VK_PRESENT_MODE_FIFO_KHR;

}



VkExtent2D
chooseSwapExtent(
                 const VkSurfaceCapabilitiesKHR &capabilites,
                 const int                       width,
                 const int                       height
                 )
{

  //
  // window resolution matches current extent
  //
  if ( capabilites.currentExtent.width != std::numeric_limits< uint32_t >::max( ) )
  {

    return capabilites.currentExtent;

  }

  //
  // choose resolution that best matches window
  //
  VkExtent2D actualExtent = { static_cast< uint32_t >( width ),
                              static_cast< uint32_t >( height ) };

  //
  // clamp width and height to be within allowed values
  //
  actualExtent.width = std::max( capabilites.minImageExtent.width,
                                 std::min( capabilites.maxImageExtent.width,
                                           actualExtent.width ) );

  actualExtent.height = std::max( capabilites.minImageExtent.height,
                                  std::min( capabilites.maxImageExtent.height,
                                            actualExtent.height ) );

  return actualExtent;

}



void
createShaderModule(
                   VkDevice                    device,
                   const std::vector< char >  &code,
                   VDeleter< VkShaderModule > &shaderModule
                   )
{

  VkShaderModuleCreateInfo createInfo = {};
  createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size( );
  createInfo.pCode    = reinterpret_cast< const uint32_t* >( code.data( ) );

  if ( vkCreateShaderModule( device, &createInfo, nullptr, shaderModule.replace( ) ) != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create shader module" );

  }

}



} // namespace




///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////                                        ////////////////////
///////////////////////              END HELPERS               ////////////////////
///////////////////////                                        ////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////
//
//  Initialization functions
//
///////////////////////////////////////////////////////////////////////////////////



VulkanGlfwWrapper::VulkanGlfwWrapper(  )
  : upGlfw_( new graphics::GlfwWrapper( true, false ) ) // no openGL
{}



///
/// \brief VulkanGlfwWrapper::~VulkanGlfwWrapper
///
VulkanGlfwWrapper::~VulkanGlfwWrapper( )
{

  if ( commandBuffers_.size( ) > 0 )
  {

    vkFreeCommandBuffers(
                         device_,
                         commandPool_,
                         commandBuffers_.size( ),
                         commandBuffers_.data( )
                         );

  }

}



void
VulkanGlfwWrapper::createNewWindow(
                                   const std::string &title,
                                   const int          width,
                                   const int          height,
                                   const bool         resizable
                                   )
{

  upGlfw_->createNewWindow( title, width, height, resizable, false ); // no openGL
  _initVulkan( title, width, height );

}



///
/// \brief VulkanGlfwWrapper::createRenderPass
///
void
VulkanGlfwWrapper::createRenderPass()
{

  VkAttachmentDescription colorAttachment = {};
  colorAttachment.format  = swapChainImageFormat_;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef = {};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  //
  // potential post processing
  //
  VkSubpassDescription subPass = {};
  subPass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subPass.colorAttachmentCount = 1;
  subPass.pColorAttachments    = &colorAttachmentRef;

  VkSubpassDependency dependency = {};
  dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass    = 0;
  dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT
                             | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  //
  // actual render pass creation
  //
  VkRenderPassCreateInfo renderPassInfo = {};
  renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments    = &colorAttachment;
  renderPassInfo.subpassCount    = 1;
  renderPassInfo.pSubpasses      = &subPass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies   = &dependency;

  if ( vkCreateRenderPass( device_, &renderPassInfo, nullptr, renderPass_.replace( ) ) != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create render pass" );

  }

}


///
/// \brief VulkanGlfwWrapper::createGraphicsPipeline
/// \param vertFile
/// \param fragFile
///
void
VulkanGlfwWrapper::createGraphicsPipeline(
                                          const std::string &vertFile,
                                          const std::string &fragFile
                                          )
{

  auto vertShaderCode = readFile( vertFile );
  auto fragShaderCode = readFile( fragFile );

  VDeleter< VkShaderModule > vertShaderModule {
    device_, vkDestroyShaderModule
  };

  VDeleter< VkShaderModule > fragShaderModule {
    device_, vkDestroyShaderModule
  };

  createShaderModule( device_, vertShaderCode, vertShaderModule );
  createShaderModule( device_, fragShaderCode, fragShaderModule );

  VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
  vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName  = "main";
  //
  // used to specify shader constants if necessary
  //
  vertShaderStageInfo.pSpecializationInfo = nullptr; // nullptr by default


  VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
  fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName  = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo,
                                                     fragShaderStageInfo };

  //
  // Vertex input
  //
  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount   = 0;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;

  //
  // Input assembly
  //
  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
  inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  //
  // Viewports and scissors
  //
  VkViewport viewport = {};
  viewport.x        = 0.0f;
  viewport.y        = 0.0f;
  viewport.width    = static_cast< float >( swapChainExtent_.width );
  viewport.height   = static_cast< float >( swapChainExtent_.height );
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  // scissor discards portions of the viewport
  // using full viewport here
  VkRect2D scissor = {};
  scissor.offset   = {0, 0};
  scissor.extent   = swapChainExtent_;

  // combine viewport and scissor structs into viewport state
  VkPipelineViewportStateCreateInfo viewportState = {};
  viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports    = &viewport;
  viewportState.scissorCount  = 1;
  viewportState.pScissors     = &scissor;

  //
  // Rasterizer
  //
  VkPipelineRasterizationStateCreateInfo rasterizer = {};
  rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable        = VK_FALSE; // discard things outside near/far planes
  rasterizer.rasterizerDiscardEnable = VK_FALSE; // VK_TRUE dissables rasterization
  rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth               = 1.0f;
  rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable         = VK_FALSE; // useful for shadow mapping?
  rasterizer.depthBiasConstantFactor = 0.0f; // Optional
  rasterizer.depthBiasClamp          = 0.0f; // Optional
  rasterizer.depthBiasSlopeFactor    = 0.0f; // Optional

  //
  // Multisampling
  //
  VkPipelineMultisampleStateCreateInfo multisampling = {};
  multisampling.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable   = VK_FALSE;
  multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading      = 1.0f; // Optional
  multisampling.pSampleMask           = nullptr; /// Optional
  multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
  multisampling.alphaToOneEnable      = VK_FALSE; // Optional

  //
  // Depth and stencil testing
  //
  // not used yet

  //
  // Color blending
  //
  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable    = VK_FALSE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD; // Optional
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD; // Optional

  // typical alpha blending
//  colorBlendAttachment.blendEnable = VK_TRUE;
//  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
//  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
//  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo colorBlending = {};
  colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable     = VK_FALSE;
  colorBlending.logicOp           = VK_LOGIC_OP_COPY; // Optional
  colorBlending.attachmentCount   = 1;
  colorBlending.pAttachments      = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f; // Optional
  colorBlending.blendConstants[1] = 0.0f; // Optional
  colorBlending.blendConstants[2] = 0.0f; // Optional
  colorBlending.blendConstants[3] = 0.0f; // Optional

  //
  // Dynamic state - things to be ignored and specified at drawing time
  //
  // not actually used yet
  //
  VkDynamicState dynamicStates[] = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_LINE_WIDTH
  };

  VkPipelineDynamicStateCreateInfo dynamicState = {};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = 2;
  dynamicState.pDynamicStates = dynamicStates;

  //
  // Pipeline layout - shader uniforms
  //
  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
  pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount         = 0; // Optional
  pipelineLayoutInfo.pSetLayouts            = nullptr; // Optional
  pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
  pipelineLayoutInfo.pPushConstantRanges    = 0; // Optiona

  if ( vkCreatePipelineLayout( device_, &pipelineLayoutInfo, nullptr, pipelineLayout_.replace( ) )
       != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create the pipeline layout" );

  }

  //
  // actual pipeline creation
  //
  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  pipelineInfo.sType      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages    = shaderStages;

  pipelineInfo.pVertexInputState   = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState      = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState   = &multisampling;
  pipelineInfo.pDepthStencilState  = nullptr; // Optional
  pipelineInfo.pColorBlendState    = &colorBlending;
  pipelineInfo.pDynamicState       = nullptr; // Optional

  pipelineInfo.layout             = pipelineLayout_;
  pipelineInfo.renderPass         = renderPass_;
  pipelineInfo.subpass            = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex  = -1; // Optional

  if ( vkCreateGraphicsPipelines( device_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, graphicsPipeline_.replace( ) )
       != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create graphics pipeline" );

  }

}



///
/// \brief VulkanGlfwWrapper::createFrameBuffer
///
void
VulkanGlfwWrapper::createFrameBuffer( )
{

  swapChainFramebuffers_.resize( swapChainImageViews_.size( ),
                                 VDeleter< VkFramebuffer >{ device_, vkDestroyFramebuffer } );

  size_t size = swapChainFramebuffers_.size( );

  for ( size_t i = 0; i < swapChainImageViews_.size( ); ++i )
  {

    VkImageView attachments[] = { swapChainImageViews_[ i ] };

    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass      = renderPass_;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments    = attachments;
    framebufferInfo.width           = swapChainExtent_.width;
    framebufferInfo.height          = swapChainExtent_.height;
    framebufferInfo.layers          = 1;

    if ( vkCreateFramebuffer( device_, &framebufferInfo, nullptr, swapChainFramebuffers_[ i ].replace( ) )
         != VK_SUCCESS )
    {

      throw std::runtime_error( "Failed to create framebuffer" );

    }

  }

}


///
/// \brief VulkanGlfwWrapper::createCommandPool
///
void
VulkanGlfwWrapper::createCommandPool( )
{

  QueueFamilyIndices queueFamilyIndices = findQueueFamilies( physicalDevice_, surface_ );

  VkCommandPoolCreateInfo poolInfo = {};
  poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = static_cast< uint32_t >( queueFamilyIndices.graphicsFamily_ );
  poolInfo.flags            = 0; // Optional

  if ( vkCreateCommandPool( device_, &poolInfo, nullptr, commandPool_.replace( ) ) != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create command pool" );

  }

}



///
/// \brief VulkanGlfwWrapper::createCommandBuffers
///
void
VulkanGlfwWrapper::createCommandBuffers( )
{

  commandBuffers_.resize( swapChainFramebuffers_.size( ) );

  VkCommandBufferAllocateInfo allocInfo = {};
  allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool        = commandPool_;
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = static_cast< uint32_t >( commandBuffers_.size( ) );

  if ( vkAllocateCommandBuffers( device_, &allocInfo, commandBuffers_.data( ) ) != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to allocate command buffers" );

  }

  size_t size = commandBuffers_.size( );

  for ( size_t i = 0; i < size; ++i )
  {

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr; // Optional

    vkBeginCommandBuffer( commandBuffers_[ i ], &beginInfo );

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass  = renderPass_;
    renderPassInfo.framebuffer = swapChainFramebuffers_[ i ];

    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChainExtent_;

    VkClearValue clearColor        = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues    = &clearColor;

    // vkCmd functions record commands to the command buffer
    vkCmdBeginRenderPass( commandBuffers_[ i ], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );

    // bind graphics pipeline
    vkCmdBindPipeline( commandBuffers_[ i ], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_ );

    // vertexCount, instanceCount, firstVertex, firstInstance
    vkCmdDraw( commandBuffers_[ i ], 4, 1, 0, 0 );

    // last command to finish render pass
    vkCmdEndRenderPass( commandBuffers_[ i ] );

    if ( vkEndCommandBuffer( commandBuffers_[ i ] ) != VK_SUCCESS )
    {

      throw std::runtime_error( "Failed to record command buffer" );

    }

  }

}



///
/// \brief VulkanGlfwWrapper::createSemaphores
///
void
VulkanGlfwWrapper::createSemaphores( )
{

  VkSemaphoreCreateInfo semaphoreInfo = {};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if ( vkCreateSemaphore( device_, &semaphoreInfo, nullptr, imageAvailableSemaphore_.replace( ) )
       != VK_SUCCESS
       || vkCreateSemaphore( device_, &semaphoreInfo, nullptr, renderFinishedSemaphore_.replace( ) )
       != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create semaphores" );

  }

}



///////////////////////////////////////////////////////////////////////////////////
//
//  Render loop functions
//
///////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////
/// \brief VulkanGlfwWrapper::checkInputEvents
//////////////////////////////////////////////////
void
VulkanGlfwWrapper::checkInputEvents ( )
{
  GlfwWrapper::pollEvents( );
}


///
/// \brief VulkanGlfwWrapper::drawFrame
///
void
VulkanGlfwWrapper::drawFrame( )
{

  uint32_t imageIndex;

  vkAcquireNextImageKHR(
                        device_,
                        swapChain_,
                        std::numeric_limits< uint64_t >::max( ), // disable timeout
                        imageAvailableSemaphore_,
                        VK_NULL_HANDLE,
                        &imageIndex
                        );

  VkSubmitInfo submitInfo = {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[]      = { imageAvailableSemaphore_ };
  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submitInfo.waitSemaphoreCount     = 1;
  submitInfo.pWaitSemaphores        = waitSemaphores;
  submitInfo.pWaitDstStageMask      = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers    = &commandBuffers_[ imageIndex ];

  VkSemaphore signalSemaphores[]  = { renderFinishedSemaphore_ };
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores    = signalSemaphores;

  if ( vkQueueSubmit( graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE ) != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to submit draw command buffer" );

  }

  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores    = signalSemaphores;

  VkSwapchainKHR swapChains[] = { swapChain_ };
  presentInfo.swapchainCount  = 1;
  presentInfo.pSwapchains     = swapChains;
  presentInfo.pImageIndices   = &imageIndex;
  presentInfo.pResults        = nullptr; // Optional: for VkResults with multiple swapChains

  vkQueuePresentKHR( presentQueue_, &presentInfo );

}



///
/// \brief VulkanGlfwWrapper::checkWindowShouldClose
///
bool
VulkanGlfwWrapper::checkWindowShouldClose( )
{

  return upGlfw_->windowShouldClose( ) != 0;

}



///
/// \brief VulkanGlfwWrapper::syncDevice
///
void
VulkanGlfwWrapper::syncDevice( )
{

  vkDeviceWaitIdle( device_ );

}



///
/// \brief VulkanGlfwWrapper::setCallback
/// \param pCallback
///
void
VulkanGlfwWrapper::setCallback( std::unique_ptr< Callback > upCallback )
{
  upGlfw_->setCallback( std::move( upCallback ) );
}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////                                        ////////////////////
///////////////////////        START PRIVATE FUNCTIONS         ////////////////////
///////////////////////                                        ////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


///
/// \brief VulkanGlfwWrapper::_initVulkan
///
void
VulkanGlfwWrapper::_initVulkan(
                               const std::string &title,
                               const int          width,
                               const int          height
                               )
{

  //
  // get number of available extentions
  //
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, nullptr );

  //
  // get names of available extentions
  //
  std::vector< VkExtensionProperties > extensions( extensionCount );
  vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, extensions.data( ) );

  std::cout << "Available Vulkan extensions:" << std::endl;

  //
  // print available extentions
  //
  for ( const auto & extension : extensions )
  {

    std::cout << "\t" << extension.extensionName << std::endl;

  }

  //
  // create instance
  //
  _createVulkanInstance( title );

  //
  // setup any debugging
  //
  _setUpVulkanDebugCallback( );

  //
  // surface for rendering?
  //
  _createVulkanSurface( );

  //
  // select GPU(s)
  //
  _setUpVulkanPhysicalDevice( );

  //
  // create device to interface with GPU
  //
  _createVulkanLogicalDevice( );

  //
  //
  //
  _createSwapChain( width, height );

  //
  // Literally views onto images.
  // How to access an image and what part to access
  //
  _createImageViews( );


} // VulkanGlfwWrapper::_initVulkan



/////////////////////////////////////////////////////////////////
/// \brief VulkanGlfwWrapper::_createVulkanInstance
/// \param title
///
/// \author Logan Barnes
/////////////////////////////////////////////////////////////////
void
VulkanGlfwWrapper::_createVulkanInstance( const std::string &title )
{

  //
  // check for existing validation layers if requested
  //
  if ( enableValidationLayers && !checkValidationLayerSupport( ) )
  {

    throw std::runtime_error( "Vulkan validation layers requested but not available" );

  }

  //
  // optional but useful struct
  //
  VkApplicationInfo appInfo = {};

  appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName   = title.c_str( );
  appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
  appInfo.pEngineName        = "No Engine";
  appInfo.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 );
  appInfo.apiVersion         = VK_API_VERSION_1_0;

  //
  // creation info
  //
  VkInstanceCreateInfo createInfo = {};

  createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto extensions                    = getRequiredExtensions( );
  createInfo.enabledExtensionCount   = extensions.size( );
  createInfo.ppEnabledExtensionNames = extensions.data( );

  //
  // specify level of validation
  //
  if ( enableValidationLayers )
  {

    createInfo.enabledLayerCount   = validationLayers.size( );
    createInfo.ppEnabledLayerNames = validationLayers.data( );

  }
  else
  {

    createInfo.enabledLayerCount = 0;

  }

  //
  // try to create the Vulkan instance
  //
  if ( vkCreateInstance( &createInfo, nullptr, instance_.replace( ) ) != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create Vulkan instance" );

  }

} // VulkanGlfwWrapper::_createVulkanInstance



/////////////////////////////////////////////////////////////////
/// \brief VulkanGlfwWrapper::_setUpVulkanDebugCallback
///
/// \author Logan Barnes
/////////////////////////////////////////////////////////////////
void
VulkanGlfwWrapper::_setUpVulkanDebugCallback( )
{

  if ( !enableValidationLayers )
  {

    return;

  }

  VkDebugReportCallbackCreateInfoEXT createInfo = {};
  createInfo.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  createInfo.flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
  createInfo.pfnCallback = debugCallback;

  if ( CreateDebugReportCallbackEXT(
                                    instance_,
                                    &createInfo,
                                    nullptr,
                                    callback_.replace( )
                                    )
      != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to set up Vulkan debug callback" );

  }

} // VulkanGlfwWrapper::_setUpVulkanDebugCallback



/////////////////////////////////////////////////////////////////
/// \brief VulkanGlfwWrapper::_createVulkanSurface
///
/// \author Logan Barnes
/////////////////////////////////////////////////////////////////
void
VulkanGlfwWrapper::_createVulkanSurface( )
{
  if ( upGlfw_->createWindowSurface( instance_, nullptr, surface_.replace( ) ) != VK_SUCCESS )
  {
    throw std::runtime_error( "Failed to create window surface" );
  }
}



/////////////////////////////////////////////////////////////////
/// \brief VulkanGlfwWrapper::_setUpVulkanPhysicalDevice
///
/// \author Logan Barnes
/////////////////////////////////////////////////////////////////
void
VulkanGlfwWrapper::_setUpVulkanPhysicalDevice( )
{

  uint32_t deviceCount = 0;

  vkEnumeratePhysicalDevices( instance_, &deviceCount, nullptr );

  if ( deviceCount == 0 )
  {

    throw std::runtime_error( "Failed to find GPUs with Vulkan support" );

  }

  std::vector< VkPhysicalDevice > devices( deviceCount );
  vkEnumeratePhysicalDevices( instance_, &deviceCount, devices.data( ) );

  for ( const auto & device : devices )
  {

    ///\todo add device checking, scoring, and selection options.
    ///      Using first vulkan capable device for now
    if ( isDeviceSuitable( device, surface_ ) )
    {

      physicalDevice_ = device;
      break;

    }

  }

  if ( physicalDevice_ == VK_NULL_HANDLE )
  {

    throw std::runtime_error( "Failed to find a suitable GPU" );

  }

} // VulkanGlfwWrapper::_setUpVulkanPhysicalDevice



/////////////////////////////////////////////////////////////////
/// \brief VulkanGlfwWrapper::_createVulkanLogicalDevice
///
/// \author Logan Barnes
/////////////////////////////////////////////////////////////////
void
VulkanGlfwWrapper::_createVulkanLogicalDevice( )
{

  QueueFamilyIndices indices = findQueueFamilies( physicalDevice_, surface_ );

  std::vector< VkDeviceQueueCreateInfo > queueCreateInfos;
  std::set< int > uniqueQueueFamilies = { indices.graphicsFamily_, indices.presentFamily_ };

  float queuePriority = 1.0f;

  for ( int queueFamily : uniqueQueueFamilies )
  {

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = static_cast< uint32_t >( queueFamily );
    queueCreateInfo.queueCount       = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back( queueCreateInfo );

  }

  VkPhysicalDeviceFeatures deviceFeatures = {};

  VkDeviceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.pQueueCreateInfos    = queueCreateInfos.data( );
  createInfo.queueCreateInfoCount = static_cast< uint32_t >( queueCreateInfos.size( ) );

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount   = deviceExtensions.size( );
  createInfo.ppEnabledExtensionNames = deviceExtensions.data( );

  if ( enableValidationLayers )
  {

    createInfo.enabledLayerCount   = validationLayers.size( );
    createInfo.ppEnabledLayerNames = validationLayers.data( );

  }
  else
  {

    createInfo.enabledLayerCount = 0;

  }

  if ( vkCreateDevice( physicalDevice_, &createInfo, nullptr, device_.replace( ) ) != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create logical device" );

  }

  vkGetDeviceQueue( device_, static_cast< uint32_t >( indices.graphicsFamily_ ), 0, &graphicsQueue_ );
  vkGetDeviceQueue( device_, static_cast< uint32_t >( indices.presentFamily_ ),  0, &presentQueue_  );

} // VulkanGlfwWrapper::_createVulkanLogicalDevice



///
/// \brief VulkanGlfwWrapper::_createSwapChain
/// \param width
/// \param height
///
void
VulkanGlfwWrapper::_createSwapChain(
                                    const int width,
                                    const int height
                                    )
{

  SwapChainSupportDetails swapChainSupport = querySwapChainSupport( physicalDevice_, surface_ );

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat( swapChainSupport.formats      );
  VkPresentModeKHR   presentMode   = chooseSwapPresentMode  ( swapChainSupport.presentModes );
  VkExtent2D         extent        = chooseSwapExtent       ( swapChainSupport.capabilities, width, height );

  //
  // one more image than normally required to allow for triple buffering
  //
  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

  if ( swapChainSupport.capabilities.maxImageCount > 0
       && imageCount > swapChainSupport.capabilities.maxImageCount )
  {

    imageCount = swapChainSupport.capabilities.maxImageCount;

  }

  //
  // swap chain structure
  //
  VkSwapchainCreateInfoKHR createInfo = {};
  createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface          = surface_;

  createInfo.minImageCount    = imageCount;
  createInfo.imageFormat      = surfaceFormat.format;
  createInfo.imageColorSpace  = surfaceFormat.colorSpace;
  createInfo.imageExtent      = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = findQueueFamilies( physicalDevice_, surface_ );

  uint32_t queueFamilyIndices[] = { static_cast< uint32_t >( indices.graphicsFamily_ ),
                                    static_cast< uint32_t >( indices.presentFamily_ ) };

  if ( indices.graphicsFamily_ != indices.presentFamily_ )
  {

    createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = queueFamilyIndices;

  }
  else
  {

    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

  }

  //
  // no transformation and ignore alpha
  //
  createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  //
  // clipping ignores pixels that can't be seen (eg. covered by another window)
  //
  createInfo.presentMode = presentMode;
  createInfo.clipped     = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if ( vkCreateSwapchainKHR( device_, &createInfo, nullptr, swapChain_.replace( ) ) != VK_SUCCESS )
  {

    throw std::runtime_error( "Failed to create swap chain" );

  }

  vkGetSwapchainImagesKHR( device_, swapChain_, &imageCount, nullptr );

  swapChainImages_.resize( imageCount );
  vkGetSwapchainImagesKHR( device_, swapChain_, &imageCount, swapChainImages_.data( ) );

  swapChainImageFormat_ = surfaceFormat.format;
  swapChainExtent_      = extent;

}



///
/// \brief VulkanGlfwWrapper::_createImageViews
///
void
VulkanGlfwWrapper::_createImageViews( )
{

  uint32_t imageCount = swapChainImages_.size( );

  swapChainImageViews_.resize( imageCount,
                               VDeleter< VkImageView >{ device_, vkDestroyImageView } );

  for ( uint32_t i = 0; i < imageCount; ++i )
  {

    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImages_[ i ];

    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // 1D, 2D, 3D, or cubemap
    createInfo.format   = swapChainImageFormat_;

    //
    // default color channel mapping
    //
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    //
    // using images as color targets without mipmapping or layering
    //
    createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel   = 0;
    createInfo.subresourceRange.levelCount     = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount     = 1;

    if ( vkCreateImageView( device_, &createInfo, nullptr, swapChainImageViews_[ i ].replace( ) ) != VK_SUCCESS )
    {

      throw std::runtime_error( "Failed to create image views" );

    }

  }

}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////                                        ////////////////////
///////////////////////         END PRIVATE FUNCTIONS          ////////////////////
///////////////////////                                        ////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


} // namespace graphics
