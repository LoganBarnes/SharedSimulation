// ImguiCallback.hpp
#pragma once


#include <memory>
#include "shared/Callback.hpp"


namespace shared
{

class IOHandler;


class ImguiCallback : public graphics::Callback
{

public:

  ImguiCallback(
                std::unique_ptr< graphics::Callback > upCallback = nullptr,
                IOHandler                            *pEventDrivenIO = nullptr
                );

  ~ImguiCallback( );


  ////////////////////////////////////////////////////////
  /// \brief setCallback
  /// \param upCallback
  ////////////////////////////////////////////////////////
  void setCallback ( std::unique_ptr< graphics::Callback > upCallback );


  ////////////////////////////////////////////////////////
  /// \brief setEventHandler
  /// \param pEventDrivenIO
  ////////////////////////////////////////////////////////
  void setEventHandler ( IOHandler *pEventDrivenIO );

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

  std::unique_ptr< graphics::Callback > upCallback_;
  IOHandler *pEventDrivenIO_;


};


} // namespace shared

