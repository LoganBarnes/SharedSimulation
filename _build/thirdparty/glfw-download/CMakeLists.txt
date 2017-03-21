cmake_minimum_required( VERSION 3.6.0 )

project( glfw-download NONE )

include( ExternalProject )

ExternalProject_Add( glfw-external
  GIT_REPOSITORY     https://github.com/glfw/glfw.git
  GIT_TAG            master
  SOURCE_DIR         "/Users/Logan/Documents/projects/cpp/playground/shared/_build/glfw-src"
  BINARY_DIR         "/Users/Logan/Documents/projects/cpp/playground/shared/_build/glfw-build"
  CONFIGURE_COMMAND  ""
  BUILD_COMMAND      ""
  INSTALL_COMMAND    ""
  TEST_COMMAND       ""
)
