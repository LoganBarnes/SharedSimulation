#ifndef OpenGLWrapper_hpp
#define OpenGLWrapper_hpp


#include <string>
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
  getTexture( const std::string name ) { return m_textures[ name ]; }

  GLsizei
  getViewportWidth( ) { return m_viewportWidth; }

  GLsizei
  getViewportHeight( ) { return m_viewportHeight; }


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


private:

  static
  std::string _readFile ( const std::string filePath );

  static
  GLuint _loadShader (
                      const std::string vertex_path,
                      const std::string fragment_path
                      );


  std::unordered_map< std::string, GLuint > m_programs;
  std::unordered_map< std::string, GLuint > m_textures;
  std::unordered_map< std::string, Buffer > m_buffers;
  std::unordered_map< std::string, Buffer > m_framebuffers;

  GLsizei m_viewportWidth, m_viewportHeight;

  CallbackSingleton *m_input;

  bool m_initialized;

};


} // namespace graphics


#endif // OpenGLWrapper_hpp
