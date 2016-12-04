#include <iostream>
#include <exception>

#include "SubtractiveTerrainConfig.hpp"
#include "Driver.hpp"
#include "IOHandler.hpp"
#include "World.hpp"


/////////////////////////////////////////////
/// \brief main
/// \return
///
/// \author Logan Barnes
/////////////////////////////////////////////
int
main(
     int          argc, ///< number of arguments
     const char **argv  ///< array of argument strings
     )
{

  srt::Driver::printProjectInfo(
                                srt::PROJECT_NAME,
                                srt::VERSION_MAJOR,
                                srt::VERSION_MINOR,
                                srt::VERSION_PATCH
                                );

  try
  {

    //
    // create world to handle physical updates
    // and ioHandler to interface between the
    // world and the user
    //
    srt::World     world;
    srt::IOHandler renderer( world );

    //
    // pass world and ioHandler to driver
    // to manage update loops
    //
    srt::Driver driver( world, renderer );

    //
    // run program
    //
    return driver.exec( argc, argv );

  }
  catch ( std::exception &e )
  {

    std::cerr << "ERROR: program failed: " << e.what( ) << std::endl;

    return EXIT_FAILURE;

  }

} // main
