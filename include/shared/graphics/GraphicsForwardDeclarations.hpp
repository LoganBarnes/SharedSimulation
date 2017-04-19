// GraphicsForwardDeclarations.hpp
#pragma once

#include <memory>
#include "glad/glad.h"

namespace graphics
{

template< typename T >
class Camera;

template< typename T >
class GlmCamera;

struct VAOElement;

struct VAOSettings;

class OpenGLWrapper;
class VulkanGlfwWrapper;

class GlfwWrapper;
class Callback;

struct StandardPipeline
{
  std::shared_ptr< GLuint > program;
  std::shared_ptr< GLuint > vbo;
  std::shared_ptr< GLuint > ibo;
  std::shared_ptr< GLuint > vao;
  std::shared_ptr< GLuint > texture;
  std::shared_ptr< GLuint > framebuffer;
};

} // namespace graphics
