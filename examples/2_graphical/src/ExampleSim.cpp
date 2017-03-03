// ExampleSim.cpp
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "driver/ContinuousDriver.hpp"
#include "world/World.hpp"
#include "io/ImguiOpenGLIOHandler.hpp"


int main( const int argc, const char **argv )
{
  try
  {
    //
    // create world to handle physical updates
    // and ioHandler to interface between the
    // world and the user
    //
    shared::World world;
    shared::ImguiOpenGLIOHandler io( world );

    //
    // pass world and ioHandler to driver
    // to manage event loop
    //
    shared::ContinuousDriver driver( world, io );

    //
    // run program and exit
    //
    return driver.exec( argc, argv );
  }
  catch( const std::exception &e )
  {
    std::cerr << "Program failed: " << e.what() << std::endl;
  }

  return EXIT_FAILURE;

} // main
