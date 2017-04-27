// SharedCallback.hpp
#pragma once


#include "shared/graphics/Callback.hpp"


namespace shs
{


class SharedCallback : public shg::Callback
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


} // namespace shs
