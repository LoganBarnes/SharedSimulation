#include "shared/graphics/ImguiCallback.hpp"
#include "GLFW/glfw3.h"
#include "imgui.h"


namespace shared
{



ImguiCallback::ImguiCallback( std::unique_ptr< Callback > upCallback )
  : graphics::Callback( )
  , upCallback_( std::move( upCallback ) )
{}



ImguiCallback::~ImguiCallback( )
{}



///
/// \brief ImguiCallback::setCallback
/// \param upCallback
///
void
ImguiCallback::setCallback( std::unique_ptr< graphics::Callback > upCallback )
{
  if ( upCallback )
  {
    if ( upCallback_ )
    {
      upCallback_.release( );
    }

    upCallback_ = std::move( upCallback );
  }
} // ImguiCallback::setCallback



////////////////////////////////////////////////////////
/// \brief handleWindowSize
/// \param pWindow
/// \param width
/// \param height
////////////////////////////////////////////////////////
void
ImguiCallback::handleWindowSize(
                                GLFWwindow *pWindow,
                                int         width,
                                int         height
                                )
{
  if ( upCallback_ )
  {
    upCallback_->handleWindowSize( pWindow, width, height );
  }
} // ImguiCallback::handleWindowSize



////////////////////////////////////////////////////////
/// \brief handleMouseButton
/// \param pWindow
/// \param button
/// \param action
/// \param mods
////////////////////////////////////////////////////////
void
ImguiCallback::handleMouseButton(
                                 GLFWwindow *pWindow,
                                 int         button,
                                 int         action,
                                 int         mods
                                 )
{
  ImGuiIO &io = ImGui::GetIO( );

  if ( !io.WantCaptureMouse && upCallback_ )
  {
    upCallback_->handleMouseButton( pWindow, button, action, mods );
  }
  else
  if ( io.WantCaptureMouse )
  {
    if ( action == GLFW_PRESS && button >= 0 && button < 3 )
    {
      io.MouseDown[ button ] = true;
    }
  }
} // ImguiCallback::handleMouseButton



////////////////////////////////////////////////////////
/// \brief handleKey
/// \param pWindow
/// \param key
/// \param scancode
/// \param action
/// \param mods
////////////////////////////////////////////////////////
void
ImguiCallback::handleKey(
                         GLFWwindow *pWindow,
                         int         key,
                         int         scancode,
                         int         action,
                         int         mods
                         )
{
  ImGuiIO &io = ImGui::GetIO( );

  if ( !io.WantCaptureKeyboard && upCallback_ )
  {
    upCallback_->handleKey( pWindow, key, scancode, action, mods );
  }
  else
  if ( io.WantCaptureKeyboard )
  {
    if ( action == GLFW_PRESS )
    {
      io.KeysDown[ key ] = true;
    }
    else
    if ( action == GLFW_RELEASE )
    {
      io.KeysDown[ key ] = false;
    }

    io.KeyCtrl  = io.KeysDown[ GLFW_KEY_LEFT_CONTROL ] || io.KeysDown[ GLFW_KEY_RIGHT_CONTROL ];
    io.KeyShift = io.KeysDown[ GLFW_KEY_LEFT_SHIFT   ] || io.KeysDown[ GLFW_KEY_RIGHT_SHIFT   ];
    io.KeyAlt   = io.KeysDown[ GLFW_KEY_LEFT_ALT     ] || io.KeysDown[ GLFW_KEY_RIGHT_ALT     ];
    io.KeySuper = io.KeysDown[ GLFW_KEY_LEFT_SUPER   ] || io.KeysDown[ GLFW_KEY_RIGHT_SUPER   ];
  }
} // ImguiCallback::handleKey



////////////////////////////////////////////////////////
/// \brief handleCursorPosition
/// \param pWindow
/// \param xpos
/// \param ypos
////////////////////////////////////////////////////////
void
ImguiCallback::handleCursorPosition(
                                    GLFWwindow *pWindow,
                                    double      xpos,
                                    double      ypos
                                    )
{
  ImGuiIO &io = ImGui::GetIO( );

  if ( !io.WantCaptureMouse && upCallback_ )
  {
    upCallback_->handleCursorPosition( pWindow, xpos, ypos );
  }
} // ImguiCallback::handleCursorPosition



////////////////////////////////////////////////////////
/// \brief handleScroll
/// \param pWindow
/// \param xoffset
/// \param yoffset
////////////////////////////////////////////////////////
void
ImguiCallback::handleScroll(
                            GLFWwindow *pWindow,
                            double      xoffset,
                            double      yoffset
                            )
{
  ImGuiIO &io = ImGui::GetIO( );

  if ( !io.WantCaptureMouse && upCallback_ )
  {
    upCallback_->handleScroll( pWindow, xoffset, yoffset );
  }
  else
  if ( io.WantCaptureMouse )
  {
    io.MouseWheel += static_cast< float >( yoffset ); // the fractional mouse wheel. 1.0 unit 5 lines
  }
} // ImguiCallback::handleScroll



////////////////////////////////////////////////////////
/// \brief handleChar
/// \param pWindow
/// \param codepoint
////////////////////////////////////////////////////////
void
ImguiCallback::handleChar(
                          GLFWwindow*,
                          unsigned codepoint
                          )
{
  ImGuiIO &io = ImGui::GetIO( );

  if ( io.WantCaptureKeyboard && codepoint > 0 && codepoint < 0x10000 )
  {
    io.AddInputCharacter( static_cast< unsigned short >( codepoint ) );
  }
} // ImguiCallback::handleChar



// ////////////////////////////////////////////////////////
// /// \brief handleWindowRefresh
// /// \param pWindow
// ////////////////////////////////////////////////////////
// void
// ImguiCallback::handleWindowRefresh( GLFWwindow* )
// {
//   if ( pEventDrivenIO_ )
//   {
//     pEventDrivenIO_->showWorld( );
//   }
// } // ImguiCallback::handleWindowRefresh



} // namespace shared
