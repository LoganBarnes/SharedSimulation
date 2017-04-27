#include "shared/core/EventDriver.hpp"

#include "shared/core/World.hpp"
#include "shared/core/IOHandler.hpp"

#include <iostream>


namespace shs
{


namespace
{

const double MAX_TIMESTEP_NFTR_LOOP = 0.05;

}


/////////////////////////////////////////////
/// \brief EventDriver::EventDriver
/// \param world
///
/// \author Logan Barnes
/////////////////////////////////////////////
EventDriver::EventDriver(
                         World     &world,
                         IOHandler &ioHandler
                         ) noexcept
  : Driver( world, ioHandler )
{
  ioHandler_.setEventBased( true );
}



/////////////////////////////////////////////
/// \brief EventDriver::exec
/// \return
///
/// \author Logan Barnes
/////////////////////////////////////////////
int
EventDriver::exec(
                  int          argc, ///< number of arguments
                  const char **argv ///< array of argument strings
                  )
{

  for ( int i = 1; i < argc; ++i )
  {
    {
      std::cerr << "WARNING: Unknown argument given: " << argv[ i ] << std::endl;
    }
  }

  _runEventLoop( );

  ioHandler_.onLoopExit( );

  return 0;

} // EventDriver::exec



/////////////////////////////////////////////
/// \brief EventDriver::_runEventLoop
///
///        Waits for event callbacks
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
EventDriver::_runEventLoop( )
{
  do
  {
    ioHandler_.showWorld( 1.0 );

    // check for input
    ioHandler_.waitForIO( );
  }
  while ( !ioHandler_.isExitRequested( ) );

} // EventDriver::_runEventLoop



} // namespace shs
