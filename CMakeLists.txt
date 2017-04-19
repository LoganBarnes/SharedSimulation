cmake_minimum_required ( VERSION 3.7.1 )
project ( SharedSimulation )

option( USE_GLM    "Use the GLM library"    OFF )
option( USE_GLFW   "Use the GLFW library"   OFF )
option( USE_VULKAN "Use the Vulkan library" OFF )
option( USE_OPTIX  "Use the optix library"  OFF )
option( USE_GUI    "Use the imgui library"  OFF )
option( USE_GMOCK  "Use the gmock library"  OFF )

option( BUILD_SHARED_TESTS "Build unit tests for the shared simulation lib" OFF )

if ( ${BUILD_SHARED_TESTS} )
  set( USE_GMOCK ON CACHE BOOL "" FORCE )
  message( "TODO: Add message about enabling gmock" )
endif()


# common directory paths
set( SRC_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/src        )
set( INC_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/include    )
set( THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty )

# configure and build necessary thirdparty libs
add_subdirectory( ${THIRDPARTY} )

# creates an hpp file with defines for each USE_<lib> setting
configure_file (
                ${SRC_DIR}/common/SharedSimulationConfig.hpp.in
                ${PROJECT_BINARY_DIR}/SharedSimulationConfig.hpp
                )


################################################
# add classes using glm
################################################
if ( USE_GLM )

  list(
       APPEND ADDITIONAL_SOURCE

       ${INC_DIR}/shared/graphics/Camera.hpp
       ${INC_DIR}/shared/graphics/GlmCamera.hpp

       ${SRC_DIR}/graphics/Camera.cpp
       ${SRC_DIR}/graphics/GlmCamera.cpp
       )

  list(
       APPEND SHARED_TEST_SOURCE

       ${SRC_DIR}/graphics/testing/CameraUnitTests.cpp
       ${SRC_DIR}/graphics/testing/GlmCameraUnitTests.cpp
       )

endif( USE_GLM )

################################################
# add glfw3 functionality
################################################
if ( USE_GLFW )

  # glfw/opengl wrapper functions and input handling
  set(
      ADDITIONAL_SOURCE
      ${ADDITIONAL_SOURCE}

      ${INC_DIR}/shared/graphics/GLID.hpp
      ${INC_DIR}/shared/graphics/Callback.hpp
      ${INC_DIR}/shared/graphics/GlfwWrapper.hpp
      ${INC_DIR}/shared/graphics/OpenGLWrapper.hpp
      ${INC_DIR}/shared/graphics/OpenGLHelper.hpp
      ${INC_DIR}/shared/core/OpenGLIOHandler.hpp
      ${INC_DIR}/shared/graphics/SharedCallback.hpp

      ${SRC_DIR}/graphics/glfw/CallbackSingleton.hpp
      ${SRC_DIR}/graphics/glfw/CallbackSingleton.cpp
      ${SRC_DIR}/graphics/glfw/GlfwWrapper.cpp
      ${SRC_DIR}/graphics/opengl/OpenGLWrapper.cpp
      ${SRC_DIR}/graphics/opengl/OpenGLHelper.cpp
      ${SRC_DIR}/io/OpenGLIOHandler.cpp
      ${SRC_DIR}/io/SharedCallback.cpp
      )

  list(
       APPEND SHARED_TEST_SOURCE

       ${SRC_DIR}/graphics/testing/OpenGLWrapperUnitTests.cpp
       ${SRC_DIR}/graphics/testing/OpenGLHelperUnitTests.cpp
       )

endif( USE_GLFW )



################################################
# add vulkan functionality
################################################
if ( USE_VULKAN )

  # vulkan wrapper functions
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

  # imgui callbacks
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
# add shared simulation files
################################################
# header dirs
list(
     APPEND SHARED_INCLUDE_DIRS

     ${INC_DIR}
     ${SRC_DIR}
     ${PROJECT_BINARY_DIR}
     )

# cpp files
set(
    PROJECT_SOURCE

    ${ADDITIONAL_SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/formatting/uncrustify.cfg

    # driver
    ${INC_DIR}/shared/core/Driver.hpp
    ${INC_DIR}/shared/core/ContinuousDriver.hpp
    ${INC_DIR}/shared/core/EventDriver.hpp

    ${SRC_DIR}/driver/Driver.cpp
    ${SRC_DIR}/driver/ContinuousDriver.cpp
    ${SRC_DIR}/driver/EventDriver.cpp

    # io
    ${INC_DIR}/shared/core/IOHandler.hpp

    ${SRC_DIR}/io/IOHandler.cpp

    # graphics
    ${INC_DIR}/shared/graphics/GraphicsForwardDeclarations.hpp

    # world
    ${INC_DIR}/shared/core/World.hpp

    ${SRC_DIR}/world/World.cpp
    )

list(
     APPEND SHARED_TEST_SOURCE

     ${SRC_DIR}/driver/testing/DriverUnitTests.cpp
     )


if ( ${BUILD_SHARED_TESTS} )

  set( TESTING_SOURCE ${SHARED_TEST_SOURCE} )

endif( )


# append thirdparty variables
set( SHARED_SYSTEM_INCLUDE_DIRS ${THIRDPARTY_SYSTEM_INCLUDE_DIRS} ${THIRDPARTY}/include )
set( SHARED_LINK_LIBS           ${THIRDPARTY_LINK_LIBS}                                 )
set( SHARED_DEP_TARGETS         ${THIRDPARTY_DEP_TARGETS}                               )

set( SHARED_CUDA_SYSTEM_INCLUDE_DIRS ${THIRDPARTY_CUDA_SYSTEM_INCLUDE_DIRS} )
set( SHARED_CUDA_LINK_LIBS           ${THIRDPARTY_CUDA_LINK_LIBS}           )
set( SHARED_CUDA_DEP_TARGETS         ${THIRDPARTY_CUDA_DEP_TARGETS}         )

set( PROJECT_INSTALL_HEADER_DIRS ${PROJECT_INSTALL_HEADER_DIRS} ${INC_DIR}/shared )

set( SHARED_UNCRUSTIFY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/formatting/uncrustify.cfg )

# build the simulation library
set ( FORCE_CUDA_STANDARD 11 )
set ( FORCE_CPP_STANDARD  14 )
include( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DefaultProjectLibrary.cmake )


# set variables for the parent project if there is one
get_directory_property( hasParent PARENT_DIRECTORY )

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

