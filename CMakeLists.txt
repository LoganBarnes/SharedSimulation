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


set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

# other paths
set( THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty )


configure_file (
                ${SRC_DIR}/common/SharedSimulationConfig.hpp.in
                ${PROJECT_BINARY_DIR}/SharedSimulationConfig.hpp
                )


set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

include( cmake/DownloadProject.cmake )

################################################
# add glfw3 functionality
################################################
if ( USE_GLFW )
  # Download and unpack glfw at configure time
  downloadProject( glfw ${NO_THIRDPARTY_DOWNLOAD} )

  set( GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE )
  set( GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE )
  set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

  add_subdirectory( ${CMAKE_BINARY_DIR}/glfw-src
                    ${CMAKE_BINARY_DIR}/glfw-build )

  # glfw and glad header dir
  set(
      SYSTEM_INCLUDE_DIRS
      ${SYSTEM_INCLUDE_DIRS}

      ${CMAKE_BINARY_DIR}/glfw-src/include
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

  set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR}/glfw-src/CMake/modules )

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
# add optix functionality
################################################
if ( USE_OPTIX )

  find_package( CUDA  REQUIRED )
  find_package( OptiX REQUIRED )

  set( CUDA_SYSTEM_INCLUDE_DIRS ${CUDA_SYSTEM_INCLUDE_DIRS} ${OptiX_INCLUDE} ${THIRDPARTY}/include )
  set(
      CUDA_LINK_LIBS
      ${CUDA_LINK_LIBS}

      ${optix_LIBRARY}
      ${optixu_LIBRARY}
      )

  set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${OptiX_INCLUDE} ${THIRDPARTY}/include )
  set(
      LINK_LIBS
      ${LINK_LIBS}

      ${optix_LIBRARY}
      ${optixu_LIBRARY}
      )

  # windows defines for exports and warnings
  add_definitions( -Dsutil_sdk_EXPORTS -D_CRT_SECURE_NO_DEPRECATE )

  # model loading utils taken from the SDK samples
  add_library(
              optixUtil

              ${THIRDPARTY}/glad/src/glad.c
              ${THIRDPARTY}/optixUtil/HDRLoader.cpp
              ${THIRDPARTY}/optixUtil/PPMLoader.cpp
              ${THIRDPARTY}/optixUtil/tinyobjloader/tiny_obj_loader.cc
              ${THIRDPARTY}/optixUtil/rply-1.01/rply.c
              ${THIRDPARTY}/optixUtil/Mesh.cpp
              ${THIRDPARTY}/optixUtil/OptiXMesh.cpp
              )

  target_link_libraries     ( optixUtil ${optix_LIBRARY} ${optixu_LIBRARY} )
  target_include_directories(
                             optixUtil SYSTEM PUBLIC

                             ${OptiX_INCLUDE}
                             ${CUDA_INCLUDE_DIRS}
                             ${THIRDPARTY}/optixUtil
                             ${THIRDPARTY}/include
                             )

  set_target_properties(
                        optixUtil PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
                        )

  # imgui header dirs
  set(
      SYSTEM_INCLUDE_DIRS
      ${SYSTEM_INCLUDE_DIRS}

      ${THIRDPARTY}/optixUtil
      )


  # optixUtil lib
  set( LINK_LIBS ${LINK_LIBS} optixUtil )

  # must be built before project lib
  set( DEP_TARGETS optixUtil )

  install(
          TARGETS optixUtil
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          )

  message( "USING OPTIX" )

endif( )



################################################
# add imgui functionality
################################################
if ( USE_GUI )
  # Download and unpack imgui at configure time
  downloadProject( imgui ${NO_THIRDPARTY_DOWNLOAD} )

  # create lib here before any
  # strict compile flags are set
  add_library(
              imgui

              ${CMAKE_BINARY_DIR}/imgui-src/imgui.cpp
              ${CMAKE_BINARY_DIR}/imgui-src/imgui.h
              ${CMAKE_BINARY_DIR}/imgui-src/imgui_demo.cpp
              ${CMAKE_BINARY_DIR}/imgui-src/imgui_draw.cpp
              ${CMAKE_BINARY_DIR}/imgui-src/imgui_internal.h
              ${CMAKE_BINARY_DIR}/imgui-src/imconfig.h
              ${CMAKE_BINARY_DIR}/imgui-src/stb_rect_pack.h
              ${CMAKE_BINARY_DIR}/imgui-src/stb_textedit.h
              ${CMAKE_BINARY_DIR}/imgui-src/stb_truetype.h

              ${SRC_DIR}/gui/imgui_impl_glfw_gl3.h
              ${SRC_DIR}/gui/imgui_impl_glfw_gl3.cpp
              )

  target_include_directories(
                             imgui SYSTEM PUBLIC
                             ${CMAKE_BINARY_DIR}/glfw-src/include
                             ${THIRDPARTY}/include
                             ${CMAKE_BINARY_DIR}/imgui-src
                             ${SRC_DIR}/gui
                             )

   # imgui header dirs
  set(
      SYSTEM_INCLUDE_DIRS
      ${SYSTEM_INCLUDE_DIRS}

      ${CMAKE_BINARY_DIR}/imgui-src
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




################################################
# add gmock functionality
################################################
if ( USE_GMOCK )
  # Download and unpack googletest at configure time
  downloadProject( googletest ${NO_THIRDPARTY_DOWNLOAD} )

  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set( gtest_force_shared_crt ON CACHE BOOL "" FORCE )

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory( ${CMAKE_BINARY_DIR}/googletest-src/googlemock
                    ${CMAKE_BINARY_DIR}/googletest-build )

  # gmock and gtest header dir
  set(
      GMOCK_INCLUDE_DIRS
      ${CMAKE_BINARY_DIR}/googletest-src/googletest/include
      ${CMAKE_BINARY_DIR}/googletest-src/googlemock/include
      )

  # gmock lib
  set( GMOCK_LIB       gmock            )
  set( GMOCK_MAIN_LIB  gmock_main       )
  set( GMOCK_BOTH_LIBS gmock gmock_main )

  if ( hasParent )

    # gmock and gtest header dir
    set( GMOCK_INCLUDE_DIRS ${GMOCK_INCLUDE_DIRS} PARENT_SCOPE )

    # gmock lib
    set( GMOCK_LIB       ${GMOCK_LIB}       PARENT_SCOPE )
    set( GMOCK_MAIN_LIB  ${GMOCK_MAIN_LIB}  PARENT_SCOPE )
    set( GMOCK_BOTH_LIBS ${GMOCK_BOTH_LIBS} PARENT_SCOPE )

  endif( hasParent )

  message( "USING GMOCK" )

endif( USE_GMOCK )



################################################
# add glm functionality
################################################
set( SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} ${THIRDPARTY}/include )


################################################
# add shared source
################################################
# header dirs
set( SHARED_INCLUDE_DIRS ${SHARED_INCLUDE_DIRS} ${SRC_DIR} ${PROJECT_BINARY_DIR} )

# cpp files
set(
    SHARED_SOURCE

    ${ADDITIONAL_SOURCE}

    # driver
    ${SRC_DIR}/driver/Driver.cpp
    ${SRC_DIR}/driver/ContinuousDriver.cpp
    ${SRC_DIR}/driver/EventDriver.cpp

    # rendering
    ${SRC_DIR}/io/IOHandler.cpp

    # graphics
    ${SRC_DIR}/graphics/Camera.cpp

    # world
    ${SRC_DIR}/world/World.cpp
    )


if( hasParent )

  set( SHARED_SYSTEM_INCLUDE_DIRS ${SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
  set( SHARED_LINK_LIBS           ${LINK_LIBS}           PARENT_SCOPE )
  set( SHARED_DEP_TARGETS         ${DEP_TARGETS}         PARENT_SCOPE )


  set( SHARED_CUDA_SYSTEM_INCLUDE_DIRS ${CUDA_SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
  set( SHARED_CUDA_LINK_LIBS           ${CUDA_LINK_LIBS}           PARENT_SCOPE )
  set( SHARED_CUDA_DEP_TARGETS         ${CUDA_DEP_TARGETS}         PARENT_SCOPE )

  set( SHARED_UNCRUSTIFY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/formatting/uncrustify.cfg  PARENT_SCOPE )

  set( SHARED_INCLUDE_DIRS ${SHARED_INCLUDE_DIRS} PARENT_SCOPE )
  set( SHARED_SOURCE       ${SHARED_SOURCE}       PARENT_SCOPE )

endif( hasParent )

