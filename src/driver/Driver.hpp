#ifndef Driver_hpp
#define Driver_hpp

#include <string>
#include <type_traits>
#include <chrono>


namespace srt
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
  ~Driver( );


  /////////////////////////////////////////////
  /// \brief exec
  /// \param argc
  /// \param argv
  /// \return
  /////////////////////////////////////////////
  int exec (
            int          argc,
            const char **argv
            );



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

  double timeScale_;
  bool paused_;

  World     &world_;
  IOHandler &ioHandler_;


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


} // namespace srt


#endif // Driver_hpp
