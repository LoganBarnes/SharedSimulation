// TCameraMover.hpp
#pragma once

#include <shared/graphics/GraphicsForwardDeclarations.hpp>
#include <glm/glm.hpp>


namespace shg
{

template< typename T >
class TCameraMover
{
public:

  explicit
  TCameraMover( TCamera< T > &camera);

  void update();

  void setYawDegrees  ( T yawDegrees );
  void setPitchDegrees( T pitchDegrees );
  void setOffsetDist  ( T offsetDist );
  void setOrbitPoint  ( glm::tvec3< T > orbitPoint );

  void setMoveAmount   ( T moveAmount );
  void addMoveDirection( glm::ivec3 forwardRightUp );

private:
  TCamera< T > &camera_;
  T yawDegrees_;
  T pitchDegrees_;
  T offsetDist_;
  glm::tvec3< T > orbitPoint_;

  T          moveAmt_;
  glm::ivec3 moveDir_;

};


using CameraMover  = TCameraMover< float >;
using CameraMoverD = TCameraMover< double >;

} // namespace shg
