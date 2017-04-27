#include "shared/graphics/Camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#include <cmath>



namespace shg
{


template< typename T >
Camera< T >::Camera( )
  : orbitX_( 0.f )
  , orbitY_( 0.f )
  , zoomZ_( 0.f )
{
  glm::tvec4< T > eye  = glm::tvec4< T >( 0, 0, 5, 0 );
  glm::tvec4< T > look = -eye;   // normalized in "orientLook()"
  glm::tvec4< T > up   = glm::tvec4< T >( 0, 1, 0, 0 );

  orientLook( eye, look, up );

  // Projection Defaults
  heightDegrees_ = 60.f;
  aspectRatio_   = 1.0f;
  near_          = 0.1f;
  far_           = 1000.0f;
  setProjectionMatrix( );

  thirdDist_ = 0.f;
  setFrustumMatrix( );
}



template< typename T >
const glm::tmat4x4< T >&
Camera< T >::getProjectionMatrix( ) const
{
  return proj_;
}



template< typename T >
const glm::tmat4x4< T >&
Camera< T >::getViewMatrix( ) const
{
  return view_;
}



template< typename T >
const glm::tmat4x4< T >&
Camera< T >::getScaleMatrix( ) const
{
  return scale_;
}



template< typename T >
const glm::tmat4x4< T >&
Camera< T >::getScaleViewInvMatrix( ) const
{
  return scaleViewInv_;
}



template< typename T >
const glm::tmat4x4< T >&
Camera< T >::getFrustumMatrix( ) const
{
  return frustum_;
}



template< typename T >
const glm::tvec4< T >&
Camera< T >::getLook( ) const
{
  return look_;
}



template< typename T >
const glm::tvec4< T >&
Camera< T >::getUp( ) const
{
  return up_;
}



template< typename T >
const glm::tvec4< T >&
Camera< T >::getRight( ) const
{
  return right_;
}



template< typename T >
const glm::tvec4< T >&
Camera< T >::getEye( ) const
{
  return eye_;
}



template< typename T >
T
Camera< T >::getAspectRatio( ) const
{
  return aspectRatio_;
}



template< typename T >
void
Camera< T >::setAspectRatio( T a )
{
  aspectRatio_ = a;
  setProjectionMatrix( );
  setFrustumMatrix( );
}



template< typename T >
void
Camera< T >::orientLook(
                        glm::tvec4< T > &eye,
                        glm::tvec4< T > &look,
                        glm::tvec4< T > &up
                        )
{
  // Camera vecs
  eye_  = eye;
  look_ = look;
  up_   = up;

  setCameraSpace( );
  setViewMatrix( );
  setFrustumMatrix( );
}



template< typename T >
void
Camera< T >::moveHorizontal( glm::tvec2< T > dir )
{
  eye_ += glm::normalize( glm::tvec4< T >( look_.x, 0.f, look_.z, 0.f ) ) * dir.x;
  eye_ += glm::normalize( glm::tvec4< T >( -look_.z, 0.f, look_.x, 0.f ) ) * dir.y;
  setViewMatrix( );
  setFrustumMatrix( );
}



template< typename T >
void
Camera< T >::moveAlongU( T mag )
{
  eye_ += u_ * mag;
  setViewMatrix( );
  setFrustumMatrix( );
}



template< typename T >
void
Camera< T >::moveAlongUp( T mag )
{
  eye_ += up_ * mag;
  setViewMatrix( );
  setFrustumMatrix( );
}



template< typename T >
void
Camera< T >::moveAlongLook( T mag )
{
  eye_ += look_ * mag;
  setViewMatrix( );
  setFrustumMatrix( );
}



template< typename T >
void
Camera< T >::pitch( T degrees )
{
  glm::tvec4< T > oldLook = look_;

  look_ =
    glm::rotate( look_, glm::radians( degrees ),
                glm::cross( glm::tvec3< T >( up_ ), glm::tvec3< T >(
                                                                    look_ ) ) );
  setCameraSpace( );

  if ( up_.y < 0 )
  {
    look_ = oldLook;
    setCameraSpace( );
  }

  setViewMatrix( );
  setFrustumMatrix( );
} // >::pitch



template< typename T >
void
Camera< T >::yaw( T degrees )
{
  T radians = glm::radians( degrees );

  glm::tvec3< T > vec = glm::tvec3< T >( 0.f, -1.f, 0.f );

  look_ = glm::rotate( look_, radians, vec );
  up_   = glm::rotate( up_, radians, vec );
  setCameraSpace( );
  setViewMatrix( );
  setFrustumMatrix( );
}



template< typename T >
void
Camera< T >::roll( T degrees )
{
  up_ = glm::rotate( up_, glm::radians( degrees ), glm::tvec3< T >( look_ ) );
  setCameraSpace( );
  setViewMatrix( );
  setFrustumMatrix( );
}



template< typename T >
void
Camera< T >::updateOrbit(
                         T zoomZ,
                         T deltaX,
                         T deltaY
                         )
{
  // x = dy and y = dx because orbits are angles around that axis
  // and deltas are translations in that direction
  zoomZ_  += zoomZ;
  orbitX_ += deltaY;
  orbitY_ += deltaX;

  if ( zoomZ_ < 0.001f )
  {
    zoomZ_ = 0.001f;
  }

  glm::tmat4x4< T > trans =
    glm::rotate( glm::radians( orbitY_ ), glm::tvec3< T >( 0.f, 1.f, 0.f ) )
    * glm::rotate( glm::radians( orbitX_ ), glm::tvec3< T >( 1.f, 0.f, 0.f ) )
    * glm::translate( glm::tvec3< T >( 0.f, 0.f, zoomZ_ ) );


  glm::tvec4< T > eye  = trans * glm::tvec4< T >( 0, 0, 0, 1 );
  glm::tvec4< T > look = -eye;   // normalized in "orientLook()"
  glm::tvec4< T > up   = trans * glm::tvec4< T >( 0, 1, 0, 0 );
  this->orientLook( eye, look, up );

} // updateOrbit



///
/// \brief Camera< T >::buildRayBasisVectors
/// \param pU
/// \param pV
/// \param pW
///
template< typename T >
void
Camera< T >::buildRayBasisVectors(
                                  glm::tvec3< T > *pU,
                                  glm::tvec3< T > *pV,
                                  glm::tvec3< T > *pW
                                  ) const
{

  glm::tvec3< T > &U = *pU;
  glm::tvec3< T > &V = *pV;
  glm::tvec3< T > &W = *pW;

  T ulen, vlen, wlen;
  W = -glm::tvec3< T >( eye_ - glm::tvec4< T >( 0, 3, 0, 1 ) ); // always looking at spot above origin for now

  wlen = glm::length( W );
  U    = glm::normalize( glm::cross( W, glm::tvec3< T >( up_ ) ) );
  V    = glm::normalize( glm::cross( U, W ) );

  ulen = wlen * glm::tan( glm::radians( 0.5f * heightDegrees_ ) );
  U   *= ulen;
  vlen = ulen / aspectRatio_;
  V   *= vlen;

} // >::buildRayBasisVectors



template< typename T >
void
Camera< T >::setCameraSpace( )
{
  // Camera coordinate space
  look_ = glm::normalize( look_ );
  w_    = -look_;
  v_    = glm::normalize( up_ - ( glm::dot( up_, w_ ) * w_ ) );
  u_    = glm::tvec4< T >( glm::normalize( glm::cross(
                                                      glm::tvec3< T >( v_ ),
                                                      glm::tvec3< T >( w_ )
                                                      ) ), 0 );
  up_    = v_;
  right_ = u_;

}



template< typename T >
void
Camera< T >::setViewMatrix( )
{
  // View Matrices
  glm::tmat4x4< T > trans = glm::tmat4x4< T >( );

  trans[ 3 ][ 0 ] = -eye_[ 0 ];
  trans[ 3 ][ 1 ] = -eye_[ 1 ];
  trans[ 3 ][ 2 ] = -eye_[ 2 ];

  // row-major order
  glm::tmat4x4< T > rot = glm::tmat4x4< T >(
                                            u_.x, u_.y, u_.z, 0.0,
                                            v_.x, v_.y, v_.z, 0.0,
                                            w_.x, w_.y, w_.z, 0.0,
                                            0.0,   0.0,   0.0,  1.0
                                            );

  // column-major order for glm and glsl
  rot = glm::transpose( rot );

  view_ = rot * trans;

  scaleViewInv_ = glm::inverse( scale_ * view_ );
} // setViewMatrix



template< typename T >
void
Camera< T >::setProjectionMatrix( )
{
  // Projection Matrices
  T h = far_ * glm::tan( glm::radians( heightDegrees_ / 2.0f ) );
  T w = aspectRatio_ * h;

  // row-major order
  scale_ = glm::tmat4x4< T >(
                             1.0 / w,       0.0,          0.0,        0.0,
                             0.0,        1.0 / h,         0.0,        0.0,
                             0.0,           0.0,       1.0 / far_,   0.0,
                             0.0,           0.0,          0.0,        1.0
                             );

  // column-major order for glm and glsl
  scale_ = glm::transpose( scale_ );

  T c = -near_ / far_;

  // row-major order
  glm::tmat4x4< T > perspective
    = glm::tmat4x4< T >(
                        1.0, 0.0,         0.0,               0.0,
                        0.0, 1.0,         0.0,               0.0,
                        0.0, 0.0, -1.0 / ( 1.0 + c ), c / ( 1.0 + c ),
                        0.0, 0.0,        -1.0,               0.0
                        );

  // column-major order for glm and glsl
  perspective = glm::transpose( perspective );

  proj_ = perspective * scale_;

  scaleViewInv_ = glm::inverse( scale_ * view_ );
} // setProjectionMatrix



template< typename T >
void
Camera< T >::setFrustumMatrix( )
{
  frustum_ = glm::transpose( proj_ * view_ );
}



template class Camera< float >;
template class Camera< double >;


} // namespace shg
