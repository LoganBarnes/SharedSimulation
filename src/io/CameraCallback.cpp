// CameraCallback.cpp
#include <shared/graphics/CameraCallback.hpp>
#include <shared/graphics/CameraMover.hpp>
#include <shared/graphics/Camera.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>



namespace shs
{


template< typename T >
TCameraCallback< T >::TCameraCallback( shg::TCameraMover< T > &cameraMover )
  : SharedCallback ( )
  , cameraMover_ ( cameraMover )
{}


template< typename T >
TCameraCallback< T >::~TCameraCallback( ) {}


template< typename T >
void
TCameraCallback< T >::handleKey(
                                GLFWwindow *pWindow,
                                int         key,
                                int         scancode,
                                int         action,
                                int         mods
                                )
{
  shs::SharedCallback::handleKey( pWindow, key, scancode, action, mods );

  switch ( key )
  {

  case GLFW_KEY_R:
    cameraMover_.setOrbitPoint( glm::tvec3< T >( 0 ) );
    break;

  case GLFW_KEY_W:

    if ( action == GLFW_PRESS )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 1, 0, 0 ) );
    }
    else
    if ( action == GLFW_RELEASE )
    {
      cameraMover_.addMoveDirection( glm::ivec3( -1, 0, 0 ) );
    }

    break;

  case GLFW_KEY_A:

    if ( action == GLFW_PRESS )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 0, -1, 0 ) );
    }
    else
    if ( action == GLFW_RELEASE )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 0, 1, 0 ) );
    }

    break;

  case GLFW_KEY_S:

    if ( action == GLFW_PRESS )
    {
      cameraMover_.addMoveDirection( glm::ivec3( -1, 0, 0 ) );
    }
    else
    if ( action == GLFW_RELEASE )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 1, 0, 0 ) );
    }

    break;

  case GLFW_KEY_D:

    if ( action == GLFW_PRESS )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 0, 1, 0 ) );
    }
    else
    if ( action == GLFW_RELEASE )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 0, -1, 0 ) );
    }

    break;

  case GLFW_KEY_E:

    if ( action == GLFW_PRESS )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 0, 0, 1 ) );
    }
    else
    if ( action == GLFW_RELEASE )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 0, 0, -1 ) );
    }

    break;

  case GLFW_KEY_Q:

    if ( action == GLFW_PRESS )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 0, 0, -1 ) );
    }
    else
    if ( action == GLFW_RELEASE )
    {
      cameraMover_.addMoveDirection( glm::ivec3( 0, 0, 1 ) );
    }

    break;

  default:
    break;
  } // switch

} // CameraCallback< T >::handleKey



template< typename T >
void
TCameraCallback< T >::handleCursorPosition(
                                           GLFWwindow*/*pWindow*/,
                                           double xpos,
                                           double ypos
                                           )
{
  if ( leftMouseDown_ )
  {
    cameraMover_.addPitchDegrees( ( prevY_ - ypos ) * static_cast< T >( 0.25 ) );
    cameraMover_.addYawDegrees  ( ( xpos - prevX_ ) * static_cast< T >( 0.25 ) );
  }
  else
  if ( rightMouseDown_ )
  {}

  prevX_ = xpos;
  prevY_ = ypos;
} // handleCursorPosition



template< typename T >
void
TCameraCallback< T >::handleScroll(
                                   GLFWwindow*,
                                   double,
                                   double yoffset
                                   )
{
  cameraMover_.addOffsetDist( static_cast< T >( -yoffset * 0.5 ) );
}



template class TCameraCallback< float >;
template class TCameraCallback< double >;

} //  namespace shs
