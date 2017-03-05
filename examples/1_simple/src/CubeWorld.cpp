#include "CubeWorld.hpp"

#include <random>

#include "glm/gtc/constants.hpp"

#include "RotatingCube.hpp"

namespace simple
{

namespace
{
std::default_random_engine randGen;
std::uniform_real_distribution< float > realDist( 0.0, 1.0 );
}

/////////////////////////////////////////////
/// \brief CubeWorld::CubeWorld
///
/// \author Logan Barnes
/////////////////////////////////////////////
CubeWorld::CubeWorld( )
  : shared::World( )
  , currentId_( 0 )
  , cubes_( )
{}



CubeWorld::~CubeWorld( ) {}


/////////////////////////////////////////////
/// \brief CubeWorld::update
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
CubeWorld::update(
                  const double worldTime, ///< update to this time
                  const double timestep   ///< interval since last update
                  )
{
  for ( auto & upCube : cubes_ )
  {
    upCube->update( worldTime, timestep );
  }
}



void
CubeWorld::addRandomCube( )
{
  constexpr float scale = glm::pi< float >();
  const glm::vec3 axis = glm::normalize( glm::vec3(
                                                   realDist( randGen ),
                                                   realDist( randGen ),
                                                   realDist( randGen )
                                                   ) - 0.5f );

  cubes_.push_back( std::unique_ptr< RotatingCube >( new RotatingCube(
                                                                      axis,
                                                                      realDist( randGen ) * scale,
                                                                      currentId_++
                                                                      ) ) );
}



void
CubeWorld::removeOldestCube( )
{
  cubes_.pop_front( );
}



} // namespace simple
