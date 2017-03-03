cmake_minimum_required ( VERSION 3.6.0 )
project ( ExampleSim )

set( SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

# include shared simulation project
add_subdirectory( ${SHARED_DIR} ${CMAKE_CURRENT_BINARY_DIR}/shared-build )

# file with main function
set( PROJECT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/ExampleSim.cpp )

# builds project using previously declared variables
include( ${SHARED_DIR}/cmake/DefaultProjectLibrary.cmake )

