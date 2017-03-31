#include "CubeIOHandler.hpp"

#include <iostream>
#include <deque>
#include <memory>

#include "CubeWorld.hpp"
#include "RotatingCube.hpp"


namespace simple
{


/////////////////////////////////////////////
/// \brief Renderer::Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
CubeIOHandler::CubeIOHandler( CubeWorld &cubeWorld )
  : shared::IOHandler( cubeWorld )
  , cubeWorld_( cubeWorld )
{
  cubeWorld_.addRandomCube();
  cubeWorld_.addRandomCube();
  cubeWorld_.addRandomCube();
}



/////////////////////////////////////////////
/// \brief Renderer::~Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
CubeIOHandler::~CubeIOHandler( )
{
  // final new line
  std::cout << std::endl;
}



/////////////////////////////////////////////
/// \brief Renderer::render
/// \param alpha
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
CubeIOHandler::showWorld( const double )
{
  const std::deque< std::unique_ptr< RotatingCube > > &cubes = cubeWorld_.getCubes( );

  std::cout << "\r(Cube id, rotations) : ";

  for ( auto & upCube : cubes )
  {
    std::cout << "(" << upCube->id( ) << ",";
    std::cout << upCube->getRotations( ) << ") ; " << std::flush;
  }
} // CubeIOHandler::showWorld



} // namespace simple
