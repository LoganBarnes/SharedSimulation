cmake_minimum_required( VERSION 3.6.0 )

project( imgui-download NONE )

include( ExternalProject )

ExternalProject_Add( imgui-external
  GIT_REPOSITORY     https://github.com/ocornut/imgui.git
  GIT_TAG            master
  SOURCE_DIR         "/Users/Logan/Documents/projects/cpp/playground/shared/_build/imgui-src"
  BINARY_DIR         "/Users/Logan/Documents/projects/cpp/playground/shared/_build/imgui-build"
  CONFIGURE_COMMAND  ""
  BUILD_COMMAND      ""
  INSTALL_COMMAND    ""
  TEST_COMMAND       ""
)
