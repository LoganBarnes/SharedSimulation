#include "shared/graphics/OpenGLHelper.hpp"

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
/// \brief OpenGLHelper::addProgram
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr< GLuint >
OpenGLHelper::createProgram(
                            const std::string vertFilePath, ///<
                            const std::string fragFilePath ///<
                            )

{
  return OpenGLHelper::_loadShader( vertFilePath, fragFilePath );
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
                                 bool /*mipmap*/      ///<
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

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, pArray );

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
                                GLsizei                         width,  ///<
                                GLsizei                         height, ///<
                                const std::shared_ptr< GLuint > &spTex, ///<
                                std::shared_ptr< GLuint >      *pRbo    ///<
                                )
{
  std::shared_ptr< GLuint > spRbo( new GLuint,
                                  [] ( auto pID )
                                  {
                                    glDeleteRenderbuffers( 1, pID );
                                    delete pID;
                                  } );

  std::shared_ptr< GLuint > spFbo( new GLuint,
                                  [ spRbo ]( auto pID )
                                  {
                                    glDeleteFramebuffers( 1, pID );
                                    delete pID;
                                  }
                                  );

  glGenFramebuffers( 1, spFbo.get( ) );
  glBindFramebuffer( GL_FRAMEBUFFER, *spFbo );

  glGenRenderbuffers( 1, spRbo.get( ) );
  glBindRenderbuffer( GL_RENDERBUFFER, *spRbo );
  glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height );
  glBindRenderbuffer( GL_RENDERBUFFER, 0 );

  // attach a texture to FBO color attachment point
  glFramebufferTexture2D(
                         GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         GL_TEXTURE_2D,
                         *spTex,
                         0
                         );


  // attach a renderbuffer to depth attachment point
  glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *spRbo );

  glBindFramebuffer( GL_FRAMEBUFFER, 0 );

  if ( pRbo )
  {
    *pRbo = spRbo;
  }

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
  glActiveTexture( GL_TEXTURE0 + activeTex );
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
                           const std::shared_ptr< GLuint > spIbo,
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
/// \param filePath
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
std::string
OpenGLHelper::_readFile( const std::string filePath )
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
} // OpenGLHelper::_readFile



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::_loadShader
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr< GLuint >
OpenGLHelper::_loadShader(
                          const std::string vertex_path,  ///<
                          const std::string fragment_path ///<
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
  glGetShaderiv( vertShader,  GL_COMPILE_STATUS, &result );
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
  glGetShaderiv( fragShader,  GL_COMPILE_STATUS, &result );
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

  glGetProgramiv( program,     GL_LINK_STATUS, &result );
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

  return std::shared_ptr< GLuint >( new GLuint( program ),
                                   [] ( auto pID )
                                   {
                                     glDeleteProgram( *pID );
                                     delete pID;
                                   } );
} // OpenGLHelper::_loadShader



} // namespace graphics
