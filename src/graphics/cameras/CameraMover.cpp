#include <shared/graphics/CameraMover.hpp>
#include <shared/graphics/Camera.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>



namespace shg
{

///
/// \brief TCameraMover<T>::TCameraMover
///
template< typename T >
TCameraMover< T >::TCameraMover( TCamera< T > &camera )
  : camera_    ( camera )
  , yawDegrees_  ( 0 )
  , pitchDegrees_( 0 )
  , offsetDist_  ( 0 )
  , orbitPoint_  ( 0 )
  , moveAmt_     ( 1 )
  , moveDir_     ( 0 )
{}

template< typename T >
void
TCameraMover< T >::update( )
{
  glm::tvec3< T > look ( 0, 0, -1 );
  look = glm::rotate( look,  glm::radians( pitchDegrees_ ), glm::tvec3< T >( 1,  0, 0 ) );
  look = glm::rotate( look,  glm::radians( yawDegrees_ ),   glm::tvec3< T >( 0, -1, 0 ) );

  //
  // orbit point update
  //
  if ( moveDir_.x != 0 || moveDir_.y != 0 || moveDir_.z != 0 )
  {
    glm::tvec3< T > moveDir = glm::normalize( glm::tvec3< T >( moveDir_ ) );

    glm::tvec3< T > right( 1, 0,  0 );
    right = glm::rotate( right, glm::radians( yawDegrees_ ),   glm::tvec3< T >(  0, -1, 0 ) );
    glm::tvec3< T > up ( glm::cross( right, look ) );

    orbitPoint_ += look  * moveDir.x;
    orbitPoint_ += right * moveDir.y;
    orbitPoint_ += up    * moveDir.z;
  }

  glm::tvec3< T > point = orbitPoint_ + look;
  glm::tvec3< T > eye   = orbitPoint_ - look * offsetDist_;

  camera_.lookAt( eye, point );
} // >::update



template< typename T >
void
TCameraMover< T >::setYawDegrees( T yawDegrees )
{
  yawDegrees_ = yawDegrees;
}



template< typename T >
void
TCameraMover< T >::setPitchDegrees( T pitchDegrees )
{
  pitchDegrees_ = glm::clamp( pitchDegrees, T( -89 ), T( 89 ) );
}



template< typename T >
void
TCameraMover< T >::setOffsetDist( T offsetDist )
{
  offsetDist_ = glm::max( T( 0 ), offsetDist );
}



template< typename T >
void
TCameraMover< T >::setOrbitPoint( glm::tvec3< T > orbitPoint )
{
  orbitPoint_ = orbitPoint;
}



template< typename T >
void
TCameraMover< T >::addYawDegrees( T yawDegrees )
{
  setYawDegrees( yawDegrees_ + yawDegrees );
}



template< typename T >
void
TCameraMover< T >::addPitchDegrees( T pitchDegrees )
{
  setPitchDegrees( pitchDegrees_ + pitchDegrees );
}



template< typename T >
void
TCameraMover< T >::addOffsetDist( T offsetDist )
{
  setOffsetDist( offsetDist_ + offsetDist );
}



template< typename T >
void
TCameraMover< T >::setMoveAmount( T moveAmt )
{
  moveAmt_ = moveAmt;
}



template< typename T >
void
TCameraMover< T >::addMoveDirection( glm::ivec3 forwardRightUp )
{
  moveDir_ = glm::clamp( moveDir_ + forwardRightUp, -1, 1 );
}



template class TCameraMover< float >;
template class TCameraMover< double >;

} // namespace shg
