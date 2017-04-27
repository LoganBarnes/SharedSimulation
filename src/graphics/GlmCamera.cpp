#include "shared/graphics/GlmCamera.hpp"

#define GLFORCE_RADIANS_
#include "glm/gtc/matrix_transform.hpp"



namespace shg
{

///
/// \brief GlmCamera<T>::GlmCamera
///
template< typename T >
GlmCamera< T >::GlmCamera( )
{
  lookAt(
         glm::tvec3< T >( 0.0, 2.0, 5.0 ),
         glm::tvec3< T >( 0.0, 0.0, 0.0 )
         );
  perspective( 60.0, 1.0, 1.0, 1000.0 );
  ortho( -1.0, 1.0, -1.0, 1.0 );
}



///
/// \brief GlmCamera<T>::lookAt
/// \param eye
/// \param point
/// \param up
///
template< typename T >
void
GlmCamera< T >::lookAt(
                       const glm::tvec3< T > &eye,
                       const glm::tvec3< T > &point,
                       const glm::tvec3< T > &up
                       )
{
  eye_         = eye;
  lookVector_  = glm::normalize( point - eye_ );
  upVector_    = up;
  rightVector_ = glm::cross( lookVector_, upVector_ );
  viewMatrix_  = glm::lookAt( eye_, point, upVector_ );

  perspectiveProjectionViewMatrix_ = perspectiveMatrix_ * viewMatrix_;
}



///
/// \brief GlmCamera<T>::perspective
/// \param fovyDegrees
/// \param aspect
/// \param zNear
/// \param zFar
///
template< typename T >
void
GlmCamera< T >::perspective(
                            T fovyDegrees,
                            T aspect,
                            T zNear,
                            T zFar
                            )
{
  fovYDegrees_       = fovyDegrees;
  fovYRadians_       = glm::radians( fovYDegrees_ );
  aspectRatio_       = aspect;
  nearPlane_         = zNear;
  farPlane_          = zFar;
  perspectiveMatrix_ = glm::perspective( fovYRadians_, aspectRatio_, nearPlane_, farPlane_ );

  perspectiveProjectionViewMatrix_ = perspectiveMatrix_ * viewMatrix_;
}



///
/// \brief GlmCamera<T>::ortho
/// \param left
/// \param right
/// \param bottom
/// \param top
///
template< typename T >
void
GlmCamera< T >::ortho(
                      T left,
                      T right,
                      T bottom,
                      T top
                      )
{
  orthoLeft_          = left;
  orthoRight_         = right;
  orthoBottom_        = bottom;
  orthoTop_           = top;
  orthographicMatrix_ = glm::ortho( orthoLeft_, orthoRight_, orthoBottom_, orthoTop_ );

  perspectiveProjectionViewMatrix_ = perspectiveMatrix_ * viewMatrix_;
}



///
/// \brief GlmCamera<T>::setEye
/// \param eye
///
template< typename T >
void
GlmCamera< T >::setEye( const glm::tvec3< T > &eye )
{
  lookAt( eye, eye + lookVector_, upVector_ );
}



///
/// \brief GlmCamera<T>::setLook
/// \param look
///
template< typename T >
void
GlmCamera< T >::setLook( const glm::tvec3< T > &look )
{
  lookAt( eye_, eye_ + look, upVector_ );
}



///
/// \brief GlmCamera<T>::setUp
/// \param up
///
template< typename T >
void
GlmCamera< T >::setUp( const glm::tvec3< T > &up )
{
  lookAt( eye_, eye_ + lookVector_, up );
}



///
/// \brief GlmCamera<T>::setFovYDegrees
/// \param fovYDegrees
///
template< typename T >
void
GlmCamera< T >::setFovYDegrees( T fovYDegrees )
{
  perspective( fovYDegrees, aspectRatio_, nearPlane_, farPlane_ );
}



///
/// \brief GlmCamera<T>::setAspectRatio
/// \param aspectRatio
///
template< typename T >
void
GlmCamera< T >::setAspectRatio( T aspectRatio )
{
  perspective( fovYDegrees_, aspectRatio, nearPlane_, farPlane_ );
}



///
/// \brief GlmCamera<T>::setNearPlaneDistance
/// \param near
///
template< typename T >
void
GlmCamera< T >::setNearPlaneDistance( T near )
{
  perspective( fovYDegrees_, aspectRatio_, near, farPlane_ );
}



///
/// \brief GlmCamera<T>::setFarPlaneDistance
/// \param far
///
template< typename T >
void
GlmCamera< T >::setFarPlaneDistance( T far )
{
  perspective( fovYDegrees_, aspectRatio_, nearPlane_, far );
}



///
/// \brief GlmCamera<T>::setOrthoLeft
/// \param left
///
template< typename T >
void
GlmCamera< T >::setOrthoLeft( T left )
{
  ortho( left, orthoRight_, orthoBottom_, orthoTop_ );
}



///
/// \brief GlmCamera<T>::setOrthoRight
/// \param right
///
template< typename T >
void
GlmCamera< T >::setOrthoRight( T right )
{
  ortho( orthoLeft_, right, orthoBottom_, orthoTop_ );
}



///
/// \brief GlmCamera<T>::setOrthoBottom
/// \param bottom
///
template< typename T >
void
GlmCamera< T >::setOrthoBottom( T bottom )
{
  ortho( orthoLeft_, orthoRight_, bottom, orthoTop_ );
}



///
/// \brief GlmCamera<T>::setOrthoTop
/// \param top
///
template< typename T >
void
GlmCamera< T >::setOrthoTop( T top )
{
  ortho( orthoLeft_, orthoRight_, orthoBottom_, top );
}



template class GlmCamera< float >;
template class GlmCamera< double >;


} // namespace shg
