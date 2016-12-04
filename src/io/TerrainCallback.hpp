#ifndef TerrainCallback_hpp
#define TerrainCallback_hpp


#include "Callback.hpp"


namespace srt
{


class TerrainCallback : public graphics::Callback
{

public:


  TerrainCallback( );

  ~TerrainCallback( );


  ////////////////////////////////////////////////////////
  /// \brief handleKey
  /// \param pWindow
  /// \param key
  /// \param scancode
  /// \param action
  /// \param mods
  ////////////////////////////////////////////////////////
  virtual
  void  handleKey(
                  GLFWwindow *pWindow,
                  int         key,
                  int         scancode,
                  int         action,
                  int         mods
                  );


};


} // namespace srt


#endif // TerrainCallback_hpp
