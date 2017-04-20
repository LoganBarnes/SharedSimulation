#include "CubeImguiOpenGLIOHandler.hpp"

// system
#include <iostream>
#include <deque>
#include <memory>

// shared
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "shared/graphics/OpenGLWrapper.hpp"
#include "shared/graphics/GlmCamera.hpp"

// project
#include "CubeWorld.hpp"
#include "RotatingCube.hpp"
#include "ExampleSimGraphicalConfig.hpp"
#include "CubeCallback.hpp"
#include "shared/graphics/ImguiCallback.hpp"


namespace example
{


/////////////////////////////////////////////
/// \brief Renderer::Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
CubeImguiOpenGLIOHandler::CubeImguiOpenGLIOHandler( CubeWorld &cubeWorld )
  : shared::ImguiOpenGLIOHandler( cubeWorld, true, 1040, 720 )
  , cubeWorld_( cubeWorld )
{
  upCamera_->lookAt(
                    glm::vec3( 0.0f, 0.0f, 15.0f ), // eye
                    glm::vec3( 0.0f )               // look point (origin)
                    );

  std::unique_ptr< CubeCallback > cubeCallback( new CubeCallback( *this ) );
  imguiCallback_->setCallback( std::move( cubeCallback ) );

  upGLWrapper_->addProgram(
                           "cubeProgram",
                           SHADER_PATH + "simple/shader.vert",
                           SHADER_PATH + "simple/shader.frag"
                           );

  std::vector< float > vbo
  {
    -1,  1, -1, // 1
    1,   1, -1, // 2
    -1,  1,  1, // 3
    1,   1,  1, // 4
    -1, -1, -1, // 5
    1,  -1, -1, // 6
    1,  -1,  1, // 7
    -1, -1,  1  // 8
  };

  graphics::VAOSettings settings = { "cubeProgram", 0 };
  settings.settings.push_back( { "inPosition", 3, GL_FLOAT, nullptr } );

  upGLWrapper_->addBuffer(
                          "cubeVBO",
                          vbo.data( ),
                          vbo.size( ),
                          GL_STATIC_DRAW,
                          settings
                          );

  std::vector< unsigned short > ibo
  {
    3, 2, 6, 7, 4,
    2, 0, 3, 1, 6,
    5, 4, 1, 0, 3
  };

  upGLWrapper_->addIndexBuffer(
                               "cubeIBO",
                               ibo.data( ),
                               ibo.size( ),
                               GL_STATIC_DRAW
                               );
}



/////////////////////////////////////////////
/// \brief Renderer::~Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
CubeImguiOpenGLIOHandler::~CubeImguiOpenGLIOHandler( )
{}



void
CubeImguiOpenGLIOHandler::addRandomCube( )
{
  cubeWorld_.addRandomCube( );
}



void
CubeImguiOpenGLIOHandler::removeOldestCube( )
{
  cubeWorld_.removeOldestCube( );
}



void
CubeImguiOpenGLIOHandler::_onRender( const double )
{
  glm::vec3 color( 1.0f, 0.0f, 0.0 );

  upGLWrapper_->clearWindow( );
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  upGLWrapper_->useProgram( "cubeProgram" );

  const glm::mat4 projectionView = upCamera_->getPerspectiveProjectionViewMatrix( );

  const CubeVec &cubes          = cubeWorld_.getCubes( );
  glm::mat4 projectionViewModel = glm::mat4( );

  for ( auto &upCube : cubes )
  {
    projectionViewModel = projectionView * upCube->getTransformationMatrix( );

    upGLWrapper_->setMatrixUniform(
                                   "cubeProgram",
                                   "projectionViewModel",
                                   glm::value_ptr( projectionViewModel )
                                   );

    upGLWrapper_->setFloatUniform(
                                  "cubeProgram",
                                  "color",
                                  glm::value_ptr( color ),
                                  3
                                  );

    upGLWrapper_->renderBuffer( "cubeVBO", 0, 15, GL_TRIANGLE_STRIP, "cubeIBO" );
  }

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

} // CubeImguiOpenGLIOHandler::_onRender



void
CubeImguiOpenGLIOHandler::_onGuiRender( )
{
  bool alwaysOpen = true;

  ImGui::SetNextWindowSize( ImVec2( 0, 0 ), ImGuiSetCond_FirstUseEver ); // auto scale size
  ImGui::SetNextWindowPos ( ImVec2( 0, 0 ), ImGuiSetCond_FirstUseEver );

  ImGui::Begin( "Cube Simulation", &alwaysOpen );

  // FPS
  ImGui::Text(
              "Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO( ).Framerate,
              ImGui::GetIO( ).Framerate
              );

  if ( ImGui::CollapsingHeader( "Controls", "controls", false, true ) )
  {
    ImGui::Text( "ESC - exit\n\n A  - add random cube\n R  - remove oldest cube\n" );
  }

  ImGui::End( );
} // _onGuiRender



} // namespace example
