#ifndef OpenGLWrapper_hpp
#define OpenGLWrapper_hpp


#include <unordered_map>


typedef int GLsizei;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef uint32_t GLenum;


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
                GLsizei width = 640,
                GLsizei height = 480
                );

  ~OpenGLWrapper( );

  void init ( );

  // getters
  GLuint
  getTexture( const char *name ) { return m_textures[ name ]; }

  GLsizei
  getViewportWidth( ) { return m_viewportWidth; }

  GLsizei
  getViewportHeight( ) { return m_viewportHeight; }


  void addProgram (
                   const char *name,
                   const char *vertFilePath,
                   const char *fragFilePath
                   );

  void addTextureArray (
                        const char *name,
                        GLsizei     width,
                        GLsizei     height,
                        float      *pArray = NULL,
                        bool        linear = false
                        );
  void addTextureImage (
                        const char *name,
                        GLsizei     width,
                        GLsizei     height,
                        const char *filename
                        );

  void addUVBuffer (
                    const char *buffer,
                    const char *program,
                    GLfloat    *data,
                    GLuint      size,
                    bool        dynamic = false
                    );

  void addFramebuffer (
                       const char *buffer,
                       GLsizei     width,
                       GLsizei     height,
                       const char *texture
                       );
  void bindFramebuffer ( const char *name );
  void swapFramebuffers (
                         const char *fbo1,
                         const char *fbo2
                         );

  void clearWindow (
                    GLsizei width = 0,
                    GLsizei height = 0
                    );

  void useProgram ( const char *program );

  void renderBuffer (
                     const char *buffer,
                     int         verts,
                     GLenum      mode
                     );

  void setTextureUniform (
                          const char *program,
                          const char *uniform,
                          const char *texture,
                          int         activeTex
                          );
  void setBoolUniform (
                       const char *program,
                       const char *uniform,
                       bool        var
                       );
  void setIntUniform (
                      const char *program,
                      const char *uniform,
                      int         value
                      );
  void setBuffer (
                  const char *bufferName,
                  float      *data,
                  GLuint      size
                  );

  void swapTextures (
                     const char *tex1,
                     const char *tex2
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


private:

  static
  std::string _readFile ( const char *filePath );

  static
  GLuint _loadShader (
                      const char *vertex_path,
                      const char *fragment_path
                      );


  std::unordered_map< const char*, GLuint > m_programs;
  std::unordered_map< const char*, GLuint > m_textures;
  std::unordered_map< const char*, Buffer > m_buffers;
  std::unordered_map< const char*, Buffer > m_framebuffers;

  GLsizei m_viewportWidth, m_viewportHeight;

  CallbackSingleton *m_input;

  bool m_initialized;

};


} // namespace graphics


#endif // OpenGLWrapper_hpp
