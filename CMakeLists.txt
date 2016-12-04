

set( SHARED_USE_GLFW   OFF CACHE BOOL "" FORCE )
set( SHARED_USE_VULKAN OFF CACHE BOOL "" FORCE )
set( SHARED_USE_GLAD   OFF CACHE BOOL "" FORCE )
set( SHARED_USE_GLM    OFF CACHE BOOL "" FORCE )


set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

# other paths
set( THIRDPARTY  ${PROJECT_SOURCE_DIR}/thirdparty )

# set( PROJECT_CONFIG_FILE ${SRC_DIR}/common/ProjectConfig.hpp.in )


# add glfw3 functionality
if ( SHARED_USE_GLFW )

  set( GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE )
  set( GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE )
  set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

  add_subdirectory( ${THIRDPARTY}/glfw )

  # glfw header dir
  set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${THIRDPARTY}/glfw/include )

  # glfw lib
  set( LINK_LIBS ${LINK_LIBS} glfw )

  # must be built before project lib
  set( DEP_TARGETS glfw )

endif( SHARED_USE_GLFW )


# set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
     # ${THIRDPARTY}/glfw/CMake/modules )


# add vulkan functionality
if ( SHARED_USE_VULKAN )

  find_package( Vulkan REQUIRED )

  # vulkan header dirs
  set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIR} )

  # vulkan lib
  set( LINK_LIBS ${LINK_LIBS} ${VULKAN_LIBRARY} )

endif ( SHARED_USE_VULKAN )



# add glad functionality
if ( SHARED_USE_GLAD )

  set( THIRDPARTY_SOURCE ${THIRDPARTY_SOURCE} ${THIRDPARTY}/glad/glad.c )

endif()


if ( SHARED_USE_GLAD OR SHARED_USE_GLM )

  set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${THIRDPARTY}/include )

endif( )




set( SHARED_SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
set( SHARED_LINK_LIBS           ${LINK_LIBS}           PARENT_SCOPE )
set( SHARED_DEP_TARGETS         ${DEP_TARGETS}         PARENT_SCOPE )


# header dirs
set(
    SHARED_INCLUDE_DIRS

    ${SRC_DIR}

    PARENT_SCOPE
    )

# cpp files
set(
    SHARED_SOURCE

    ${THIRDPARTY_SOURCE}

    # driver
    ${SRC_DIR}/driver/Driver.cpp

    # rendering
    ${SRC_DIR}/io/IOHandler.cpp

    # graphics/glfw
    # ${SRC_DIR}/graphics/glfw/GlfwWrapper.cpp
    # ${SRC_DIR}/graphics/glfw/CallbackSingleton.cpp

    # world
    ${SRC_DIR}/world/World.cpp

    PARENT_SCOPE
    )


