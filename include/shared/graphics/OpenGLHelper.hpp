// OpenGLHelper.hpp
#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "glad/glad.h"


namespace graphics
{


class OpenGLHelper
{

public:

  static
  void setDefaults ( );


  static
  std::shared_ptr< GLuint >  createProgram (
                                            const std::string vertFilePath,
                                            const std::string fragFilePath
                                            );

  static
  std::shared_ptr< GLuint >  createTextureArray (
                                                 GLsizei width,
                                                 GLsizei height,
                                                 float  *pArray = nullptr,
                                                 GLint   filterType = GL_NEAREST,
                                                 GLint   wrapType = GL_REPEAT,
                                                 bool    mipmap = true
                                                 );

  template< typename T >
  static
  std::shared_ptr< GLuint >  createBuffer (
                                           const T     *pData,
                                           const size_t numElements,
                                           const GLenum type = GL_ARRAY_BUFFER,
                                           const GLenum usage = GL_STATIC_DRAW
                                           );

  template< typename T >
  static
  std::shared_ptr< GLuint >  createIndexBuffer (
                                                const T     *pData,
                                                const size_t numElements,
                                                const GLenum usage
                                                );

  template< typename T >
  static
  void updateBuffer (
                     const std::shared_ptr< GLuint > &spBuffer,
                     const size_t                    elementOffset,
                     const size_t                    numElements,
                     const T                        *pData,
                     const GLenum                    bufferType
                     );


//  void addFramebuffer (
//                       const std::string buffer,
//                       GLsizei           width,
//                       GLsizei           height,
//                       const std::string texture
//                       );

//  static
//  void deleteProgram      ( const GLuint program );
//  static
//  void deleteTexture      ( const GLuint tex );
//  static
//  void deleteFramebuffer  ( const GLuint fbo );
//  static
//  void deleteRenderbuffer ( const GLuint rbo );

  void clearFramebuffer ( );


//  void useProgram ( const std::string program );

//  void renderBuffer (
//                     const std::string buffer,
//                     const int         start,
//                     const int         verts,
//                     const GLenum      mode,
//                     const std::string ibo = "",
//                     const void       *pOffset = 0,
//                     const GLenum      iboType = GL_UNSIGNED_SHORT
//                     );


//  void setTextureUniform (
//                          const std::string program,
//                          const std::string uniform,
//                          const std::string texture,
//                          int               activeTex
//                          );
//  void setBoolUniform (
//                       const std::string program,
//                       const std::string uniform,
//                       bool              var
//                       );
//  void setIntUniform (
//                      const std::string program,
//                      const std::string uniform,
//                      int               value
//                      );

//  void setFloatUniform (
//                        const std::string program,
//                        const std::string uniform,
//                        const float      *pValue,
//                        const int         size  = 1,
//                        const int         count = 1
//                        );

//  void setMatrixUniform (
//                         const std::string program,
//                         const std::string uniform,
//                         const float      *pValue,
//                         const int         size  = 4,
//                         const int         count = 1
//                         );

//  void swapTextures (
//                     const std::string tex1,
//                     const std::string tex2
//                     );

//  void setBlending ( bool blend );
//  void setCallback ( Callback *callback );

//  void setWindowSize (
//                      GLsizei width,
//                      GLsizei height
//                      );

//  void setViewportSize (
//                        GLsizei width,
//                        GLsizei height
//                        );

//  void bindBufferToTexture (
//                            const std::string texture,
//                            GLuint            bufId,
//                            int               alignment,
//                            int               width,
//                            int               height
//                            );

//  void setClearColor (
//                      const float r,
//                      const float g,
//                      const float b,
//                      const float a
//                      );

//  void destroyTexture (
//                       const std::string name,
//                       const bool        glDelete = true
//                       );
//  void destroyFramebuffer ( const std::string name );


private:

  static
  std::string _readFile ( const std::string filePath );

  static
  std::shared_ptr< GLuint > _loadShader (
                                         const std::string vertex_path,
                                         const std::string fragment_path
                                         );


};



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::createBuffer
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename T >
std::shared_ptr< GLuint >
OpenGLHelper::createBuffer(const T     *pData,       ///<
                           const size_t numElements, ///<
                           const GLenum type,        ///<
                           const GLenum usage        ///<
                           )
{
  std::shared_ptr< GLuint > upBuffer( new GLuint,
                                     [] ( auto pID )
                                     {
                                       glDeleteBuffers( 1, pID );
                                       delete pID;
                                     } );

  glGenBuffers( 1, upBuffer.get( ) );
  glBindBuffer( type, *upBuffer );
  glBufferData(
               type,
               static_cast< GLsizeiptr >( numElements * sizeof( T ) ),
               pData,
               usage
               );

  glBindBuffer( type, 0 );

  return upBuffer;
} // OpenGLHelper::addBuffer




//////////////////////////////////////////////////////////////////////////////////
///// \brief OpenGLHelper::createIndexBuffer
///// \return
/////
///// \author Logan Barnes
//////////////////////////////////////////////////////////////////////////////////
//template< typename T >
//std::shared_ptr< GLuint >
//OpenGLHelper::createIndexBuffer(const T     *pData,       ///<
//                                const size_t numElements, ///<
//                                const GLenum usage        ///<
//                                )
//{
//  std::shared_ptr< GLuint > upIbo( new GLuint,
//                                  [] ( auto pID )
//                                  {
//                                    glDeleteBuffers( 1, pID );
//                                    delete pID;
//                                  } );

//  glGenBuffers( 1, upIbo.get( ) );
//  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *upIbo );
//  glBufferData(
//               GL_ELEMENT_ARRAY_BUFFER,
//               static_cast< GLsizeiptr >( numElements * sizeof( T ) ),
//               pData,
//               usage
//               );

//  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

//  return upIbo;
//} // OpenGLHelper::addIndexBuffer




////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::updateBuffer
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename T >
void
OpenGLHelper::updateBuffer(
                           const std::shared_ptr< GLuint > &upBuffer, ///<
                           const size_t                    elementOffset, ///<
                           const size_t                    numElements, ///<
                           const T                        *pData,     ///<
                           const GLenum                    bufferType ///<
                           )
{
  constexpr auto typeSizeBytes = sizeof( T );

  glBindBuffer( bufferType, *upBuffer );
  glBufferSubData(
                  bufferType,
                  static_cast< GLintptr >( elementOffset * typeSizeBytes ),
                  static_cast< GLsizeiptr >( numElements * typeSizeBytes ),
                  pData
                  );

  glBindBuffer( bufferType, 0 );
} // OpenGLHelper::updateBuffer



} // namespace graphics
