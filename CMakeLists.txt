

option( USE_GLFW   "Use GLFW library"   OFF )
option( USE_VULKAN "Use Vulkan library" OFF )
option( USE_GLAD   "Use glad library"   OFF )
option( USE_GLM    "Use glm library"    OFF )
option( USE_OPTIX  "Use optix library"  OFF )
option( USE_GUI    "Use imgui library"  OFF )
option( USE_GMOCK  "Use gmock library"  OFF )


set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

# other paths
set( THIRDPARTY  ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty )


configure_file (
                ${SRC_DIR}/common/SharedSimulationConfig.hpp.in
                ${PROJECT_BINARY_DIR}/SharedSimulationConfig.hpp
                )


set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

################################################
# add glfw3 functionality
################################################
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



################################################
# add vulkan functionality
################################################
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




################################################
# add glm functionality
################################################
if ( USE_GLM )

  set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${THIRDPARTY}/include )

  message( "USING GLM" )

endif( )




################################################
# add optix functionality
################################################
if ( USE_OPTIX )

  find_package( OptiX REQUIRED )

  set( CUDA_SYSTEM_INCLUDE_DIRS ${CUDA_SYSTEM_INCLUDE_DIRS} ${OptiX_INCLUDE} )
  set(
      CUDA_LINK_LIBS
      ${CUDA_LINK_LIBS}

      ${optix_LIBRARY}
      ${optixu_LIBRARY}
      )

  set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${OptiX_INCLUDE} )
  set(
      LINK_LIBS
      ${LINK_LIBS}

      ${optix_LIBRARY}
      ${optixu_LIBRARY}
      )

  message( "USING OPTIX" )

endif( )



################################################
# add gmock functionality
################################################
if ( USE_GMOCK )

  # prevent /MT MSVC flag
  set( gtest_force_shared_crt ON CACHE BOOL "" FORCE )

  add_subdirectory( ${THIRDPARTY}/googletest/googlemock )

  # gmock and gtest header dir
  set(
      GMOCK_INCLUDE_DIRS
      ${THIRDPARTY}/googletest/googletest/include
      ${THIRDPARTY}/googletest/googlemock/include
      PARENT_SCOPE
      )

  # gmock lib
  set( GMOCK_LIB       gmock            PARENT_SCOPE )
  set( GMOCK_MAIN_LIB  gmock_main       PARENT_SCOPE )
  set( GMOCK_BOTH_LIBS gmock gmock_main PARENT_SCOPE )

  message( "USING GMOCK" )

endif( USE_GMOCK )





################################################
# add imgui functionality
################################################
if ( USE_GUI )

  # create lib here before any
  # strict compile flags are set
  add_library(
              imgui

              ${THIRDPARTY}/imgui/imgui.cpp
              ${THIRDPARTY}/imgui/imgui.h
              ${THIRDPARTY}/imgui/imgui_demo.cpp
              ${THIRDPARTY}/imgui/imgui_draw.cpp
              ${THIRDPARTY}/imgui/imgui_internal.h
              ${THIRDPARTY}/imgui/imconfig.h
              ${THIRDPARTY}/imgui/stb_rect_pack.h
              ${THIRDPARTY}/imgui/stb_textedit.h
              ${THIRDPARTY}/imgui/stb_truetype.h

              ${SRC_DIR}/gui/imgui_impl_glfw_gl3.h
              ${SRC_DIR}/gui/imgui_impl_glfw_gl3.cpp
              )

  target_include_directories(
                             imgui SYSTEM PUBLIC
                             ${THIRDPARTY}/glfw/include
                             ${THIRDPARTY}/include
                             ${THIRDPARTY}/imgui
                             ${SRC_DIR}/gui
                             )

   # imgui header dirs
  set(
      SYSTEM_INCLUDE_DIRS
      ${SYSTEM_INCLUDE_DIRS}

      ${THIRDPARTY}/imgui
      ${SRC_DIR}/gui
      )


  # imgui lib
  set( LINK_LIBS ${LINK_LIBS} imgui )

  # must be built before project lib
  set( DEP_TARGETS imgui )

  # imgui IOhandler class
  set(
      ADDITIONAL_SOURCE
      ${ADDITIONAL_SOURCE}

      ${SRC_DIR}/io/ImguiOpenGLIOHandler.cpp
      ${SRC_DIR}/io/ImguiCallback.cpp
      )

  message( "USING GUI" )

endif( USE_GUI )





set( SHARED_SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
set( SHARED_LINK_LIBS           ${LINK_LIBS}           PARENT_SCOPE )
set( SHARED_DEP_TARGETS         ${DEP_TARGETS}         PARENT_SCOPE )


set( SHARED_CUDA_SYSTEM_INCLUDE_DIRS ${CUDA_SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
set( SHARED_CUDA_LINK_LIBS           ${CUDA_LINK_LIBS}           PARENT_SCOPE )
set( SHARED_CUDA_DEP_TARGETS         ${CUDA_DEP_TARGETS}         PARENT_SCOPE )


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


