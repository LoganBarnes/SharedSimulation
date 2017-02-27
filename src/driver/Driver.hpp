// Driver.hpp
#pragma once

#include <string>
#include <type_traits>


namespace shared
{


class World;
class IOHandler;


/////////////////////////////////////////////
/// \brief The Driver class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class Driver
{


public:

  /////////////////////////////////////////////
  /// \brief Driver
  /// \param world
  /////////////////////////////////////////////
  Driver(
         World     &world,
         IOHandler &ioHandler
         ) noexcept;


  /////////////////////////////////////////////
  /// \brief ~Driver
  /////////////////////////////////////////////
  virtual
  ~Driver( ) = default;


  /////////////////////////////////////////////
  /// \brief exec
  /// \param argc
  /// \param argv
  /// \return
  /////////////////////////////////////////////
  virtual
  int exec (
            int          argc,
            const char **argv
            ) = 0;



  /////////////////////////////////////////////
  /// \brief printProjectInfo
  /// \param name
  /// \param versionMajor
  /// \param versionMinor
  /// \param versionPatch
  /////////////////////////////////////////////
  static
  void printProjectInfo (
                         const std::string name,
                         const std::string versionMajor,
                         const std::string versionMinor,
                         const std::string versionPatch
                         );


protected:

  World     &world_;
  IOHandler &ioHandler_;


};


} // namespace shared
