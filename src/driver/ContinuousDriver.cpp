#include "ContinuousDriver.hpp"

#include <iostream>

#include "world/World.hpp"
#include "io/IOHandler.hpp"



namespace shared
{


namespace
{

const double MAX_TIMESTEP_NFTR_LOOP = 0.05;

}


/////////////////////////////////////////////
/// \brief ContinuousDriver::ContinuousDriver
/// \param world
///
/// \author Logan Barnes
/////////////////////////////////////////////
ContinuousDriver::ContinuousDriver(
               World     &world,
               IOHandler &ioHandler
               ) noexcept
  :
  Driver( world, ioHandler )
  , timeScale_  ( 1.0 )
  , paused_     ( false )
  , startTime_  ( 0.0 )
  , timeStep_   ( 0.1 )
  , worldTime_  ( startTime_ )
  , updateFrame_( 0 )
  , initTime_   ( std::chrono::system_clock::now( ) )
{}



/////////////////////////////////////////////
/// \brief ContinuousDriver::exec
/// \return
///
/// \author Logan Barnes
/////////////////////////////////////////////
int
ContinuousDriver::exec(
             int          argc, ///< number of arguments
             const char **argv  ///< array of argument strings
             )
{

  bool useDefaultLoop( true );

  for ( int i = 1; i < argc; ++i )
  {

    if ( std::string( argv[ i ] ) == "-r" )
    {

      useDefaultLoop = false;

    }
    else
    {

      std::cerr << "WARNING: Unknown argument given: " << argv[ i ] << std::endl;

    }

  }

  if ( useDefaultLoop )
  {

    _runAFAPLoop( );

  }
  else
  {

    _runNFTRLoop( );

  }

  ioHandler_.onLoopExit( );

  return 0;

} // ContinuousDriver::exec



/////////////////////////////////////////////
/// \brief ContinuousDriver::_runAFAPLoop
///
///        Runs sim As Fast As Possible
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
ContinuousDriver::_runAFAPLoop( )
{

  while ( !ioHandler_.isExitRequested( ) )
  {

    // check for input
    ioHandler_.updateIO( );

    if ( !paused_ )
    {

      world_.update( worldTime_, timeStep_ );
      ++updateFrame_;
      worldTime_ = updateFrame_ * timeStep_ + startTime_;

    }

    ioHandler_.showWorld( 1.0 );

    ///\todo print timing info

  }

} // ContinuousDriver::_runAFAPLoop



/////////////////////////////////////////////
/// \brief ContinuousDriver::_runNFTRLoop
///
///        Runs sim No Faster Than Real-time
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
ContinuousDriver::_runNFTRLoop( )
{

  //
  // physics game loop:
  // http://gafferongames.com/game-physics/fix-your-timestep/
  //

  double currentTime = _getTimeSeconds( );
  double accumulator = 0.0;

  double deltaTime = timeStep_ * timeScale_;

  double newTime, frameTime, alpha;

  //
  // Loop until the user closes the window
  //
  while ( !ioHandler_.isExitRequested( ) )
  {

    // check for input
    ioHandler_.updateIO( );

    if ( !paused_ )
    {

      newTime   = _getTimeSeconds( );
      frameTime = newTime - currentTime;

      if ( frameTime > MAX_TIMESTEP_NFTR_LOOP )
      {

        frameTime = MAX_TIMESTEP_NFTR_LOOP;

      }

      currentTime = newTime;

      accumulator += frameTime;

      deltaTime = timeStep_ * timeScale_;

      while ( accumulator >= deltaTime )
      {

        world_.update( worldTime_, deltaTime );

        worldTime_  += deltaTime;
        accumulator -= deltaTime;

      }

    }
    else
    {

      currentTime = _getTimeSeconds( );

    }

    alpha = accumulator / deltaTime;

    ioHandler_.showWorld( alpha );

  }

} // ContinuousDriver::_runNFTRLoop



/////////////////////////////////////////////
/// \brief ContinuousDriver::_getTimeSeconds
///
/// \author Logan Barnes
/////////////////////////////////////////////
double
ContinuousDriver::_getTimeSeconds( )
{

  //
  // current time
  //
  std::chrono::time_point< std::chrono::system_clock > timeNow;
  timeNow = std::chrono::system_clock::now( );

  //
  // time since initialization
  //
  std::chrono::duration< float > timeElapsedSecs = timeNow - initTime_;
  return timeElapsedSecs.count( );

}


} // namespace shared
