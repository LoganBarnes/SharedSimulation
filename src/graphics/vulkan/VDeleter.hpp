#ifndef VDeleter_hpp
#define VDeleter_hpp


namespace
{

///
/// \brief CreateDebugReportCallbackEXT
/// \param instance
/// \param pCreateInfo
/// \param pAllocator
/// \param pCallback
/// \return
///
VkResult
CreateDebugReportCallbackEXT(
                             VkInstance instance,
                             const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator,
                             VkDebugReportCallbackEXT* pCallback
                             )
{

  auto func = reinterpret_cast< PFN_vkCreateDebugReportCallbackEXT > ( vkGetInstanceProcAddr( instance, "vkCreateDebugReportCallbackEXT" ) );

  if ( func )
  {

    return func(
                instance,
                pCreateInfo,
                pAllocator,
                pCallback
                );

  }
  else
  {

    return VK_ERROR_EXTENSION_NOT_PRESENT;

  }

}


///
/// \brief DestroyDebugReportCallbackEXT
/// \param instance
/// \param callback
/// \param pAllocator
///
void
DestroyDebugReportCallbackEXT(
                              VkInstance instance,
                              VkDebugReportCallbackEXT callback,
                              const VkAllocationCallbacks* pAllocator
                              )
{

  auto func = reinterpret_cast< PFN_vkDestroyDebugReportCallbackEXT >( vkGetInstanceProcAddr( instance, "vkDestroyDebugReportCallbackEXT" ) );

  if ( func)
  {

    func(
         instance,
         callback,
         pAllocator
         );

  }

}


} // namespace


////////////////////////////////////////////////////////////////////////////
/// \brief The VDeleter class
///
///        Wrapper around vulkan functions enabling
///        automatic cleanup
///
/// take from:
///  https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Base_code
////////////////////////////////////////////////////////////////////////////
template< typename T >
class VDeleter
{

public:

  ////////////////////////////////////////////////////////////////////////////
  /// \brief VDeleter
  ////////////////////////////////////////////////////////////////////////////
  VDeleter( ) : VDeleter( [ ](
                              T,
                              VkAllocationCallbacks*
                              ) {} )
  {}

  ////////////////////////////////////////////////////////////////////////////
  /// \brief VDeleter
  ///
  ///        Only the object itself needs to be passed to
  ///        the cleanup function, so we can simply construct
  ///        a VDeleter with just the function as argument
  ///
  /// \param deletef
  ////////////////////////////////////////////////////////////////////////////
  VDeleter( std::function< void(
                                T,
                                VkAllocationCallbacks*
                                ) > deletef )
  {

    this->deleter = [ = ]( T obj ) {
                      deletef( obj, nullptr );
                    };

  }


  ////////////////////////////////////////////////////////////////////////////
  /// \brief VDeleter
  ///
  ///        A VkInstance also needs to be passed to the cleanup
  ///        function, so we use the VDeleter constructor that takes
  ///        the VkInstance reference and cleanup function as parameters
  ///
  /// \param instance
  /// \param deletef
  ////////////////////////////////////////////////////////////////////////////
  VDeleter(
           const VDeleter< VkInstance > &instance,
           std::function< void( VkInstance, T, VkAllocationCallbacks* ) > deletef
           )
  {

    this->deleter = [ &instance, deletef ]( T obj ) {
                      deletef( instance, obj, nullptr );
                    };

  }


  ////////////////////////////////////////////////////////////////////////////
  /// \brief VDeleter
  ///
  ///        Similar to the previous constructor, but a VkDevice
  ///        must be passed instead of a VkInstance
  ///
  /// \param device
  /// \param deletef
  ////////////////////////////////////////////////////////////////////////////
  VDeleter(
           const VDeleter< VkDevice > &device,
           std::function< void( VkDevice, T, VkAllocationCallbacks* ) > deletef
           )
  {

    this->deleter = [ &device, deletef ]( T obj ) {
                      deletef( device, obj, nullptr );
                    };

  }


  ~VDeleter( )
  {

    cleanup( );

  }


  const T*
  operator&( ) const
  {

    return &object;

  }


  T*
  replace( )
  {

    cleanup( );
    return &object;

  }


  operator T( ) const
  {

    return object;

  }


  void
  operator=( T rhs )
  {
    if ( rhs != object )
    {
      cleanup( );
      object = rhs;
    }
  }


  template< typename V >
  bool
  operator==( V rhs )
  {

    return object == T( rhs );

  }


private:

  T object
  {

    VK_NULL_HANDLE

  };

  std::function< void( T ) > deleter;


  ///
  /// \brief cleanup
  ///
  void
  cleanup( )
  {

    if ( object != VK_NULL_HANDLE )
    {

      deleter( object );

    }

    object = VK_NULL_HANDLE;

  }


};


#endif // VDeleter_hpp

