#include "shared/core/Driver.hpp"

#include <iostream>
#include <sstream>


namespace shs
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
  : world_     ( world )
  , ioHandler_ ( ioHandler )
{}



/////////////////////////////////////////////
/// \brief Driver::printProjectInfo
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
Driver::printProjectInfo(
                         const std::string name,         ///<
                         const std::string versionMajor, ///<
                         const std::string versionMinor, ///<
                         const std::string versionPatch  ///<
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



/////////////////////////////////////////////
/// \brief Driver::printProjectInfo
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
Driver::printProjectInfo( const std::string name )
{
  std::stringstream info;

  info << "#  Running " << name << "  #";

  std::string borders( info.str( ).length( ), '#' );

  std::cout << borders << '\n' << info.str( ) << '\n' << borders << std::endl;
}



} // namespace shs
