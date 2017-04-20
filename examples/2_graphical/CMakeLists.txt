cmake_minimum_required ( VERSION 3.2.0 )
project ( ExampleSimGraphical )

set( USE_GLFW  ON CACHE BOOL "" FORCE )
set( USE_GLM   ON CACHE BOOL "" FORCE )
set( USE_GUI   ON CACHE BOOL "" FORCE )

set( SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

# include shared simulation project
add_subdirectory( ${SHARED_DIR} ${CMAKE_CURRENT_BINARY_DIR}/shared-build )

# header dirs
set( PROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/src )

# cpp files
set(
    PROJECT_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CubeWorld.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CubeImguiOpenGLIOHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RotatingCube.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CubeCallback.cpp
    )

set( PROJECT_NAMESPACE example )
set( SHADER_PATH ${SHARED_DIR}/shaders )

set( PROJECT_CONFIG_FILE ${SHARED_DIR}/src/common/ProjectConfig.hpp.in )

# file with main function
set( PROJECT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/src/ExampleSim.cpp )

# builds project using previously declared variables
set( FORCE_CPP_STANDARD 14 )
include( ${SHARED_DIR}/cmake/DefaultProjectLibrary.cmake )

