// OpenGLHelper.hpp
#pragma once

#include "shared/graphics/GraphicsForwardDeclarations.hpp"

#include <glad/glad.h>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>


namespace shg
{


///
/// \brief The VAOSetting struct
///
struct VAOElement
{
  std::string name;
  GLint size;
  GLenum type;
  void *pointer;
};


typedef std::vector< std::shared_ptr< GLuint > > IdVec;


class OpenGLHelper
{

public:

  static
  void setDefaults ( );

  template< typename ... Shaders >
  static
  std::shared_ptr< GLuint >  createProgram ( const Shaders ... shaders );

  static
  std::shared_ptr< GLuint >  createTextureArray (
                                                 GLsizei width,
                                                 GLsizei height,
                                                 float  *pArray = nullptr,
                                                 GLint   filterType = GL_NEAREST,
                                                 GLint   wrapType = GL_REPEAT,
                                                 GLint   internalFormat = GL_RGBA32F,
                                                 GLenum  format = GL_RGBA
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
                     const size_t                     elementOffset,
                     const size_t                     numElements,
                     const T                         *pData,
                     const GLenum                     bufferType
                     );

  static
  std::shared_ptr< GLuint > createVao (
                                       const std::shared_ptr< GLuint > &spProgram,
                                       const std::shared_ptr< GLuint > &spVbo,
                                       const GLsizei                    totalStride,
                                       const std::vector< VAOElement > &elements
                                       );


  static
  std::shared_ptr< GLuint > createFramebuffer (
                                               GLsizei                         width,
                                               GLsizei                         height,
                                               const std::shared_ptr< GLuint > spColorTex = nullptr,
                                               const std::shared_ptr< GLuint > spDepthTex = nullptr
                                               );

  template< typename T >
  static
  StandardPipeline createStandardPipeline (
                                           const std::vector< std::string > &shaderFiles,
                                           const T                          *pData,
                                           const size_t                      numElements,
                                           const GLsizei                     totalStride,
                                           const std::vector< VAOElement >  &elements,
                                           const GLenum                      type = GL_ARRAY_BUFFER,
                                           const GLenum                      usage = GL_STATIC_DRAW
                                           );

  static
  void bindFramebuffer ( );

  static
  void bindFramebuffer ( const std::shared_ptr< GLuint > &spFbo );

  static
  void clearFramebuffer ( );

  static
  void setTextureUniform (
                          const std::shared_ptr< GLuint > &spProgram,
                          const std::string                uniform,
                          const std::shared_ptr< GLuint > &spTexture,
                          int                              activeTex
                          );

  static
  void setIntUniform (
                      const std::shared_ptr< GLuint > &spProgram,
                      const std::string                uniform,
                      const int                       *pValue,
                      const int                        size = 1,
                      const int                        count = 1
                      );

  static
  void setFloatUniform (
                        const std::shared_ptr< GLuint > &spProgram,
                        const std::string                uniform,
                        const float                     *pValue,
                        const int                        size = 1,
                        const int                        count = 1
                        );


  static
  void setMatrixUniform (
                         const std::shared_ptr< GLuint > &spProgram,
                         const std::string                uniform,
                         const float                     *pValue,
                         const int                        size = 4,
                         const int                        count = 1
                         );

  static
  void renderBuffer (
                     const std::shared_ptr< GLuint > &spVao,
                     const int                        start,
                     const int                        verts,
                     const GLenum                     mode,
                     const std::shared_ptr< GLuint > &spIbo = nullptr,
                     const void                      *pOffset = 0,
                     const GLenum                     iboType = GL_UNSIGNED_SHORT
                     );


//  void setBlending ( bool blend );

//  void bindBufferToTexture (
//                            const std::string texture,
//                            GLuint            bufId,
//                            int               alignment,
//                            int               width,
//                            int               height
//                            );


private:

  static
  std::string _readFile ( const std::string filePath );

  static
  std::shared_ptr< GLuint > _createShader (
                                           GLenum            shaderType,
                                           const std::string filePath
                                           );

  static
  std::shared_ptr< GLuint > _createShader ( const std::string filePath );

  template< typename ... Shaders >
  static
  void _createShader (
                      IdVec            *pIds,
                      const std::string filePath
                      );

  template< typename ... Shaders >
  static
  void _createShader (
                      IdVec            *pIds,
                      const std::string filePath,
                      const Shaders ... shaders
                      );

  static
  std::shared_ptr< GLuint > _createProgram ( const IdVec shaderIds );
};



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::createProgram
/// \param shaders
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename ... Shaders >
std::shared_ptr< GLuint >
OpenGLHelper::createProgram( const Shaders ... shaders )
{
  IdVec shaderIds;

  // create and compile all the shaders
  OpenGLHelper::_createShader( &shaderIds, shaders ... );

  // link shaders and create OpenGL program
  return OpenGLHelper::_createProgram( shaderIds );
}



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::_createShader
///
///        Base case for templated shader creation functions
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename ... Shaders >
void
OpenGLHelper::_createShader(
                            IdVec            *pIds,
                            const std::string filePath
                            )
{
  // create/compile shader and add it to list of shaders
  pIds->emplace_back( OpenGLHelper::_createShader( filePath ) );
}



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::_createShader
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename ... Shaders >
void
OpenGLHelper::_createShader(
                            IdVec            *pIds,      ///<
                            const std::string filePath, ///<
                            const Shaders ... shaders   ///<
                            )
{
  // create/compile shader and add it to list of shaders
  pIds->emplace_back( OpenGLHelper::_createShader( filePath ) );

  // repeat for the rest of the shaders
  OpenGLHelper::_createShader( pIds, shaders ... );
}



////////////////////////////////////////////////////////////////////////////////
/// \brief OpenGLHelper::createBuffer
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename T >
std::shared_ptr< GLuint >
OpenGLHelper::createBuffer(
                           const T     *pData,       ///<
                           const size_t numElements, ///<
                           const GLenum type,        ///<
                           const GLenum usage        ///<
                           )
{
  std::shared_ptr< GLuint > upBuffer( new GLuint,
                                     [ ] ( auto pID )
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
                           const std::shared_ptr< GLuint > &upBuffer,     ///<
                           const size_t                     elementOffset, ///<
                           const size_t                     numElements,  ///<
                           const T                         *pData,        ///<
                           const GLenum                     bufferType    ///<
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



////////////////////////////////////////////////////////////////////////////////
/// \brief createStandardPipeline
/// \return
///
/// \author Logan Barnes
////////////////////////////////////////////////////////////////////////////////
template< typename T >
StandardPipeline
OpenGLHelper::createStandardPipeline(
                                     const std::vector< std::string > &shaderFiles, ///<
                                     const T                          *pData,      ///<
                                     const size_t                      numElements, ///<
                                     const GLsizei                     totalStride, ///<
                                     const std::vector< VAOElement >  &elements,   ///<
                                     const GLenum                      type,       ///<
                                     const GLenum                      usage       ///<
                                     )
{
  StandardPipeline glIds;

  IdVec shaders;
  for ( auto &shaderFile : shaderFiles )
  {
    shaders.emplace_back(OpenGLHelper::_createShader(shaderFile));
  }

  glIds.program = OpenGLHelper::_createProgram( shaders );

  glIds.vbo = OpenGLHelper::createBuffer(
                                         pData,
                                         numElements,
                                         type,
                                         usage
                                         );

  glIds.vao = OpenGLHelper::createVao(
                                      glIds.program,
                                      glIds.vbo,
                                      totalStride,
                                      elements
                                      );

  return glIds;
} // OpenGLHelper::createStandardPipeline



} // namespace shg
