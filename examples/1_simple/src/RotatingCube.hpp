// RotatingCube.hpp
#pragma once


#include "glm/glm.hpp"


namespace simple
{


/////////////////////////////////////////////
/// \brief The RotatingCube class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class RotatingCube
{

public:

  ///////////////////////////////////////////////////////////////
  /// \brief RotatingCube
  ///////////////////////////////////////////////////////////////
  RotatingCube(
               const glm::vec3 axis,
               const float     rotateRate,
               const unsigned  id
               );


  ///////////////////////////////////////////////////////////////
  /// \brief ~RotatingCube
  ///////////////////////////////////////////////////////////////
  ~RotatingCube( );


  ///////////////////////////////////////////////////////////////
  /// \brief update
  ///////////////////////////////////////////////////////////////
  void update (
               const double worldTime, ///< update to this time
               const double timestep   ///< interval since last update
               );

  unsigned
  getRotations( ) const { return rotations_; }

  unsigned
  id( ) const { return id_; }


private:

  const glm::vec3 axis_;
  const float rotateRate_;
  const unsigned id_;

  float angle_;
  unsigned rotations_;


};


} // end namespace simple
