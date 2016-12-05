#include "OpenGLWrapper.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "glad/glad.h"
#include "graphics/glfw/CallbackSingleton.hpp"


namespace graphics
{


OpenGLWrapper::OpenGLWrapper(
                             GLsizei width,
                             GLsizei height
                             )
  : m_viewportWidth( width )
  , m_viewportHeight( height )
  , m_input( NULL )
  , m_initialized( false )
{

  m_input = &CallbackSingleton::getInstance( );

}


OpenGLWrapper::~OpenGLWrapper( )
{

  for ( auto it = m_programs.begin( ); it != m_programs.end( ); ++it )
  {

    glDeleteProgram( it->second );

  }


  for ( auto it = m_textures.begin( ); it != m_textures.end( ); ++it )
  {

    glDeleteTextures( 1, &( it->second ) );

  }

  for ( auto it = m_buffers.begin( ); it != m_buffers.end( ); ++it )
  {

    const Buffer &buffer = it->second;
    glDeleteBuffers( 1, &( buffer.vbo ) );
    glDeleteVertexArrays( 1, &( buffer.vao ) );

  }

  for ( auto it = m_framebuffers.begin( ); it != m_framebuffers.end( ); ++it )
  {

    const Buffer &buffer = it->second;
    glDeleteFramebuffers( 1, &( buffer.vbo ) );
    glDeleteRenderbuffers( 1, &( buffer.vao ) );

  }

}


///
/// \brief OpenGLWrapper::init
///
void
OpenGLWrapper::init( )
{

  // Enable depth testing, so that objects are occluded based on depth instead of drawing order.
  glEnable( GL_DEPTH_TEST );

  // Move the polygons back a bit so lines are still drawn even though they are coplanar with the
  // polygons they came from, which will be drawn before them.
  glEnable( GL_POLYGON_OFFSET_LINE );
  glPolygonOffset( -1, -1 );

//  // Enable back-face culling, meaning only the front side of every face is rendered.
//  glEnable( GL_CULL_FACE );
//  glCullFace( GL_BACK );

//  // Specify that the front face is represented by vertices in counterclockwise order (this is
//  // the default).
//  glFrontFace( GL_CCW );

  // Specify the color used when glClear is called
  glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );

} // init



///
/// \brief OpenGLWrapper::addProgram
/// \param name
/// \param vertFilePath
/// \param fragFilePath
///
void
OpenGLWrapper::addProgram(
                          const std::string name,
                          const std::string vertFilePath,
                          const std::string fragFilePath
                          )

{

  m_programs[ name ] = OpenGLWrapper::_loadShader( vertFilePath, fragFilePath );

}



void
OpenGLWrapper::addTextureArray(
                               const std::string name,
                               GLsizei     width,
                               GLsizei     height,
                               float      *pArray,
                               bool        linear
                               )

{

  if ( m_textures.count( name ) )
  {
    glDeleteTextures( 1, &( m_textures[ name ] ) );
  }

  GLuint texture;
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

  if ( linear )
  {
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  }
  else
  {
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  }

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, pArray );

  m_textures[ name ] = texture;

} // addTextureArray



void
OpenGLWrapper::addTextureImage(
                               const std::string name,
                               GLsizei     width,
                               GLsizei     height,
                               const std::string
                               )
{
  if ( m_textures.count( name ) )
  {
    glDeleteTextures( 1, &( m_textures[ name ] ) );
  }

  GLuint texture;
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL );

  m_textures[ name ] = texture;
} // addTextureImage



void
OpenGLWrapper::addUVBuffer(
                           const std::string name,
                           const std::string program,
                           GLfloat    *data,
                           GLuint      size,
                           bool        dynamic
                           )
{
  Buffer buffer;

  if ( m_buffers.count( name ) )
  {
    buffer = m_buffers[ name ];
    glDeleteBuffers( 1, &( buffer.vbo ) );
    glDeleteVertexArrays( 1, &( buffer.vao ) );
  }

  // Initialize the vertex buffer object.
  glGenBuffers( 1, &( buffer.vbo ) );
  glBindBuffer( GL_ARRAY_BUFFER, ( buffer.vbo ) );

  if ( dynamic )
  {
    glBufferData(
                 GL_ARRAY_BUFFER,
                 static_cast< GLsizeiptr >( size * sizeof( GLfloat ) ),
                 data,
                 GL_DYNAMIC_DRAW
                 );
  }
  else
  {
    glBufferData(
                 GL_ARRAY_BUFFER,
                 static_cast< GLsizeiptr >( size * sizeof( GLfloat ) ),
                 data,
                 GL_STATIC_DRAW
                 );
  }


  // Initialize the vertex array object.
  glGenVertexArrays( 1, &( buffer.vao ) );
  glBindVertexArray( ( buffer.vao ) );

  int pos = glGetAttribLocation( m_programs[ program ], "inScreenPos" );

  if ( pos < 0 )
  {
    std::cerr << "attrib location not found for program " << program << std::endl;
    // Unbind buffers and return
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    return;
  }

  GLuint position = static_cast< GLuint >( pos );

  glEnableVertexAttribArray( position );
  glVertexAttribPointer(
                        position,
                        3,        // Num coordinates per position
                        GL_FLOAT, // Type
                        GL_FALSE, // Normalized
                        0,        // Stride, 0 = tightly packed
                        nullptr   // Array buffer offset
                        );

  // Unbind buffers.
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );

  m_buffers[ name ] = buffer;
}



void
OpenGLWrapper::addFramebuffer(
                              const std::string buffer,
                              GLsizei     width,
                              GLsizei     height,
                              const std::string texture
                              )

{
    if ( m_framebuffers.find( buffer ) != m_framebuffers.end( ) )
    {
        Buffer buf = m_framebuffers[buffer];
        glDeleteFramebuffers(1, &buf.vbo);
        glDeleteRenderbuffers(1, &buf.vao);
    }

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach a texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[texture], 0);

    // attach a renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Buffer buf;
    buf.vbo = fbo;
    buf.vao = rbo;
    m_framebuffers[buffer] = buf;
}

void
OpenGLWrapper::bindFramebuffer( const std::string name )
{
  if ( m_framebuffers.find( name ) != m_framebuffers.end( ) )
  {
    glBindFramebuffer( GL_FRAMEBUFFER, m_framebuffers[ name ].vbo );
  }
  else
  {
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  }
}


void OpenGLWrapper::swapFramebuffers(const std::string fbo1, const std::string fbo2)
{
    Buffer temp = m_framebuffers[fbo1];
    m_framebuffers[fbo1] = m_framebuffers[fbo2];
    m_framebuffers[fbo2] = temp;
}


void
OpenGLWrapper::clearWindow(
                           GLsizei width,
                           GLsizei height
                           )
{
  GLsizei w = m_viewportWidth;
  GLsizei h = m_viewportHeight;

  if ( width > 0 )
  {

    w = width;

  }

  if ( height > 0 )
  {

    h = height;

  }

  glViewport( 0, 0, w, h );

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

} // clearWindow


void OpenGLWrapper::useProgram(const std::string program)
{
    glUseProgram(m_programs[program]);
}


void OpenGLWrapper::setTextureUniform(const std::string program, const std::string uniform, const std::string texture, int activeTex)
{
    switch(activeTex)
    {
    case 0:
        glActiveTexture(GL_TEXTURE0);
        break;
    case 1:
        glActiveTexture(GL_TEXTURE1);
        break;
    case 2:
        glActiveTexture(GL_TEXTURE2);
        break;
    default:
        glActiveTexture(GL_TEXTURE3);
        break;
    }
    glUniform1i(glGetUniformLocation( m_programs[program], uniform.c_str() ), activeTex);
    glBindTexture(GL_TEXTURE_2D, m_textures[texture]);
}


void OpenGLWrapper::renderBuffer(const std::string buffer, int verts, GLenum mode)
{
    glBindVertexArray(m_buffers[buffer].vao);
    glDrawArrays(mode, 0, verts);
    glBindVertexArray(0);
}



void OpenGLWrapper::setBuffer(const std::string bufferName, float *data, GLuint size)
{
    Buffer buffer = m_buffers[bufferName];
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBindVertexArray(buffer.vao);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    static_cast< GLsizeiptr>( size * sizeof(float)),
                    data);
}


void OpenGLWrapper::setBoolUniform(const std::string program, const std::string uniform, bool var)
{
    glUniform1i(glGetUniformLocation(m_programs[program], uniform.c_str()), var);
}


void OpenGLWrapper::setIntUniform(const std::string program, const std::string uniform, int value)
{
    glUniform1i(glGetUniformLocation(m_programs[program], uniform.c_str()), value);
}




void
OpenGLWrapper::setFloatUniform(
                               const std::string program,
                               const std::string uniform,
                               const float      *pValue,
                               const int         size,
                               const int         count
                               )
{

  switch ( size )
  {

  case 1:
    glUniform1f( glGetUniformLocation( m_programs[ program ], uniform.c_str( ) ), *pValue );
    break;

  case 2:
    glUniform2fv( glGetUniformLocation( m_programs[ program ], uniform.c_str( ) ), count, pValue );
    break;

  case 3:
    glUniform3fv( glGetUniformLocation( m_programs[ program ], uniform.c_str( ) ), count, pValue );
    break;

  case 4:
    glUniform4fv( glGetUniformLocation( m_programs[ program ], uniform.c_str( ) ), count, pValue );
    break;

  default:
    break;

  } // switch

} // setFloatUniform



void OpenGLWrapper::swapTextures(const std::string tex1, const std::string tex2)
{
    GLuint temp = m_textures[tex1];
    m_textures[tex1] = m_textures[tex2];
    m_textures[tex2] = temp;
}


void OpenGLWrapper::setBlending(bool blend)
{
    if (blend)
    {
        glEnable(GL_BLEND);
        glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    } else
        glDisable(GL_BLEND);
}



void OpenGLWrapper::setViewportSize(GLsizei width, GLsizei height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;
}


std::string OpenGLWrapper::_readFile(const std::string filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


GLuint OpenGLWrapper::_loadShader(const std::string vertex_path, const std::string fragment_path) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    std::string vertShaderStr = _readFile(vertex_path);
    std::string fragShaderStr = _readFile(fragment_path);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile vertex shader
    std::cout << "Compiling vertex shader." << std::endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> vertShaderError((logLength > 1) ? static_cast<unsigned long>(logLength) : 1);
    glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;

    // Compile fragment shader
    std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> fragShaderError((logLength > 1) ? static_cast<unsigned long>(logLength) : 1);
    glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
    std::cout << &fragShaderError[0] << std::endl;

    std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError((logLength > 1) ? static_cast<unsigned long>(logLength) : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}


} // namespace graphics

