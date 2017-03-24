// ContinuousDriver.hpp
#pragma once

#include "Driver.hpp"
#include <chrono>


namespace shared
{


class World;
class IOHandler;


/////////////////////////////////////////////
/// \brief The ContinuousDriver class
///
/// \author Logan Barnes
/////////////////////////////////////////////
class ContinuousDriver : public Driver
{


public:

  /////////////////////////////////////////////
  /// \brief ContinuousDriver
  /// \param world
  /////////////////////////////////////////////
  ContinuousDriver(
                   World     &world,
                   IOHandler &ioHandler
                   ) noexcept;


  /////////////////////////////////////////////
  /// \brief ~ContinuousDriver
  /////////////////////////////////////////////
  virtual
  ~ContinuousDriver( ) = default;


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
            );


protected:

  double timeScale_;
  bool paused_;


private:

  ///////////////////////////////////////////////////////////////
  /// \brief _runAFAPLoop
  ///
  ///        As Fast As Possible Loop. Alternates world updates
  ///        and render calls as fast as possible.
  ///
  ///////////////////////////////////////////////////////////////
  void _runAFAPLoop ( );


  ///////////////////////////////////////////////////////////////
  /// \brief _runNFTRLoop
  ///
  ///        No Faster Than Real-time Loop. Compares world time
  ///        to CPU clock time and only updates the world to
  ///        time 'X' after the time 'X' has elapsed on the CPU
  ///        clock. No constraints are applied to render calls.
  ///
  ///////////////////////////////////////////////////////////////
  void _runNFTRLoop ( );


  ///////////////////////////////////////////////////////////////
  /// \brief _getTimeSeconds
  ///////////////////////////////////////////////////////////////
  double _getTimeSeconds ( );



  const double startTime_;
  const double timeStep_;
  double worldTime_;
  unsigned long updateFrame_;


  /// \brief initTime_
  const std::chrono::time_point< std::chrono::system_clock > initTime_;


};


} // namespace shared

