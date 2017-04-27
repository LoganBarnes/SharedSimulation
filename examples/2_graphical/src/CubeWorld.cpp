#include "CubeWorld.hpp"
#include "RotatingCube.hpp"

#include <glm/gtc/constants.hpp>
#include <random>

namespace example
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
  : shs::World( )
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
  for ( auto &upCube : cubes_ )
  {
    upCube->update( worldTime, timestep );
  }
}



void
CubeWorld::addRandomCube( )
{
  constexpr float scale = glm::pi< float >( );
  const glm::vec3 axis  = glm::normalize( glm::vec3(
                                                    realDist( randGen ),
                                                    realDist( randGen ),
                                                    realDist( randGen )
                                                    ) - 0.5f );

  std::unique_ptr< RotatingCube > cube( new RotatingCube(
                                                         axis,
                                                         realDist( randGen ) * scale,
                                                         currentId_++
                                                         ) );

  cube->setPosition( glm::vec3(
                               realDist( randGen ),
                               realDist( randGen ),
                               realDist( randGen )
                               ) * 10.0f - 5.0f );

  cubes_.push_back( std::move( cube ) );
} // CubeWorld::addRandomCube



void
CubeWorld::removeOldestCube( )
{
  if ( !cubes_.empty( ) )
  {
    cubes_.pop_front( );
  }
}



} // namespace example
