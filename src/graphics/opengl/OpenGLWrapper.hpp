#ifndef OpenGLWrapper_hpp
#define OpenGLWrapper_hpp

#include <string>
#include <unordered_map>

#include "glad/glad.h"


struct Buffer
{
  GLuint vbo;
  GLuint vao;
};



namespace graphics
{


class CallbackSingleton;
class Callback;


class OpenGLWrapper
{

public:

  explicit
  OpenGLWrapper(
                GLsizei width  = 640,
                GLsizei height = 480
                );

  ~OpenGLWrapper( );

  void init ( );

  // getters
  GLuint
  getTexture( const std::string name ) { return textures_[ name ]; }

  GLuint
  getBuffer( const std::string name ) { return buffers_[ name ].vbo; }

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
  void addBufferNoVAO (
                       const std::string name,
                       GLsizeiptr        sizeBytes,
                       T                *pData,
                       GLenum            type
                       );

  void addUVBuffer (
                    const std::string buffer,
                    const std::string program,
                    GLfloat          *data,
                    GLuint            size,
                    bool              dynamic = false
                    );


  void addFramebuffer (
                       const std::string buffer,
                       GLsizei           width,
                       GLsizei           height,
                       const std::string texture
                       );
  void bindFramebuffer ( const std::string name );
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
                     int               verts,
                     GLenum            mode
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

  void setBuffer (
                  const std::string bufferName,
                  float            *data,
                  GLuint            size
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


private:

  static
  std::string _readFile ( const std::string filePath );

  static
  GLuint _loadShader (
                      const std::string vertex_path,
                      const std::string fragment_path
                      );


  std::unordered_map< std::string, GLuint > programs_;
  std::unordered_map< std::string, GLuint > textures_;
  std::unordered_map< std::string, Buffer > buffers_;
  std::unordered_map< std::string, Buffer > framebuffers_;

  GLsizei viewportWidth_, viewportHeight_;

  CallbackSingleton *input_;

  bool initialized_;

};



template< typename T >
void
OpenGLWrapper::addBufferNoVAO(
                              const std::string name,
                              GLsizeiptr        sizeBytes,
                              T                *pData,
                              GLenum            type
                              )
{

  Buffer buffer;

  if ( buffers_.find( name ) != buffers_.end( ) )
  {
    buffer = buffers_[ name ];
    glDeleteBuffers( 1, &( buffer.vbo ) );
  }

  glGenBuffers( 1, &buffer.vbo );
  glBindBuffer( GL_ARRAY_BUFFER, buffer.vbo );
  glBufferData(
               GL_ARRAY_BUFFER,
               sizeBytes,
               pData,
               type
               );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  buffers_[ name ] = buffer;

} // OpenGLWrapper::addBufferNoVAO



} // namespace graphics


#endif // OpenGLWrapper_hpp
