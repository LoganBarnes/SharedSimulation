// ImguiCallback.hpp
#pragma once

#include "shared/graphics/Callback.hpp"
#include <memory>


namespace shs
{


class ImguiCallback : public shg::Callback
{

public:

  ImguiCallback( std::unique_ptr< shg::Callback > upCallback = nullptr );

  ~ImguiCallback( );


  ////////////////////////////////////////////////////////
  /// \brief setCallback
  /// \param upCallback
  ////////////////////////////////////////////////////////
  void setCallback ( std::unique_ptr< shg::Callback > upCallback );


  ////////////////////////////////////////////////////////
  /// \brief handleWindowSize
  /// \param pWindow
  /// \param width
  /// \param height
  ////////////////////////////////////////////////////////
  virtual
  void handleWindowSize (
                         GLFWwindow *pWindow,
                         int         width,
                         int         height
                         );

  ////////////////////////////////////////////////////////
  /// \brief handleMouseButton
  /// \param pWindow
  /// \param button
  /// \param action
  /// \param mods
  ////////////////////////////////////////////////////////
  virtual
  void handleMouseButton (
                          GLFWwindow *pWindow,
                          int         button,
                          int         action,
                          int         mods
                          );

  ////////////////////////////////////////////////////////
  /// \brief handleKey
  /// \param pWindow
  /// \param key
  /// \param scancode
  /// \param action
  /// \param mods
  ////////////////////////////////////////////////////////
  virtual
  void handleKey (
                  GLFWwindow *pWindow,
                  int         key,
                  int         scancode,
                  int         action,
                  int         mods
                  );

  ////////////////////////////////////////////////////////
  /// \brief handleCursorPosition
  /// \param pWindow
  /// \param xpos
  /// \param ypos
  ////////////////////////////////////////////////////////
  virtual
  void handleCursorPosition (
                             GLFWwindow *pWindow,
                             double      xpos,
                             double      ypos
                             );

  ////////////////////////////////////////////////////////
  /// \brief handleScroll
  /// \param pWindow
  /// \param xoffset
  /// \param yoffset
  ////////////////////////////////////////////////////////
  virtual
  void handleScroll (
                     GLFWwindow *pWindow,
                     double      xoffset,
                     double      yoffset
                     );

  ////////////////////////////////////////////////////////
  /// \brief handleChar
  /// \param pWindow
  /// \param codepoint
  ////////////////////////////////////////////////////////
  virtual
  void handleChar (
                   GLFWwindow *pWindow,
                   unsigned    codepoint
                   );


private:

  std::unique_ptr< shg::Callback > upCallback_;


};


} // namespace shs
