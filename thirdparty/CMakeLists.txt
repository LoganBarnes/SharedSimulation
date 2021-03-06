cmake_minimum_required ( VERSION 3.7.1 )
project ( SimulationThirdparty )

option( USE_GLM    "Download and compile the glm library"    OFF )
option( USE_GLFW   "Download and compile the GLFW library"   OFF )
option( USE_OPENGL "Use the OpenGL framework"                OFF )
option( USE_VULKAN "Download and compile the Vulkan library" OFF )
option( USE_OPTIX  "Download and compile the optix library"  OFF )
option( USE_GUI    "Download and compile the imgui library"  OFF )
option( USE_GMOCK  "Download and compile the gmock library"  OFF )


# for readability
set( THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR} )

# append non-standard Find<Lib>.cmake files
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

# allows for downloading of thirdparty libs at configure time
include( ../cmake/DownloadProject.cmake )

set( CMAKE_MACOSX_RPATH TRUE )

###\todo Add checks to make sure dependent libraries will be built

################################################
# add glfw3 functionality
################################################
if ( USE_GLM )

  # Download and unpack glm at configure time
  download_project(
                   PROJ                glm
                   GIT_REPOSITORY      https://github.com/g-truc/glm.git
                   GIT_TAG             master
                   UPDATE_DISCONNECTED 1
                   QUIET
                   )

  list( APPEND THIRDPARTY_SYSTEM_INCLUDE_DIRS ${glm_SOURCE_DIR} )

  message( "USING GLM" )

endif( )


################################################
# add glfw3 functionality
################################################
if ( USE_GLFW )

  # Download and unpack glfw at configure time
  download_project(
                   PROJ                glfw
                   GIT_REPOSITORY      https://github.com/glfw/glfw.git
                   GIT_TAG             master
                   UPDATE_DISCONNECTED 1
                   QUIET
                   )

  set( GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE )
  set( GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE )
  set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

  # compile glfw with current project
  add_subdirectory( ${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} )

  # glfw header dir
  list(
       APPEND THIRDPARTY_SYSTEM_INCLUDE_DIRS
       ${glfw_SOURCE_DIR}/include
       )

  # glfw lib
  list( APPEND THIRDPARTY_LINK_LIBS glfw )

  # glfw must be built before project lib
  list( APPEND THIRDPARTY_DEP_TARGETS glfw )

  message( "USING GLFW" )

endif( USE_GLFW )



################################################
# add OpenGL functionality
################################################
if ( USE_OPENGL )

  # build glad as its own library to avoid problems with strict compile flags
  add_library               ( glad ${THIRDPARTY}/glad/src/glad.c       )
  target_include_directories( glad SYSTEM PUBLIC ${THIRDPARTY}/include )

  install(
          TARGETS glad
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          )

    # glad header dir
    list(
        APPEND THIRDPARTY_SYSTEM_INCLUDE_DIRS
        ${THIRDPARTY}/include
        )

  # glad lib
  list( APPEND THIRDPARTY_LINK_LIBS glad )

  # glad must be built before project lib
  list( APPEND THIRDPARTY_DEP_TARGETS glad )

  set( IMGUI_LINK_LIB   glad )
  set( IMGUI_DEP_TARGET glad )

  message( "USING OPENGL" )

endif( USE_OPENGL )



################################################
# add Vulkan functionality
################################################
if ( USE_VULKAN )

  # append FindVulkan.cmake file
  set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${glfw_SOURCE_DIR}/CMake/modules )

  find_package( Vulkan REQUIRED )

  # vulkan header dirs
  list( APPEND THIRDPARTY_SYSTEM_INCLUDE_DIRS ${VULKAN_INCLUDE_DIR} )

  # vulkan lib
  list( APPEND THIRDPARTY_LINK_LIBS ${VULKAN_LIBRARY} )

  set( IMGUI_LINK_LIB ${VULKAN_LIBRARY} )

  message( "USING VULKAN" )

endif ( USE_VULKAN )



################################################
# add optix functionality
################################################
if ( USE_OPTIX )

  find_package( CUDA  REQUIRED )
  find_package( OptiX REQUIRED )

  list(
       APPEND
       THIRDPARTY_CUDA_SYSTEM_INCLUDE_DIRS

       ${OptiX_INCLUDE}
       ${THIRDPARTY}/include
       )

   list(
       APPEND
       THIRDPARTY_CUDA_LINK_LIBS

       ${optix_LIBRARY}
       ${optixu_LIBRARY}
       )

   list(
        APPEND
        THIRDPARTY_SYSTEM_INCLUDE_DIRS

        ${OptiX_INCLUDE}
        ${THIRDPARTY}/include
        )

  list(
       APPEND
       THIRDPARTY_LINK_LIBS

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

  # optixUtil header dirs
  list( APPEND THIRDPARTY_SYSTEM_INCLUDE_DIRS ${THIRDPARTY}/optixUtil )

  # optixUtil lib
  list( APPEND THIRDPARTY_LINK_LIBS optixUtil )

  # optixUtil must be built before project lib
  list( APPEND THIRDPARTY_DEP_TARGETS optixUtil )

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
  download_project(
                   PROJ                imgui
                   GIT_REPOSITORY      https://github.com/ocornut/imgui.git
                   GIT_TAG             master
                   UPDATE_DISCONNECTED 1
                   QUIET
                   )

  if ( USE_OPENGL )
    set(
        IMGUI_GL_SOURCE
        ${SRC_DIR}/gui/imgui_impl_glfw_gl3.cpp
        ${SRC_DIR}/gui/imgui_impl_glfw_gl3.h
        )
  endif( )


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

              ${IMGUI_GL_SOURCE}
              )

  target_include_directories(
                             imgui SYSTEM PUBLIC
                             ${glfw_SOURCE_DIR}/include
                             ${THIRDPARTY}/include
                             ${CMAKE_BINARY_DIR}/imgui-src
                             ${SRC_DIR}/gui
                             )

  target_link_libraries( imgui glfw ${IMGUI_LINK_LIB}   )
  add_dependencies     ( imgui glfw ${IMGUI_DEP_TARGET} )

  set_property( TARGET imgui PROPERTY CXX_STANDARD          11 )
  set_property( TARGET imgui PROPERTY CXX_STANDARD_REQUIRED ON )

  # rpath for unix libs
  set_property( TARGET imgui PROPERTY INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib )

  install(
          TARGETS imgui
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          )

   # imgui header dirs
  list(
       APPEND
       THIRDPARTY_SYSTEM_INCLUDE_DIRS

       ${CMAKE_BINARY_DIR}/imgui-src
       ${SRC_DIR}/gui
       )

  # imgui lib
  list( APPEND THIRDPARTY_LINK_LIBS imgui )

  # imgui must be built before project lib
  list( APPEND THIRDPARTY_DEP_TARGETS imgui )

  install()

  message( "USING GUI" )

endif( USE_GUI )




################################################
# add gmock functionality
################################################
if ( USE_GMOCK )

  # Download and unpack googletest at configure time
  download_project(
                   PROJ                googletest
                   GIT_REPOSITORY      https://github.com/google/googletest.git
                   GIT_TAG             master
                   UPDATE_DISCONNECTED 1
                   QUIET
                   )

  # Prevent overriding the parent project's compiler/linker settings on Windows
  set( gtest_force_shared_crt  ON CACHE BOOL "" FORCE )

  # Add googlemock directly to the project build
  add_subdirectory( ${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} )

  # gmock and gtest header dirs
  list(
       APPEND
       THIRDPARTY_SYSTEM_INCLUDE_DIRS

       ${gtest_SOURCE_DIR}/include
       ${gmock_SOURCE_DIR}/include
       )

  message( "USING GMOCK" )

endif( USE_GMOCK )



# set variables for the parent project if there is one
get_directory_property( hasParent PARENT_DIRECTORY )

if( hasParent )

  set( THIRDPARTY_SYSTEM_INCLUDE_DIRS ${THIRDPARTY_SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
  set( THIRDPARTY_LINK_LIBS           ${THIRDPARTY_LINK_LIBS}           PARENT_SCOPE )
  set( THIRDPARTY_DEP_TARGETS         ${THIRDPARTY_DEP_TARGETS}         PARENT_SCOPE )

  set( THIRDPARTY_CUDA_SYSTEM_INCLUDE_DIRS ${THIRDPARTY_CUDA_SYSTEM_INCLUDE_DIRS} PARENT_SCOPE )
  set( THIRDPARTY_CUDA_LINK_LIBS           ${THIRDPARTY_CUDA_LINK_LIBS}           PARENT_SCOPE )
  set( THIRDPARTY_CUDA_DEP_TARGETS         ${THIRDPARTY_CUDA_DEP_TARGETS}         PARENT_SCOPE )

endif( hasParent )

