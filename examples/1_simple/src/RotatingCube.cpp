// RotatingCube.cpp

#include "RotatingCube.hpp"
#include "glm/gtx/transform.hpp"


namespace simple
{

///////////////////////////////////////////////////////////////
/// \brief RotatingCube
///////////////////////////////////////////////////////////////
RotatingCube::RotatingCube(
                           const glm::vec3 axis,
                           const float     rotateRate,
                           const unsigned  id
                           )
  : axis_                ( axis )
  , rotateRate_          ( rotateRate )
  , id_                  ( id )
  , angle_               ( 0.0f )
  , rotations_           ( 0 )
{}



///////////////////////////////////////////////////////////////
/// \brief ~RotatingCube
///////////////////////////////////////////////////////////////
RotatingCube::~RotatingCube( )
{}


///////////////////////////////////////////////////////////////
/// \brief update
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
}

} // end namespace simple

