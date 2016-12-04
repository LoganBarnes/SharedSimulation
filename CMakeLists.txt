

option( USE_GLFW   "Use GLFW library"   OFF )
option( USE_VULKAN "Use Vulkan library" OFF )
option( USE_GLAD   "Use glad library"   OFF )
option( USE_GLM    "Use glm library"    OFF )


set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

# other paths
set( THIRDPARTY  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty )


configure_file (
                ${SRC_DIR}/common/SharedSimulationConfig.hpp.in
                ${PROJECT_BINARY_DIR}/SharedSimulationConfig.hpp
                )


# add glfw3 functionality
if ( USE_GLFW )

  set( GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE )
  set( GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE )
  set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

  add_subdirectory( ${THIRDPARTY}/glfw )

  # glfw and glad header dir
  set(
      SYSTEM_INCLUDE_DIRS
      ${SYSTEM_INCLUDE_DIRS}

      ${THIRDPARTY}/glfw/include
      ${THIRDPARTY}/include
      )

  # glfw lib
  set( LINK_LIBS ${LINK_LIBS} glfw )

  # must be built before project lib
  set( DEP_TARGETS glfw )

  # common wrapper functions
  set(
      ADDITIONAL_SOURCE
      ${ADDITIONAL_SOURCE}

      ${SRC_DIR}/graphics/glfw/CallbackSingleton.cpp
      ${SRC_DIR}/graphics/glfw/GlfwWrapper.cpp
      ${SRC_DIR}/graphics/opengl/OpenGLWrapper.cpp
      ${SRC_DIR}/io/OpenGLIOHandler.cpp
      ${SRC_DIR}/io/SharedCallback.cpp
      ${THIRDPARTY}/glad/src/glad.c
      )

  message( "USING GLFW" )

endif( USE_GLFW )



# add vulkan functionality
if ( USE_VULKAN )

  set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${THIRDPARTY}/glfw/CMake/modules )

  find_package( Vulkan REQUIRED )

  # vulkan header dirs
  set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIR} )

  # vulkan lib
  set( LINK_LIBS ${LINK_LIBS} ${VULKAN_LIBRARY} )

  # common wrapper functions
  set(
      ADDITIONAL_SOURCE
      ${ADDITIONAL_SOURCE}

      ${SRC_DIR}/graphics/vulkan/VulkanGlfwWrapper.cpp
      ${SRC_DIR}/io/VulkanIOHandler.cpp
      )

  message( "USING VULKAN" )

endif ( USE_VULKAN )




if ( USE_GLM )

  set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${THIRDPARTY}/include )

  message( "USING GLM" )

endif( )





set( SHARED_SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
set( SHARED_LINK_LIBS           ${LINK_LIBS}           PARENT_SCOPE )
set( SHARED_DEP_TARGETS         ${DEP_TARGETS}         PARENT_SCOPE )

set( SHARED_UNCRUSTIFY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/formatting/uncrustify.cfg  PARENT_SCOPE )


# header dirs
set(
    SHARED_INCLUDE_DIRS

    ${SRC_DIR}

    PARENT_SCOPE
    )

# cpp files
set(
    SHARED_SOURCE

    ${ADDITIONAL_SOURCE}

    # driver
    ${SRC_DIR}/driver/Driver.cpp

    # rendering
    ${SRC_DIR}/io/IOHandler.cpp

    # graphics
    ${SRC_DIR}/graphics/Camera.cpp

    # world
    ${SRC_DIR}/world/World.cpp

    PARENT_SCOPE
    )


