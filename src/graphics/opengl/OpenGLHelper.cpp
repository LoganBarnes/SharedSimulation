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
  std::shared_ptr< GLuint > upTexture(
                                      new GLuint,
                                      [] ( auto pID )
                                      {
                                        glDeleteTextures( 1, pID );
                                        delete pID;
                                      }
                                      );

  glGenTextures( 1, upTexture.get( ) );
  glBindTexture( GL_TEXTURE_2D, *upTexture );

  glTexParameteri( GL_TEXTURE_2D,     GL_TEXTURE_WRAP_S,   wrapType );
  glTexParameteri( GL_TEXTURE_2D,     GL_TEXTURE_WRAP_T,   wrapType );

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, pArray );

  return upTexture;
} // addTextureArray



//GLuint
//OpenGLHelper::_addVAOToBuffer(
//                               const GLuint       vbo,
//                               const VAOSettings &settings
//                               ) const
//{
//  GLuint vao;

//  glBindBuffer( GL_ARRAY_BUFFER, vbo );

//  // Initialize the vertex array object.
//  glGenVertexArrays( 1, &vao );
//  glBindVertexArray( vao );

//  _checkItemExists( settings.program, programs_, "programs" );

//  const GLuint &program = programs_.at( settings.program );

//  for ( size_t i = 0; i < settings.settings.size( ); ++i )
//  {
//    const VAOElement &vaoElmt = settings.settings[ i ];
//    int pos                   = glGetAttribLocation( program, vaoElmt.name.c_str( ) );

//    if ( pos < 0 )
//    {
//      std::stringstream msg;
//      msg << "attrib location "
//          << vaoElmt.name
//          << " not found for program "
//          << settings.program;

//      throw std::runtime_error( msg.str( ) );
//    }

//    GLuint position = static_cast< GLuint >( pos );

//    glEnableVertexAttribArray( position );
//    glVertexAttribPointer(
//                          position,
//                          vaoElmt.size,         // Num coordinates per position
//                          vaoElmt.type,         // Type
//                          GL_FALSE,             // Normalized
//                          settings.totalStride, // Stride, 0 = tightly packed
//                          vaoElmt.pointer       // Array buffer offset
//                          );
//  }

//  // Unbind buffers.
//  glBindBuffer( GL_ARRAY_BUFFER, 0 );
//  glBindVertexArray( 0 );

//  return vao;

//} // OpenGLHelper::addVAOToBuffer



//GLuint
//OpenGLHelper::_getVAO( const Buffer &buf )
//{
//  ContextVAOMap &vaos = vaoMap_[ buf.vbo ];

//  if ( vaos.find( pContext_ ) == vaos.end( ) )
//  {
//    vaos[ pContext_ ] = _addVAOToBuffer( buf.vbo, buf.settings );
//  }

//  return vaos[ pContext_ ];

//}



//void
//OpenGLHelper::addFramebuffer(
//                              const std::string buffer,
//                              GLsizei           width,
//                              GLsizei           height,
//                              const std::string texture
//                              )

//{
//  _checkItemExists( texture, textures_, "textures" );

//  if ( framebuffers_.find( buffer ) != framebuffers_.end( ) )
//  {
//    FrameBuffer &buf = framebuffers_[ buffer ];
//    glDeleteFramebuffers( 1, &buf.fbo );
//    glDeleteRenderbuffers( 1, &buf.rbo );
//  }

//  FrameBuffer &buf = framebuffers_[ buffer ];

//  glGenFramebuffers( 1, &buf.fbo );
//  glBindFramebuffer( GL_FRAMEBUFFER, buf.fbo );

//  glGenRenderbuffers( 1, &buf.rbo );
//  glBindRenderbuffer( GL_RENDERBUFFER, buf.rbo );
//  glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height );
//  glBindRenderbuffer( GL_RENDERBUFFER, 0 );

//  // attach a texture to FBO color attachment point
//  glFramebufferTexture2D( GL_FRAMEBUFFER,
//                         GL_COLOR_ATTACHMENT0,
//                         GL_TEXTURE_2D,
//                         textures_[ texture ],
//                         0 );

//  // attach a renderbuffer to depth attachment point
//  glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buf.rbo );

//  glBindFramebuffer( GL_FRAMEBUFFER, 0 );

//} // OpenGLHelper::addFramebuffer



//void
//OpenGLHelper::bindFramebuffer( const std::string name )
//{
//  if ( name.length( ) == 0 )
//  {
//    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
//    return;
//  }

//  _checkItemExists( name, framebuffers_, "framebuffers" );
//  glBindFramebuffer( GL_FRAMEBUFFER, framebuffers_.at( name ).fbo );
//}



void
OpenGLHelper::clearFramebuffer( )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
} // clearWindow



//void
//OpenGLHelper::setTextureUniform(
//                                const GLuint      program,
//                                const std::string uniform,
//                                const GLuint      texture,
//                                int               activeTex
//                                )
//{
//  glActiveTexture( GL_TEXTURE0 + activeTex );
//  glUniform1i( glGetUniformLocation( program, uniform.c_str( ) ), activeTex );
//  glBindTexture( GL_TEXTURE_2D, texture );
//} // OpenGLHelper::setTextureUniform



//void
//OpenGLHelper::renderBuffer(
//                           const std::string buffer,
//                           const int         start,
//                           const int         verts,
//                           const GLenum      mode,
//                           const std::string ibo,
//                           const void       *pOffset,
//                           const GLenum      iboType
//                           )
//{
//  const bool usingIBO = ibo.length( ) > 0;

//  _checkItemExists( buffer, buffers_, "buffer" );

//  GLuint vao = _getVAO( buffers_[ buffer ] );

//  glBindVertexArray( vao );


//  if ( usingIBO )
//  {
//    _checkItemExists( ibo, indexBuffers_, "indexBuffer" );

//    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffers_[ ibo ] );
//    glDrawElements( mode, verts, iboType, pOffset );
//    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
//  }
//  else
//  {
//    glDrawArrays( mode, start, verts );
//  }

//  glBindVertexArray( 0 );
//} // OpenGLHelper::renderBuffer



//void
//OpenGLHelper::setBoolUniform(
//                             const std::string program,
//                             const std::string uniform,
//                             bool              var
//                             )
//{
//  _checkItemExists( program, programs_, "programs" );
//  glUniform1i( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), var );
//}



//void
//OpenGLHelper::setIntUniform(
//                            const std::string program,
//                            const std::string uniform,
//                            int               value
//                            )
//{
//  _checkItemExists( program, programs_, "programs" );
//  glUniform1i( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), value );
//}



//void
//OpenGLHelper::setFloatUniform(
//                              const std::string program,
//                              const std::string uniform,
//                              const float      *pValue,
//                              const int         size,
//                              const int         count
//                              )
//{
//  _checkItemExists( program, programs_, "programs" );

//  switch ( size )
//  {

//  case 1:
//    glUniform1f( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), *pValue );
//    break;

//  case 2:
//    glUniform2fv( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), count, pValue );
//    break;

//  case 3:
//    glUniform3fv( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), count, pValue );
//    break;

//  case 4:
//    glUniform4fv( glGetUniformLocation( programs_[ program ], uniform.c_str( ) ), count, pValue );
//    break;

//  default:
//    std::stringstream msg;
//    msg << "Float or vector of size " << size << " does not exist";
//    throw std::runtime_error( msg.str( ) );
//    break;

//  } // switch

//} // setFloatUniform



//void
//OpenGLHelper::setMatrixUniform(
//                               const std::string program,
//                               const std::string uniform,
//                               const float      *pValue,
//                               const int         size,
//                               const int         count
//                               )
//{
//  _checkItemExists( program, programs_, "programs" );

//  switch ( size )
//  {
//  case 2:
//    glUniformMatrix2fv(
//                       glGetUniformLocation( programs_[ program ], uniform.c_str( ) ),
//                       count,
//                       GL_FALSE,
//                       pValue
//                       );
//    break;

//  case 3:
//    glUniformMatrix3fv(
//                       glGetUniformLocation( programs_[ program ], uniform.c_str( ) ),
//                       count,
//                       GL_FALSE,
//                       pValue
//                       );
//    break;


//  case 4:
//    glUniformMatrix4fv(
//                       glGetUniformLocation( programs_[ program ], uniform.c_str( ) ),
//                       count,
//                       GL_FALSE,
//                       pValue
//                       );
//    break;

//  default:
//    std::stringstream msg;
//    msg << "Matrix of size " << size << " does not exist";
//    throw std::runtime_error( msg.str( ) );
//    break;
//  } // switch

//} // OpenGLHelper::setMatrixUniform



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



//////////////////////////////////////////////////////////////////////////////////
///// \brief OpenGLHelper::deleteProgram
///// \param program
/////
///// \author Logan Barnes
//////////////////////////////////////////////////////////////////////////////////
//void
//OpenGLHelper::deleteProgram( const GLuint program )
//{
//  glDeleteProgram( program );
//}



//////////////////////////////////////////////////////////////////////////////////
///// \brief OpenGLHelper::deleteTexture
///// \param tex
/////
///// \author Logan Barnes
//////////////////////////////////////////////////////////////////////////////////
//void
//OpenGLHelper::deleteTexture( const GLuint tex )
//{
//  glDeleteTextures( 1, &tex );
//}



//////////////////////////////////////////////////////////////////////////////////
///// \brief OpenGLHelper::deleteFramebuffer
///// \param fbo
/////
///// \author Logan Barnes
//////////////////////////////////////////////////////////////////////////////////
//void
//OpenGLHelper::deleteFramebuffer( const GLuint fbo )
//{
//  glDeleteFramebuffers( 1, &fbo );
//}



//////////////////////////////////////////////////////////////////////////////////
///// \brief OpenGLHelper::deleteRenderbuffer
///// \param rbo
/////
///// \author Logan Barnes
//////////////////////////////////////////////////////////////////////////////////
//void
//OpenGLHelper::deleteRenderbuffer( const GLuint rbo )
//{
//  glDeleteRenderbuffers( 1, &rbo );
//}



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
