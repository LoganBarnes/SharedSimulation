// RotatingCube.hpp
#pragma once


#include <glm/glm.hpp>


namespace example
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
               const float rotateRate,
               const unsigned  id
               );


  ///////////////////////////////////////////////////////////////
  /// \brief ~RotatingCube
  ///////////////////////////////////////////////////////////////
  ~RotatingCube( ) = default;


  ///////////////////////////////////////////////////////////////
  /// \brief update
  ///////////////////////////////////////////////////////////////
  void update (
               const double worldTime, ///< update to this time
               const double timestep   ///< interval since last update
               );

  void setPosition( const glm::vec3 pos );

  unsigned
  getRotations( ) const { return rotations_; }

  unsigned
  id( ) const { return id_; }

  const glm::mat4&
  getTransformationMatrix( ) const { return transformationMatrix_; }


private:

  const glm::vec3 axis_;
  const float rotateRate_;
  const unsigned id_;

  float angle_;
  unsigned rotations_;

  glm::mat4 translateMatrix_;
  glm::mat4 rotateMatrix_;
  glm::mat4 transformationMatrix_;

};


} // end namespace example

