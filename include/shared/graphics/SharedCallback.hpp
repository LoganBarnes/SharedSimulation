// SharedCallback.hpp
#pragma once


#include "shared/graphics/Callback.hpp"


namespace shared
{


class SharedCallback : public graphics::Callback
{

public:

  explicit
  SharedCallback( );

  virtual
  ~SharedCallback( );


  ////////////////////////////////////////////////////////
  /// \brief handleKey
  /// \param pWindow
  /// \param key
  /// \param scancode
  /// \param action
  /// \param mods
  ////////////////////////////////////////////////////////
  virtual
  void  handleKey (
                   GLFWwindow *pWindow,
                   int         key,
                   int         scancode,
                   int         action,
                   int         mods
                   );


};


} // namespace shared
