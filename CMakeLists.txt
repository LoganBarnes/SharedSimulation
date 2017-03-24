cmake_minimum_required ( VERSION 3.6.0 )
project ( SharedSimulation )

get_directory_property( hasParent PARENT_DIRECTORY )

option( USE_GLFW   "Use GLFW library"   OFF )
option( USE_VULKAN "Use Vulkan library" OFF )
option( USE_GLAD   "Use glad library"   OFF )
option( USE_OPTIX  "Use optix library"  OFF )
option( USE_GUI    "Use imgui library"  OFF )
option( USE_GMOCK  "Use gmock library"  OFF )

option( NO_THIRDPARTY_DOWNLOAD "Assume thirdparty projects have already been downloaded" OFF )


set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src     )
set( INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include )

# other paths
set( THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty )

add_subdirectory( ${THIRDPARTY} )

configure_file (
                ${SRC_DIR}/common/SharedSimulationConfig.hpp.in
                ${PROJECT_BINARY_DIR}/SharedSimulationConfig.hpp
                )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )


################################################
# add glfw3 functionality
################################################
if ( USE_GLFW )

  # common wrapper functions
  set(
      ADDITIONAL_SOURCE
      ${ADDITIONAL_SOURCE}

      ${INC_DIR}/shared/graphics/Callback.hpp
      ${INC_DIR}/shared/graphics/GlfwWrapper.hpp
      ${INC_DIR}/shared/graphics/OpenGLWrapper.hpp
      ${INC_DIR}/shared/core/OpenGLIOHandler.hpp
      ${INC_DIR}/shared/graphics/SharedCallback.hpp

      ${SRC_DIR}/graphics/glfw/CallbackSingleton.hpp
      ${SRC_DIR}/graphics/glfw/CallbackSingleton.cpp
      ${SRC_DIR}/graphics/glfw/GlfwWrapper.cpp
      ${SRC_DIR}/graphics/opengl/OpenGLWrapper.cpp
      ${SRC_DIR}/io/OpenGLIOHandler.cpp
      ${SRC_DIR}/io/SharedCallback.cpp
      )

endif( USE_GLFW )



################################################
# add vulkan functionality
################################################
if ( USE_VULKAN )

  # common wrapper functions
  set(
      ADDITIONAL_SOURCE
      ${ADDITIONAL_SOURCE}

      ${INC_DIR}/shared/graphics/VulkanGlfwWrapper.hpp
      ${INC_DIR}/shared/core/VulkanIOHandler.hpp

      ${SRC_DIR}/graphics/vulkan/VulkanGlfwWrapper.cpp
      ${SRC_DIR}/io/VulkanIOHandler.cpp
      )

endif ( USE_VULKAN )



################################################
# add imgui functionality
################################################
if ( USE_GUI )

  # imgui IOhandler class
  set(
      ADDITIONAL_SOURCE
      ${ADDITIONAL_SOURCE}

      ${INC_DIR}/shared/graphics/ImguiCallback.hpp
      ${INC_DIR}/shared/core/ImguiOpenGLIOHandler.hpp

      ${SRC_DIR}/io/ImguiOpenGLIOHandler.cpp
      ${SRC_DIR}/io/ImguiCallback.cpp
      )

endif( USE_GUI )



################################################
# add glm functionality
################################################
set( SHARED_SYSTEM_INCLUDE_DIRS ${SHARED_SYSTEM_INCLUDE_DIRS} ${THIRDPARTY}/include )


################################################
# add shared source
################################################
# header dirs
set( SHARED_INCLUDE_DIRS ${SHARED_INCLUDE_DIRS} ${INC_DIR} ${SRC_DIR} ${PROJECT_BINARY_DIR} )

# cpp files
set(
    PROJECT_SOURCE

    ${ADDITIONAL_SOURCE}

    # driver
    ${INC_DIR}/shared/core/Driver.hpp
    ${INC_DIR}/shared/core/ContinuousDriver.hpp
    ${INC_DIR}/shared/core/EventDriver.hpp

    ${SRC_DIR}/driver/Driver.cpp
    ${SRC_DIR}/driver/ContinuousDriver.cpp
    ${SRC_DIR}/driver/EventDriver.cpp

    # rendering
    ${INC_DIR}/shared/core/IOHandler.hpp

    ${SRC_DIR}/io/IOHandler.cpp

    # graphics
    ${INC_DIR}/shared/graphics/GraphicsForwardDeclarations.hpp
    ${INC_DIR}/shared/graphics/Camera.hpp

    ${SRC_DIR}/graphics/Camera.cpp

    # world
    ${INC_DIR}/shared/core/World.hpp

    ${SRC_DIR}/world/World.cpp
    )

set( SHARED_SYSTEM_INCLUDE_DIRS ${THIRDPARTY_SYSTEM_INCLUDE_DIRS} )
set( SHARED_LINK_LIBS           ${THIRDPARTY_LINK_LIBS}           )
set( SHARED_DEP_TARGETS         ${THIRDPARTY_DEP_TARGETS}         )


set( SHARED_CUDA_SYSTEM_INCLUDE_DIRS ${THIRDPARTY_CUDA_SYSTEM_INCLUDE_DIRS} )
set( SHARED_CUDA_LINK_LIBS           ${THIRDPARTY_CUDA_LINK_LIBS}           )
set( SHARED_CUDA_DEP_TARGETS         ${THIRDPARTY_CUDA_DEP_TARGETS}         )

set( SHARED_UNCRUSTIFY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/formatting/uncrustify.cfg )

set ( FORCE_CUDA_STANDARD 11 )
set ( FORCE_CPP_STANDARD  14 )
include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DefaultProjectLibrary.cmake )


if( hasParent )

  set( SHARED_SYSTEM_INCLUDE_DIRS ${SHARED_SYSTEM_INCLUDE_DIRS}         PARENT_SCOPE )
  set( SHARED_LINK_LIBS           ${SHARED_LINK_LIBS}   ${PROJECT_NAME} PARENT_SCOPE )
  set( SHARED_DEP_TARGETS         ${SHARED_DEP_TARGETS} ${PROJECT_NAME} PARENT_SCOPE )


  set( SHARED_CUDA_SYSTEM_INCLUDE_DIRS ${SHARED_CUDA_SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
  set( SHARED_CUDA_LINK_LIBS           ${SHARED_CUDA_LINK_LIBS}           PARENT_SCOPE )
  set( SHARED_CUDA_DEP_TARGETS         ${SHARED_CUDA_DEP_TARGETS}         PARENT_SCOPE )

  set( SHARED_UNCRUSTIFY_FILE ${SHARED_UNCRUSTIFY_FILE} PARENT_SCOPE )

  set( SHARED_INCLUDE_DIRS ${INC_DIR} PARENT_SCOPE )

endif( hasParent )

