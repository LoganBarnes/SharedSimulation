// OpenGLHelper.hpp
#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "glad/glad.h"



namespace graphics
{


class OpenGLHelper
{

public:

  static
  void setDefaults ( );


  static
  GLuint createProgram (
                        const std::string vertFilePath,
                        const std::string fragFilePath
                        );

  static
  GLuint createTextureArray (
                             GLsizei width,
                             GLsizei height,
                             float  *pArray = nullptr,
                             GLint   filterType = GL_NEAREST,
                             GLint   wrapType = GL_REPEAT,
                             bool    mipmap = true
                             );

  template< typename T >
  static
  GLuint createBuffer (
                       const T     *pData,
                       const size_t numElements,
                       const GLenum type
                       );

  template< typename T >
  static
  GLuint createIndexBuffer (
                            const T     *pData,
                            const size_t numElements,
                            const GLenum type
                            );

  template< typename T >
  static
  void updateBuffer (
                     const GLuint buffer,
                     const size_t elementOffset,
                     const size_t numElements,
                     const T     *pData,
                     const GLenum bufferType
                     );


//  void addFramebuffer (
//                       const std::string buffer,
//                       GLsizei           width,
//                       GLsizei           height,
//                       const std::string texture
//                       );

  static
  void deleteProgram      ( const GLuint program );
  static
  void deleteTexture      ( const GLuint tex );
  static
  void deleteFramebuffer  ( const GLuint fbo );
  static
  void deleteRenderbuffer ( const GLuint rbo );

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


//  void
//  setCurrentContext( void *pContext ) { pContext_ = pContext; }


private:

  static
  std::string _readFile ( const std::string filePath );

  static
  GLuint _loadShader (
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
GLuint
OpenGLHelper::createBuffer(
                           const T     *pData,       ///<
                           const size_t numElements, ///<
                           const GLenum type         ///<
                           )
{
  GLuint buffer;

  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData(
               GL_ARRAY_BUFFER,
               static_cast< GLsizeiptr >( numElements * sizeof( T ) ),
               pData,
               type
               );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  return buffer;
} // OpenGLHelper::addBuffer



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::createIndexBuffer
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename T >
GLuint
OpenGLHelper::createIndexBuffer(
                                const T     *pData,       ///<
                                const size_t numElements, ///<
                                const GLenum type         ///<
                                )
{
  GLuint ibo;

  glGenBuffers( 1, &ibo );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
  glBufferData(
               GL_ELEMENT_ARRAY_BUFFER,
               static_cast< GLsizeiptr >( numElements * sizeof( T ) ),
               pData,
               type
               );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

  return ibo;
} // OpenGLHelper::addIndexBuffer



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::updateBuffer
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename T >
void
OpenGLHelper::updateBuffer(
                           const GLuint buffer,        ///<
                           const size_t elementOffset, ///<
                           const size_t numElements,   ///<
                           const T     *pData,         ///<
                           const GLenum bufferType     ///<
                           )
{
  constexpr auto typeSizeBytes = sizeof( T );

  glBindBuffer( bufferType, buffer );
  glBufferSubData(
                  bufferType,
                  static_cast< GLintptr >( elementOffset * typeSizeBytes ),
                  static_cast< GLsizeiptr >( numElements * typeSizeBytes ),
                  pData
                  );

  glBindBuffer( bufferType, 0 );
} // OpenGLHelper::updateBuffer



} // namespace graphics
