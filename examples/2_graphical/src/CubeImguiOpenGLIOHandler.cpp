#include "CubeImguiOpenGLIOHandler.hpp"

// project
#include "CubeWorld.hpp"
#include "RotatingCube.hpp"
#include "ExampleSimGraphicalConfig.hpp"
#include "CubeCallback.hpp"

// shared
#include "shared/graphics/ImguiCallback.hpp"
#include "shared/graphics/OpenGLHelper.hpp"
#include "shared/graphics/GlmCamera.hpp"
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/gtc/type_ptr.hpp>

// system
#include <iostream>
#include <deque>
#include <memory>


namespace example
{


/////////////////////////////////////////////
/// \brief Renderer::Renderer
/////////////////////////////////////////////
CubeImguiOpenGLIOHandler::CubeImguiOpenGLIOHandler( CubeWorld &cubeWorld )
  : shs::ImguiOpenGLIOHandler( cubeWorld, true, 1040, 720 )
  , cubeWorld_( cubeWorld )
{
  upCamera_->lookAt(
                    glm::vec3( 0.0f, 0.0f, 15.0f ), // eye
                    glm::vec3( 0.0f )               // look point (origin)
                    );

  std::unique_ptr< CubeCallback > cubeCallback( new CubeCallback( *this ) );
  imguiCallback_->setCallback( std::move( cubeCallback ) );

  glIds_.program =
    shg::OpenGLHelper::createProgram(
                                     SHADER_PATH + "simple/shader.vert",
                                     SHADER_PATH + "simple/shader.frag"
                                     );

  std::vector< float > vbo =
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

  std::vector< shg::VAOElement > vao =
  {
    { "inPosition", 3, GL_FLOAT, nullptr }
  };

  std::vector< unsigned short > ibo
  {
    3, 2, 6, 7, 4,
    2, 0, 3, 1, 6,
    5, 4, 1, 0, 3
  };


  glIds_.vbo = shg::OpenGLHelper::createBuffer(
                                               vbo.data( ),
                                               vbo.size( )
                                               );

  glIds_.vao = shg::OpenGLHelper::createVao(
                                            glIds_.program,
                                            glIds_.vbo,
                                            0,
                                            vao
                                            );


  glIds_.ibo = shg::OpenGLHelper::createBuffer(
                                               ibo.data( ),
                                               ibo.size( ),
                                               GL_ELEMENT_ARRAY_BUFFER,
                                               GL_STATIC_DRAW
                                               );
}



/////////////////////////////////////////////
/// \brief Renderer::~Renderer
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

  shg::OpenGLHelper::clearFramebuffer( );
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  glUseProgram( *glIds_.program );

  const glm::mat4 projectionView = upCamera_->getPerspectiveProjectionViewMatrix( );

  const CubeVec &cubes          = cubeWorld_.getCubes( );
  glm::mat4 projectionViewModel = glm::mat4( );

  for ( auto &upCube : cubes )
  {
    projectionViewModel = projectionView * upCube->getTransformationMatrix( );

    shg::OpenGLHelper::setMatrixUniform(
                                        glIds_.program,
                                        "projectionViewModel",
                                        glm::value_ptr( projectionViewModel )
                                        );

    shg::OpenGLHelper::setFloatUniform(
                                       glIds_.program,
                                       "color",
                                       glm::value_ptr( color ),
                                       3
                                       );

    shg::OpenGLHelper::renderBuffer(
                                    glIds_.vao,
                                    0,
                                    15,
                                    GL_TRIANGLE_STRIP,
                                    glIds_.ibo
                                    );
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
