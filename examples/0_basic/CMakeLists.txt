cmake_minimum_required ( VERSION 3.2.0 )
project ( ExampleSimBasic )

set( USE_GLFW  OFF CACHE BOOL "" FORCE )
set( USE_GLM   OFF CACHE BOOL "" FORCE )
set( USE_GUI   OFF CACHE BOOL "" FORCE )

set( SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

# include shared simulation project
add_subdirectory( ${SHARED_DIR} ${CMAKE_CURRENT_BINARY_DIR}/shared-build )

# file with main function
set( PROJECT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/ExampleSim.cpp )

# builds project using previously declared variables
include( ${SHARED_DIR}/cmake/DefaultProjectLibrary.cmake )

