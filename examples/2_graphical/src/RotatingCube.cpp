// RotatingCube.cpp

#include "RotatingCube.hpp"
#include "glm/gtx/transform.hpp"


namespace example
{

///////////////////////////////////////////////////////////////
/// \brief RotatingCube
///////////////////////////////////////////////////////////////
RotatingCube::RotatingCube(
                           const glm::vec3 axis,
                           const float rotateRate,
                           const unsigned  id
                           )
  : axis_                ( axis )
  , rotateRate_          ( rotateRate )
  , id_                  ( id )
  , angle_               ( 0.0f )
  , rotations_           ( 0 )
  , translateMatrix_     ( 1.0f )
  , rotateMatrix_        ( 1.0f )
  , transformationMatrix_( 1.0f )
{}


///////////////////////////////////////////////////////////////
/// \brief update
/// \param worldTime
/// \param timestep
///////////////////////////////////////////////////////////////
void
RotatingCube::update (
                      const double,           ///< update to this time
                      const double timestep   ///< interval since last update
                      )
{
  constexpr float twoPi = glm::pi< float >() * 2.0f;
  const float rotateAmount = rotateRate_ * static_cast< float >( timestep );
  angle_ += rotateAmount;

  while ( angle_ > twoPi )
  {
    angle_ -= twoPi;
    ++rotations_;
  }

  rotateMatrix_ = glm::rotate(
                              rotateMatrix_,
                              rotateAmount,
                              axis_
                              );

  transformationMatrix_ = translateMatrix_ * rotateMatrix_;
}



///////////////////////////////////////////////////////////////
/// \brief setPosition
/// \param pos
///////////////////////////////////////////////////////////////
void
RotatingCube::setPosition( const glm::vec3 pos )
{
  translateMatrix_ = glm::translate( glm::mat4( ), pos );
  transformationMatrix_ = translateMatrix_ * rotateMatrix_;
}


} // end namespace example

