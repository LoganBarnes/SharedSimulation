#include "Driver.hpp"

#include <iostream>
#include <sstream>


namespace shared
{



/////////////////////////////////////////////
/// \brief Driver::Driver
/// \param world
///
/// \author Logan Barnes
/////////////////////////////////////////////
Driver::Driver(
               World     &world,
               IOHandler &ioHandler
               ) noexcept
  :
  world_     ( world )
  , ioHandler_ ( ioHandler )
{}



/////////////////////////////////////////////
/// \brief Driver::printProjectInfo
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
Driver::printProjectInfo(
                         std::string name,         ///<
                         std::string versionMajor, ///<
                         std::string versionMinor, ///<
                         std::string versionPatch  ///<
                         )
{

  std::stringstream info;
  info << "#  Running "
       << name << " v"
       << versionMajor << "."
       << versionMinor << "."
       << versionPatch << "  #";

  std::string borders( info.str( ).length( ), '#' );

  std::cout << borders << '\n' << info.str( ) << '\n' << borders << std::endl;

}



} // namespace shared
