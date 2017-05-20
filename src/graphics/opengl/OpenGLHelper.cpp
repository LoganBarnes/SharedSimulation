#include "shared/graphics/OpenGLHelper.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdexcept>


namespace shg
{


namespace
{

// http://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
bool ends_with( std::string const &fullString, std::string const &ending )
{
  if( fullString.length( ) >= ending.length( ) )
  {
    return ( 0 == fullString.compare(
                                     fullString.length( ) - ending.length( ),
                                     ending.length( ), 
                                     ending
                                     ) );
  }
  else
  {
    return false;
  }
}

}


////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::setDefaults
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::setDefaults( )
{
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
  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
} // initContext



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::createProgram
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr< GLuint >
OpenGLHelper::createProgram( const ShaderVec shaders )
{
  IdVec shaderIds;

  for ( auto & shader : shaders )
  {
    shaderIds.emplace_back( OpenGLHelper::_createShader( 
                                                        shader.first, 
                                                        shader.second
                                                        ) );
  }

  return OpenGLHelper::_createProgram( shaderIds );
}



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::addTextureArray
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr< GLuint >
OpenGLHelper::createTextureArray(
                                 GLsizei width,      ///<
                                 GLsizei height,     ///<
                                 float  *pArray,     ///<
                                 GLint   filterType, ///<
                                 GLint   wrapType,   ///<
                                 GLint   internalFormat,
                                 GLenum  format
                                 )

{
  std::shared_ptr< GLuint > spTexture(
                                      new GLuint,
                                      [] ( auto pID )
                                      {
                                        glDeleteTextures( 1, pID );
                                        delete pID;
                                      }
                                      );

  glGenTextures( 1, spTexture.get( ) );
  glBindTexture( GL_TEXTURE_2D, *spTexture );

  glTexParameteri( GL_TEXTURE_2D,     GL_TEXTURE_WRAP_S,   wrapType );
  glTexParameteri( GL_TEXTURE_2D,     GL_TEXTURE_WRAP_T,   wrapType );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType );

  glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, pArray );

  return spTexture;
} // addTextureArray



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::createVao
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr< GLuint >
OpenGLHelper::createVao(
                        const std::shared_ptr< GLuint > &spProgram,  ///<
                        const std::shared_ptr< GLuint > &spVbo,      ///<
                        const GLsizei                   totalStride, ///<
                        const std::vector< VAOElement > &elements    ///<
                        )
{
  std::shared_ptr< GLuint > spVao( new GLuint,
                                  [] ( auto pID )
                                  {
                                    glDeleteVertexArrays( 1, pID );
                                    delete pID;
                                  } );

  //
  // Initialize the vertex array object
  //
  glGenVertexArrays( 1, spVao.get( ) );
  glBindVertexArray( *spVao );

  //
  // bind buffer and save program id for loop
  //
  glBindBuffer( GL_ARRAY_BUFFER, *spVbo );

  //
  // iteratoe through all elements
  //
  for ( const auto &vaoElmt : elements )
  {
    int pos = glGetAttribLocation( *spProgram, vaoElmt.name.c_str( ) );

    if ( pos < 0 )
    {
      std::stringstream msg;
      msg << "attrib location "
          << vaoElmt.name
          << " not found for program "
          << *spProgram;

      throw std::runtime_error( msg.str( ) );
    }

    GLuint position = static_cast< GLuint >( pos );

    glEnableVertexAttribArray( position );
    glVertexAttribPointer(
                          position,
                          vaoElmt.size,     // Num coordinates per position
                          vaoElmt.type,     // Type
                          GL_FALSE,         // Normalized
                          totalStride,      // Stride, 0 = tightly packed
                          vaoElmt.pointer   // Array buffer offset
                          );
  }

  // Unbind buffers.
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );

  return spVao;
} // createVao



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::createFramebuffer
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr< GLuint >
OpenGLHelper::createFramebuffer(
                                GLsizei                         width,      ///<
                                GLsizei                         height,     ///<
                                const std::shared_ptr< GLuint > spColorTex, ///<
                                const std::shared_ptr< GLuint > spDepthTex  ///<
                                )
{
  std::shared_ptr< GLuint > spRbo = nullptr;
  std::shared_ptr< GLuint > spFbo = nullptr;

  //
  // no depth texture; create a renderbuffer
  //
  if ( !spDepthTex )
  {
    spRbo = std::shared_ptr< GLuint >( new GLuint,
                                      []  ( auto pID )
                                      {
                                        glDeleteRenderbuffers( 1, pID );
                                        delete pID;
                                      }
                                      );
  }

  spFbo = std::shared_ptr< GLuint >( new GLuint,
                                    [ spRbo ] ( auto pID )
                                    {
                                      glDeleteFramebuffers( 1, pID );
                                      delete pID;
                                    }
                                    );


  glGenFramebuffers( 1, spFbo.get( ) );
  glBindFramebuffer( GL_FRAMEBUFFER, *spFbo );

  //
  // set color attachment if there is one
  //
  if ( spColorTex )
  {
    glBindTexture( GL_TEXTURE_2D, *spColorTex );

    glFramebufferTexture2D(
                           GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           *spColorTex,
                           0
                           );
  }
  else // no color attachment
  {
    glDrawBuffer( GL_NONE ); // No color buffer is drawn to
    glReadBuffer( GL_NONE ); // No color buffer is read to
  }

  if ( spDepthTex )
  {
    glBindTexture( GL_TEXTURE_2D, *spDepthTex );

    glFramebufferTexture2D(
                           GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           *spDepthTex,
                           0
                           );
  }
  else
  {
    glGenRenderbuffers( 1, spRbo.get( ) );
    glBindRenderbuffer( GL_RENDERBUFFER, *spRbo );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height );
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );

    // attach a renderbuffer to depth attachment point
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *spRbo );
  }


  // Check the framebuffer is ok
  if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
  {
    throw std::runtime_error( "Framebuffer creation failed" );
  }

  glBindFramebuffer( GL_FRAMEBUFFER, 0 );

  return spFbo;
} // createFramebuffer



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::bindFramebuffer
/// \param spFbo
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::bindFramebuffer( )
{
  glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::bindFramebuffer
/// \param spFbo
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::bindFramebuffer( const std::shared_ptr< GLuint > &spFbo )
{
  glBindFramebuffer( GL_FRAMEBUFFER, *spFbo );
}



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::clearFramebuffer
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::clearFramebuffer( )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
} // clearWindow



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::setTextureUniform
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::setTextureUniform(
                                const std::shared_ptr< GLuint > &spProgram, ///<
                                const std::string               uniform,    ///<
                                const std::shared_ptr< GLuint > &spTexture, ///<
                                int                             activeTex   ///<
                                )
{
  glActiveTexture( static_cast< GLenum >( GL_TEXTURE0 + activeTex ) );
  glUniform1i( glGetUniformLocation( *spProgram, uniform.c_str( ) ), activeTex );
  glBindTexture( GL_TEXTURE_2D, *spTexture );
}



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::setIntUniform
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::setIntUniform(
                            const std::shared_ptr< GLuint > &spProgram, ///<
                            const std::string               uniform,    ///<
                            const int                      *pValue,     ///<
                            const int                       size,       ///<
                            const int                       count       ///<
                            )
{
  switch ( size )
  {

  case 1:
    glUniform1i( glGetUniformLocation( *spProgram, uniform.c_str( ) ), *pValue );
    break;

  case 2:
    glUniform2iv( glGetUniformLocation( *spProgram, uniform.c_str( ) ), count, pValue );
    break;

  case 3:
    glUniform3iv( glGetUniformLocation( *spProgram, uniform.c_str( ) ), count, pValue );
    break;

  case 4:
    glUniform4iv( glGetUniformLocation( *spProgram, uniform.c_str( ) ), count, pValue );
    break;

  default:
    std::stringstream msg;
    msg << "Int or vector of size " << size << " does not exist";
    throw std::runtime_error( msg.str( ) );
    break;

  } // switch

} // setIntUniform



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::setFloatUniform
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::setFloatUniform(
                              const std::shared_ptr< GLuint > &spProgram, ///<
                              const std::string               uniform,    ///<
                              const float                    *pValue,     ///<
                              const int                       size,       ///<
                              const int                       count       ///<
                              )
{
  switch ( size )
  {

  case 1:
    glUniform1f( glGetUniformLocation( *spProgram, uniform.c_str( ) ), *pValue );
    break;

  case 2:
    glUniform2fv( glGetUniformLocation( *spProgram, uniform.c_str( ) ), count, pValue );
    break;

  case 3:
    glUniform3fv( glGetUniformLocation( *spProgram, uniform.c_str( ) ), count, pValue );
    break;

  case 4:
    glUniform4fv( glGetUniformLocation( *spProgram, uniform.c_str( ) ), count, pValue );
    break;

  default:
    std::stringstream msg;
    msg << "Float or vector of size " << size << " does not exist";
    throw std::runtime_error( msg.str( ) );
    break;

  } // switch

} // setFloatUniform



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::setMatrixUniform
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::setMatrixUniform(
                               const std::shared_ptr< GLuint > &spProgram, ///<
                               const std::string               uniform,    ///<
                               const float                    *pValue,     ///<
                               const int                       size,       ///<
                               const int                       count       ///<
                               )
{
  switch ( size )
  {
  case 2:
    glUniformMatrix2fv(
                       glGetUniformLocation( *spProgram, uniform.c_str( ) ),
                       count,
                       GL_FALSE,
                       pValue
                       );
    break;

  case 3:
    glUniformMatrix3fv(
                       glGetUniformLocation( *spProgram, uniform.c_str( ) ),
                       count,
                       GL_FALSE,
                       pValue
                       );
    break;


  case 4:
    glUniformMatrix4fv(
                       glGetUniformLocation( *spProgram, uniform.c_str( ) ),
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

} // OpenGLHelper::setMatrixUniform



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::renderBuffer
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
void
OpenGLHelper::renderBuffer(
                           const std::shared_ptr< GLuint > &spVao,
                           const int                       start,
                           const int                       verts,
                           const GLenum                    mode,
                           const std::shared_ptr< GLuint > &spIbo,
                           const void                     *pOffset,
                           const GLenum                    iboType
                           )
{
  glBindVertexArray( *spVao );

  if ( spIbo )
  {
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *spIbo );
    glDrawElements( mode, verts, iboType, pOffset );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  }
  else
  {
    glDrawArrays( mode, start, verts );
  }

  glBindVertexArray( 0 );
} // OpenGLHelper::renderBuffer



//void
//OpenGLHelper::setBlending( bool blend )
//{
//  if ( blend )
//  {
//    glEnable( GL_BLEND );
//    glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
//    glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO );
//  }
//  else
//  {
//    glDisable( GL_BLEND );
//  }
//}



//void
//OpenGLHelper::bindBufferToTexture(
//                                  const std::string texture,
//                                  GLuint            bufId,
//                                  int               alignment,
//                                  int               width,
//                                  int               height
//                                  )
//{
//  glBindTexture( GL_TEXTURE_2D, getTexture( texture ) );
//  glBindBuffer ( GL_PIXEL_UNPACK_BUFFER, bufId );

//  glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );

//  glTexImage2D(
//               GL_TEXTURE_2D,
//               0,
//               GL_RGBA32F,
//               width,
//               height,
//               0,
//               GL_RGBA,
//               GL_FLOAT,
//               0
//               );

//  glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 );

//} // OpenGLHelper::bindBufferToTexture



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::_readFile
///
///        http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
///
/// \param filePath
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
std::string
OpenGLHelper::_readFile( const std::string filePath )
{
  std::ifstream file( filePath, std::ios::in );

  if ( !file.is_open( ) || !file.good( ) )
  {
    throw std::runtime_error( "Could not read file: " + filePath );
  }

  // get full file size
  file.seekg( 0, std::ios::end );
  size_t size = file.tellg( );

  // allocate string of correct size
  std::string buffer( size, ' ' );

  // fill string with file contents
  // note: string memory is only guaranteed contiguous in C++11 and up
  file.seekg(0);
  file.read( &buffer[0], size );

  file.close( );

  return buffer;
} // OpenGLHelper::_readFile



std::shared_ptr< GLuint > 
OpenGLHelper::_createShader(
                            GLenum            shaderType,
                            const std::string filePath
                            )
{
  std::shared_ptr< GLuint > upShader( new GLuint,
                                     [ ] ( auto pID )
    {
      glDeleteShader( *pID );
      delete pID;
    } );

  GLuint shader = glCreateShader( shaderType );
  *upShader = shader;

  // Load shader
  std::string shaderStr    = _readFile( filePath );
  const char *shaderSource = shaderStr.c_str();

  // Compile shader
  glShaderSource( shader, 1, &shaderSource, nullptr );
  glCompileShader( shader );

  // Check shader
  GLint result = GL_FALSE;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &result );

  if ( result == GL_FALSE )
  {
    int logLength = 0;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
    std::vector< char > shaderError( static_cast< size_t >( logLength ) );
    glGetShaderInfoLog( shader, logLength, nullptr, shaderError.data( ) );

    // shader will get deleted when shared_ptr goes out of scope

    throw std::runtime_error( "(Shader) " + std::string( shaderError.data( ) ) );
  }
  return upShader;
}


std::shared_ptr< GLuint > 
OpenGLHelper::_createShader( const std::string filePath )
{
  size_t dot = filePath.find_last_of( "." );

  GLenum shaderType;
  if( ends_with( filePath, ".vert" ) )
  {
    shaderType = GL_VERTEX_SHADER;
  }
  else
  {
    throw std::runtime_error( "Unknown shader extension: " );
  }
  return OpenGLHelper::_createShader( shaderType, filePath );
}


void 
OpenGLHelper::_createShader( 
                            IdVec &ids, 
                            GLenum shaderType, 
                            const std::string filePath 
                            )
{
  ids.emplace_back( OpenGLHelper::_createShader( shaderType, filePath ) );
}


std::shared_ptr< GLuint > 
OpenGLHelper::_createProgram( const IdVec shaderIds )
{
  std::shared_ptr< GLuint > upProgram( new GLuint,
                                      [shaderIds] ( auto pID )
    {
      for ( auto &upShader : shaderIds )
      {
        glDeleteShader( *upShader );
      }
      glDeleteProgram( *pID );
      delete pID;
    } );

  // Create and link program
  GLuint program = glCreateProgram( );
  *upProgram = program;

  for ( auto &upShader : shaderIds )
  {
    glAttachShader( program, *upShader );
  }

  glLinkProgram( program );

  // Check program
  GLint result = GL_FALSE;
  glGetProgramiv( program, GL_LINK_STATUS, &result );

  if ( result == GL_FALSE )
  {
    int logLength = 0;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
    std::vector< char > programError( static_cast< size_t >( logLength ) );
    glGetProgramInfoLog( program, logLength, NULL, programError.data( ) );

    // shaders and programs get deleted with shared_ptr goes out of scope

    throw std::runtime_error( "(Program) " + std::string( programError.data( ) ) );
  }

  for ( auto &upShader : shaderIds )
  {
    glDetachShader( program, *upShader );
  }

  return upProgram;
}

} // namespace shg
