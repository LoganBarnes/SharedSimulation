// GraphicsForwardDeclarations.hpp
#pragma once

#include <glad/glad.h>
#include <memory>

namespace shg
{

template< typename T >
class TCamera;
template< typename T >
class TCameraMover;

using Camera       = TCamera< float >;
using CameraD      = TCamera< double >;
using CameraMover  = TCameraMover< float >;
using CameraMoverD = TCameraMover< double >;

struct VAOElement;

struct VAOSettings;

class OpenGLWrapper;
class VulkanGlfwWrapper;
class OpenGLHelper;

class GlfwWrapper;
class Callback;

///
/// \brief The StandardPipeline struct
///
struct StandardPipeline
{
  std::shared_ptr< GLuint > program;
  std::shared_ptr< GLuint > vbo;
  std::shared_ptr< GLuint > ibo;
  std::shared_ptr< GLuint > vao;
  std::shared_ptr< GLuint > texture;
  std::shared_ptr< GLuint > framebuffer;
};

} // namespace shg
