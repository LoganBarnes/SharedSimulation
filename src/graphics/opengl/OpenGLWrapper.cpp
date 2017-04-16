#include "shared/graphics/OpenGLWrapper.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdexcept>

//#include "graphics/glfw/CallbackSingleton.hpp"


namespace graphics
{


///
/// \brief OpenGLWrapper::OpenGLWrapper
///
OpenGLWrapper::OpenGLWrapper( )
  : viewportWidth_( 0 )
  , viewportHeight_( 0 )
  , pContext_( nullptr )
{}



///
/// \brief OpenGLWrapper::~OpenGLWrapper
///
OpenGLWrapper::~OpenGLWrapper( )
{
  for ( auto &it : programs_ )
  {
    glDeleteProgram( it.second );
  }

  for ( auto &it : textures_ )
  {
    glDeleteTextures( 1, &( it.second ) );
  }

  for ( auto &it : buffers_ )
  {
    glDeleteBuffers( 1, &( it.second.vbo ) );
  }

  for ( auto &vaos : vaoMap_ )
  {
    for ( auto &it : vaos.second )
    {
      glDeleteVertexArrays( 1, &it.second );
    }
  }

  for ( auto &it : framebuffers_ )
  {
    const FrameBuffer &buffer = it.second;
    glDeleteFramebuffers( 1, &( buffer.fbo ) );
    glDeleteRenderbuffers( 1, &( buffer.rbo ) );
  }
}



///
/// \brief OpenGLWrapper::initContext
/// \param width
/// \param height
///
void
OpenGLWrapper::setDefaults(
                           GLsizei width,
                           GLsizei height
                           )
{
  viewportWidth_  = width;
  viewportHeight_ = height;

  // Enable depth testing, so that objects are occluded based on depth instead of drawing order.
  glEnable( GL_DEPTH_TEST );

  // Move the polygons back a bit so lines are still drawn even though they are coplanar with the
  // polygons they came from, which will be drawn before them.
  glEnable( GL_POLYGON_OFFSET_LINE );
  glPolygonOffset( -1, -1 );

  // Enable back-face culling, meaning only the front side of every face is rendered.
  glEnable( GL_CULL_FACE );
  glCullFace( GL_BACK );

  // Specify that the front face is represented by vertices in counterclockwise order (this is
  // the default).
  glFrontFace( GL_CCW );

  // Specify the color used when glClear is called
  setClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
} // initContext



// getters
GLuint
OpenGLWrapper::getTexture( const std::string name )
{
  _checkItemExists( name, textures_, "texture" );
  return textures_[ name ];
}



void
OpenGLWrapper::setTexture(
                          const std::string name,
                          const GLuint      id,
                          const bool        forceOverride
                          )
{
  _replaceItemId(
                 name,
                 textures_,
                 id,
                 "texture",
                 forceOverride,
                 [ this, &name ]( )
    {
      glDeleteTextures( 1, &( this->textures_[ name ] ) );
    }
                 );
}



GLuint
OpenGLWrapper::getBuffer( const std::string name )
{
  _checkItemExists( name, buffers_, "buffer" );
  return buffers_[ name ].vbo;
}



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
  programs_[ name ] = OpenGLWrapper::_loadShader( vertFilePath, fragFilePath );
}



void
OpenGLWrapper::addTextureArray(
                               const std::string name,
                               GLsizei           width,
                               GLsizei           height,
                               float            *pArray,
                               bool              linear
                               )

{

  if ( textures_.find( name ) != textures_.end( ) )
  {
    glDeleteTextures( 1, &( textures_[ name ] ) );
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

  textures_[ name ] = texture;

} // addTextureArray



void
OpenGLWrapper::addTextureImage(
                               const std::string name,
                               GLsizei           width,
                               GLsizei           height,
                               const std::string
                               )
{
  if ( textures_.find( name ) != textures_.end( ) )
  {
    glDeleteTextures( 1, &( textures_[ name ] ) );
  }

  GLuint texture;
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,      GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,      GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL );

  textures_[ name ] = texture;

} // addTextureImage



GLuint
OpenGLWrapper::_addVAOToBuffer(
                               const GLuint       vbo,
                               const VAOSettings &settings
                               ) const
{
  GLuint vao;

  glBindBuffer( GL_ARRAY_BUFFER, vbo );

  // Initialize the vertex array object.
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  _checkItemExists( settings.program, programs_, "programs" );

  const GLuint &program = programs_.at( settings.program );

  for ( size_t i = 0; i < settings.settings.size( ); ++i )
  {
    const VAOElement &vaoElmt = settings.settings[ i ];
    int pos                   = glGetAttribLocation( program, vaoElmt.name.c_str( ) );

    if ( pos < 0 )
    {
      std::stringstream msg;
      msg << "attrib location "
          << vaoElmt.name
          << " not found for program "
          << settings.program;

      throw std::runtime_error( msg.str( ) );
    }

    GLuint position = static_cast< GLuint >( pos );

    glEnableVertexAttribArray( position );
    glVertexAttribPointer(
                          position,
                          vaoElmt.size,         // Num coordinates per position
                          vaoElmt.type,         // Type
                          GL_FALSE,             // Normalized
                          settings.totalStride, // Stride, 0 = tightly packed
                          vaoElmt.pointer       // Array buffer offset
                          );
  }

  // Unbind buffers.
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );

  return vao;

} // OpenGLWrapper::addVAOToBuffer



GLuint
OpenGLWrapper::_getVAO( const Buffer &buf )
{
  ContextVAOMap &vaos = vaoMap_[ buf.vbo ];

  if ( vaos.find( pContext_ ) == vaos.end( ) )
  {
    vaos[ pContext_ ] = _addVAOToBuffer( buf.vbo, buf.settings );
  }

  return vaos[ pContext_ ];

}



void
OpenGLWrapper::addFramebuffer(
                              const std::string buffer,
                              GLsizei           width,
                              GLsizei           height,
                              const std::string texture
                              )

{
  _checkItemExists( texture, textures_, "textures" );

  if ( framebuffers_.find( buffer ) != framebuffers_.end( ) )
  {
    FrameBuffer &buf = framebuffers_[ buffer ];
    glDeleteFramebuffers( 1, &buf.fbo );
    glDeleteRenderbuffers( 1, &buf.rbo );
  }

  FrameBuffer &buf = framebuffers_[ buffer ];

  glGenFramebuffers( 1, &buf.fbo );
  glBindFramebuffer( GL_FRAMEBUFFER, buf.fbo );

  glGenRenderbuffers( 1, &buf.rbo );
  glBindRenderbuffer( GL_RENDERBUFFER, buf.rbo );
  glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height );
  glBindRenderbuffer( GL_RENDERBUFFER, 0 );

  // attach a texture to FBO color attachment point
  glFramebufferTexture2D( GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         textures_[ texture ],
                         0 );

  // attach a renderbuffer to depth attachment point
  glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buf.rbo );

  glBindFramebuffer( GL_FRAMEBUFFER, 0 );

} // OpenGLWrapper::addFramebuffer



void
OpenGLWrapper::bindFramebuffer( const std::string name )
{
  if ( name.length( ) == 0 )
  {
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    return;
  }

  _checkItemExists( name, framebuffers_, "framebuffers" );
  glBindFramebuffer( GL_FRAMEBUFFER, framebuffers_.at( name ).fbo );
}



void
OpenGLWrapper::swapFramebuffers(
                                const std::string fbo1,
                                const std::string fbo2
                                )
{
  _checkItemExists( fbo1, framebuffers_, "framebuffer" );
  _checkItemExists( fbo2, framebuffers_, "framebuffer" );

  FrameBuffer temp = framebuffers_[ fbo1 ];

  framebuffers_[ fbo1 ] = framebuffers_[ fbo2 ];
  framebuffers_[ fbo2 ] = temp;
}



void
OpenGLWrapper::clearWindow(
                           GLsizei width,
                           GLsizei height
                           )
{
  clearWindow( 0, 0, width, height );
}



void
OpenGLWrapper::clearWindow(
                           GLint   x,
                           GLint   y,
                           GLsizei width,
                           GLsizei height
                           )
{
  GLsizei w = viewportWidth_;
  GLsizei h = viewportHeight_;

  if ( width > 0 )
  {
    w = width;
  }

  if ( height > 0 )
  {
    h = height;
  }

  glViewport( x, y, w, h );

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

} // clearWindow



void
OpenGLWrapper::useProgram( const std::string program )
{
  _checkItemExists( program, programs_, "programs" );
  glUseProgram( programs_[ program ] );
}



void
OpenGLWrapper::setTextureUniform(
                                 const std::string program,
                                 const std::string uniform,
                                 const std::string texture,
                                 int               activeTex
                                 )
{
  _checkItemExists( program, programs_, "programs" );
  _checkItemExists( texture, textures_, "textures" );

  switch ( activeTex )
  {
  case 0:
    glActiveTexture( GL_TEXTURE0 );
    break;

  case 1:
    glActiveTexture( GL_TEXTURE1 );
    break;

  case 2:
    glActiveTexture( GL_TEXTURE2 );
    break;

  default:
    glActiveTexture( GL_TEXTURE3 );
    break;
  } // switch

  glUniform1i( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), activeTex );
  glBindTexture( GL_TEXTURE_2D, textures_[ texture ] );
} // OpenGLWrapper::setTextureUniform



void
OpenGLWrapper::renderBuffer(
                            const std::string buffer,
                            const int         start,
                            const int         verts,
                            const GLenum      mode,
                            const std::string ibo,
                            const void       *pOffset,
                            const GLenum      iboType
                            )
{
  const bool usingIBO = ibo.length( ) > 0;

  _checkItemExists( buffer, buffers_, "buffer" );

  GLuint vao = _getVAO( buffers_[ buffer ] );

  glBindVertexArray( vao );


  if ( usingIBO )
  {
    _checkItemExists( ibo, indexBuffers_, "indexBuffer" );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffers_[ ibo ] );
    glDrawElements( mode, verts, iboType, pOffset );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  }
  else
  {
    glDrawArrays( mode, start, verts );
  }

  glBindVertexArray( 0 );
} // OpenGLWrapper::renderBuffer



void
OpenGLWrapper::setBoolUniform(
                              const std::string program,
                              const std::string uniform,
                              bool              var
                              )
{
  _checkItemExists( program, programs_, "programs" );
  glUniform1i( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), var );
}



void
OpenGLWrapper::setIntUniform(
                             const std::string program,
                             const std::string uniform,
                             int               value
                             )
{
  _checkItemExists( program, programs_, "programs" );
  glUniform1i( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), value );
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
  _checkItemExists( program, programs_, "programs" );

  switch ( size )
  {

  case 1:
    glUniform1f( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), *pValue );
    break;

  case 2:
    glUniform2fv( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), count, pValue );
    break;

  case 3:
    glUniform3fv( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), count, pValue );
    break;

  case 4:
    glUniform4fv( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), count, pValue );
    break;

  default:
    std::stringstream msg;
    msg << "Float or vector of size " << size << " does not exist";
    throw std::runtime_error( msg.str( ) );
    break;

  } // switch

} // setFloatUniform



void
OpenGLWrapper::setMatrixUniform(
                                const std::string program,
                                const std::string uniform,
                                const float      *pValue,
                                const int         size,
                                const int         count
                                )
{
  _checkItemExists( program, programs_, "programs" );

  switch ( size )
  {
  case 2:
    glUniformMatrix2fv(
                       glGetUniformLocation( programs_[ program ], uniform.c_str( ) ),
                       count,
                       GL_FALSE,
                       pValue
                       );
    break;

  case 3:
    glUniformMatrix3fv(
                       glGetUniformLocation( programs_[ program ], uniform.c_str( ) ),
                       count,
                       GL_FALSE,
                       pValue
                       );
    break;


  case 4:
    glUniformMatrix4fv(
                       glGetUniformLocation( programs_[ program ], uniform.c_str( ) ),
                       count,
                       GL_FALSE,
                       pValue
                       );
    break;

  default:
    std::stringstream msg;
    msg << "Matrix of size " << size << " does not exist";
    throw std::runtime_error( msg.str( ) );
    break;
  } // switch

} // OpenGLWrapper::setMatrixUniform



void
OpenGLWrapper::swapTextures(
                            const std::string tex1,
                            const std::string tex2
                            )
{
  _checkItemExists( tex1, textures_, "textures" );
  _checkItemExists( tex2, textures_, "textures" );

  GLuint temp = textures_[ tex1 ];

  textures_[ tex1 ] = textures_[ tex2 ];
  textures_[ tex2 ] = temp;
}



void
OpenGLWrapper::setBlending( bool blend )
{
  if ( blend )
  {
    glEnable( GL_BLEND );
    glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
    glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );
  }
  else
  {
    glDisable( GL_BLEND );
  }
}



void
OpenGLWrapper::setViewportSize(
                               GLsizei width,
                               GLsizei height
                               )
{
  viewportWidth_  = width;
  viewportHeight_ = height;
}



void
OpenGLWrapper::bindBufferToTexture(
                                   const std::string texture,
                                   GLuint            bufId,
                                   int               alignment,
                                   int               width,
                                   int               height
                                   )
{
  glBindTexture( GL_TEXTURE_2D, getTexture( texture ) );
  glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, bufId );

  glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );

  glTexImage2D(
               GL_TEXTURE_2D,
               0,
               GL_RGBA32F,
               width,
               height,
               0,
               GL_RGBA,
               GL_FLOAT,
               0
               );

  glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );

} // OpenGLWrapper::bindBufferToTexture



void
OpenGLWrapper::setClearColor(
                             const float r,
                             const float g,
                             const float b,
                             const float a
                             )
{
  glClearColor( r, g, b, a );
}



void
OpenGLWrapper::destroyTexture(
                              const std::string name,
                              const bool        glDelete
                              )
{
  _checkItemExists( name, textures_, "textures" );

  if ( glDelete )
  {
    glDeleteTextures( 1, &( textures_.at( name ) ) );
  }

  textures_.erase( name );
}



void
OpenGLWrapper::destroyFramebuffer( const std::string name )
{
  _checkItemExists( name, framebuffers_, "framebuffers" );

  const FrameBuffer &buffer = framebuffers_.at( name );
  glDeleteFramebuffers( 1, &( buffer.fbo ) );
  glDeleteRenderbuffers( 1, &( buffer.rbo ) );

  framebuffers_.erase( name );
}



std::string
OpenGLWrapper::_readFile( const std::string filePath )
{
  std::string content;
  std::ifstream fileStream( filePath, std::ios::in );

  if ( !fileStream.is_open( ) )
  {
    std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
    return "";
  }

  std::string line = "";

  while ( !fileStream.eof( ) )
  {
    std::getline( fileStream, line );
    content.append( line + "\n" );
  }

  fileStream.close( );
  return content;
} // OpenGLWrapper::_readFile



GLuint
OpenGLWrapper::_loadShader(
                           const std::string vertex_path,
                           const std::string fragment_path
                           )
{
  GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
  GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );

  // Read shaders
  std::string vertShaderStr = _readFile( vertex_path );
  std::string fragShaderStr = _readFile( fragment_path );
  const char *vertShaderSrc = vertShaderStr.c_str( );
  const char *fragShaderSrc = fragShaderStr.c_str( );

  GLint result = GL_FALSE;
  int logLength;

  // Compile vertex shader
//  std::cout << "Compiling vertex shader." << std::endl;
  glShaderSource( vertShader, 1, &vertShaderSrc, NULL );
  glCompileShader( vertShader );

  // Check vertex shader
  glGetShaderiv( vertShader, GL_COMPILE_STATUS,  &result );
  glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &logLength );
  std::vector< char > vertShaderError(
                                      ( logLength > 1 ) ?
                                      static_cast< unsigned long >( logLength ) : 1
                                      );
  glGetShaderInfoLog( vertShader, logLength, NULL, &vertShaderError[ 0 ] );

  if ( vertShaderError[ 0 ] != '\0' )
  {
    std::cout << "(Vertex Shader) " << &vertShaderError[ 0 ];
    std::cout << vertex_path << std::endl;
  }

  // Compile fragment shader
//  std::cout << "Compiling fragment shader." << std::endl;
  glShaderSource( fragShader, 1, &fragShaderSrc, NULL );
  glCompileShader( fragShader );

  // Check fragment shader
  glGetShaderiv( fragShader, GL_COMPILE_STATUS,  &result );
  glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &logLength );
  std::vector< char > fragShaderError(
                                      ( logLength > 1 ) ?
                                      static_cast< unsigned long >( logLength ) : 1
                                      );
  glGetShaderInfoLog( fragShader, logLength, NULL, &fragShaderError[ 0 ] );

  if ( fragShaderError[ 0 ] != '\0' )
  {
    std::cout << "(Fragment Shader) " << &fragShaderError[ 0 ];
    std::cout << fragment_path << std::endl;
  }

//  std::cout << "Linking program" << std::endl;
  GLuint program = glCreateProgram( );
  glAttachShader( program, vertShader );
  glAttachShader( program, fragShader );
  glLinkProgram( program );

  glGetProgramiv( program, GL_LINK_STATUS,     &result );
  glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
  std::vector< char > programError(
                                   ( logLength > 1 ) ?
                                   static_cast< unsigned long >( logLength ) : 1
                                   );
  glGetProgramInfoLog( program, logLength, NULL, &programError[ 0 ] );

  if ( programError[ 0 ] != '\0' )
  {
    std::cout << "(Shader Program) " << &programError[ 0 ];
    std::cout << vertex_path << "\n" << fragment_path << std::endl;
  }

  glDeleteShader( vertShader );
  glDeleteShader( fragShader );

  return program;
} // OpenGLWrapper::_loadShader



} // namespace graphics
