#ifndef OpenGLWrapper_hpp
#define OpenGLWrapper_hpp

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "glad/glad.h"



namespace graphics
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

///
/// \brief The VAOSettings struct
///
struct VAOSettings
{
  std::string program;
  GLsizei totalStride;
  std::vector< VAOElement > settings;

  VAOSettings( ) {}

  VAOSettings(
              std::string program_,
              GLsizei     totalStride_ = 0
              )
    : program( program_ ), totalStride( totalStride_ )
  {}
};

///
/// \brief The Buffer struct
///
struct Buffer
{
  GLuint vbo;
  VAOSettings settings;
};


///
/// \brief The FrameBuffer struct
///
struct FrameBuffer
{
  GLuint fbo;
  GLuint rbo;
};


class CallbackSingleton;
class Callback;


class OpenGLWrapper
{

public:

  explicit
  OpenGLWrapper( );

  ~OpenGLWrapper( );

  void initContext (
                    GLsizei width  = 640,
                    GLsizei height = 480
                    );

  // getters
  GLuint getTexture ( const std::string name );

  GLuint getBuffer ( const std::string name );

  GLsizei
  getViewportWidth( ) { return viewportWidth_; }

  GLsizei
  getViewportHeight( ) { return viewportHeight_; }


  void addProgram (
                   const std::string name,
                   const std::string vertFilePath,
                   const std::string fragFilePath
                   );

  void addTextureArray (
                        const std::string name,
                        GLsizei           width,
                        GLsizei           height,
                        float            *pArray = NULL,
                        bool              linear = false
                        );

  void addTextureImage (
                        const std::string name,
                        GLsizei           width,
                        GLsizei           height,
                        const std::string filename
                        );

  template< typename T >
  void addBuffer (
                  const std::string  name,
                  const T           *pData,
                  const size_t       numElements,
                  const GLenum       type,
                  const VAOSettings &settings,
                  const bool         forceOverride = false
                  );

  template< typename T >
  void addIndexBuffer (
                       const std::string name,
                       const T          *pData,
                       const size_t      numElements,
                       const GLenum      type,
                       const bool        forceOverride = false
                       );

  template< typename T >
  void updateBuffer (
                     const std::string bufferName,
                     const size_t      elementOffset,
                     const size_t      numElements,
                     const float      *pData,
                     const GLenum      bufferType = GL_ARRAY_BUFFER
                     );


  void addFramebuffer (
                       const std::string buffer,
                       GLsizei           width,
                       GLsizei           height,
                       const std::string texture
                       );
  void bindFramebuffer ( const std::string name = "" );
  void swapFramebuffers (
                         const std::string fbo1,
                         const std::string fbo2
                         );

  void clearWindow (
                    GLsizei width = 0,
                    GLsizei height = 0
                    );

  void useProgram ( const std::string program );

  void renderBuffer (
                     const std::string buffer,
                     const int         start,
                     const int         verts,
                     const GLenum      mode,
                     const std::string ibo = "",
                     const void       *pOffset = 0,
                     const GLenum      iboType = GL_UNSIGNED_SHORT
                     );


  void setTextureUniform (
                          const std::string program,
                          const std::string uniform,
                          const std::string texture,
                          int               activeTex
                          );
  void setBoolUniform (
                       const std::string program,
                       const std::string uniform,
                       bool              var
                       );
  void setIntUniform (
                      const std::string program,
                      const std::string uniform,
                      int               value
                      );

  void setFloatUniform (
                        const std::string program,
                        const std::string uniform,
                        const float      *pValue,
                        const int         size  = 1,
                        const int         count = 1
                        );

  void setMatrixUniform (
                         const std::string program,
                         const std::string uniform,
                         const float      *pValue,
                         const int         size  = 4,
                         const int         count = 1
                         );

  void swapTextures (
                     const std::string tex1,
                     const std::string tex2
                     );

  void setBlending ( bool blend );
  void setCallback ( Callback *callback );

  void setWindowSize (
                      GLsizei width,
                      GLsizei height
                      );

  void setViewportSize (
                        GLsizei width,
                        GLsizei height
                        );

  void bindBufferToTexture (
                            const std::string texture,
                            GLuint            bufId,
                            int               alignment,
                            int               width,
                            int               height
                            );

  void setClearColor (
                      const float r,
                      const float g,
                      const float b,
                      const float a
                      );

  void
  setCurrentContext( void *pContext ) { pContext_ = pContext; }


private:

  static
  std::string _readFile ( const std::string filePath );

  static
  GLuint _loadShader (
                      const std::string vertex_path,
                      const std::string fragment_path
                      );

  GLuint _getVAO ( const Buffer &buf );

  GLuint _addVAOToBuffer (
                          const GLuint       vbo,
                          const VAOSettings &settings
                          ) const;

  template< typename Map >
  static
  void _checkItemExists (
                         const std::string &key,
                         const Map         &map,
                         const std::string &mapDiscription = ""
                         );


  std::unordered_map< std::string, GLuint > programs_;
  std::unordered_map< std::string, GLuint > textures_;
  std::unordered_map< std::string, Buffer > buffers_;
  std::unordered_map< std::string, unsigned > indexBuffers_;
  std::unordered_map< std::string, FrameBuffer > framebuffers_;

  typedef std::unordered_map< void*, GLuint > ContextVAOMap;
  typedef std::unordered_map< GLuint, ContextVAOMap > BufferVAOMap;

  BufferVAOMap vaoMap_;

  GLsizei viewportWidth_, viewportHeight_;

  CallbackSingleton *input_;

  void *pContext_;

  bool initialized_;

};



template< typename T >
void
OpenGLWrapper::addBuffer(
                         const std::string  name,
                         const T           *pData,
                         const size_t       numElements,
                         const GLenum       type,
                         const VAOSettings &settings,
                         const bool         forceOverride
                         )
{
  if ( buffers_.find( name ) != buffers_.end( ) )
  {
    if ( !forceOverride )
    {
      std::stringstream msg;
      msg << "Buffer with name " << name << " already exists and 'forceOverride' is false.";
      throw std::runtime_error( msg.str( ) );
    }

    Buffer &buffer = buffers_[ name ];
    vaoMap_.erase( buffer.vbo );
    glDeleteBuffers( 1, &buffer.vbo );
  }

  Buffer &buffer = buffers_[ name ];

  glGenBuffers( 1, &buffer.vbo );
  glBindBuffer( GL_ARRAY_BUFFER, buffer.vbo );
  glBufferData(
               GL_ARRAY_BUFFER,
               static_cast< GLsizeiptr >( numElements * sizeof( T ) ),
               pData,
               type
               );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  buffer.settings = settings;
} // OpenGLWrapper::addBuffer



template< typename T >
void
OpenGLWrapper::addIndexBuffer(
                              const std::string name,
                              const T          *pData,
                              const size_t      numElements,
                              const GLenum      type,
                              const bool        forceOverride
                              )
{
  if ( indexBuffers_.find( name ) != indexBuffers_.end( ) )
  {
    if ( !forceOverride )
    {
      std::stringstream msg;
      msg << "Index buffer with name " << name << " already exists and 'forceOverride' is false.";
      throw std::runtime_error( msg.str( ) );
    }

    GLuint &ibo = indexBuffers_[ name ];
    glDeleteBuffers( 1, &ibo );
  }

  GLuint &ibo = indexBuffers_[ name ];

  glGenBuffers( 1, &ibo );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
  glBufferData(
               GL_ELEMENT_ARRAY_BUFFER,
               static_cast< GLsizeiptr >( numElements * sizeof( T ) ),
               pData,
               type
               );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
} // OpenGLWrapper::addIndexBuffer



template< typename T >
void
OpenGLWrapper::updateBuffer(
                            const std::string bufferName,
                            const size_t      elementOffset,
                            const size_t      numElements,
                            const float      *pData,
                            const GLenum      bufferType
                            )
{
  _checkItemExists( bufferName, buffers_, "buffer" );

  Buffer &buffer = buffers_[ bufferName ];
  GLuint vao     = _getVAO( buffer );

  constexpr auto floatSize = sizeof( float );

  glBindVertexArray( vao );
  glBindBuffer( bufferType, buffer.vbo );
  glBufferSubData(
                  bufferType,
                  static_cast< GLintptr >( elementOffset * floatSize ),
                  static_cast< GLsizeiptr >( numElements * floatSize ),
                  pData
                  );

  glBindVertexArray( 0 );
  glBindBuffer( bufferType, 0 );
} // OpenGLWrapper::updateBuffer



template< typename Map >
void
OpenGLWrapper::_checkItemExists(
                                const std::string &key,
                                const Map         &map,
                                const std::string &mapDiscription
                                )
{
  if ( map.find( key ) == map.end( ) )
  {
    std::stringstream msg;
    msg << "Item '" << key << "' not found in " << mapDiscription << " map.";
    throw std::runtime_error( msg.str( ) );
  }
}



} // namespace graphics


#endif // OpenGLWrapper_hpp
