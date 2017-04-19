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

struct StandardIds
{
  std::shared_ptr< GLuint > upProgram;
  std::shared_ptr< GLuint > upVbo;
  std::shared_ptr< GLuint > upIbo;
  std::shared_ptr< GLuint > upVao;
  std::shared_ptr< GLuint > upTexture;
  std::shared_ptr< GLuint > upFramebuffer;
};

} // namespace graphics
