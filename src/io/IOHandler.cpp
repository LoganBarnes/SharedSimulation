#include "IOHandler.hpp"

#include <iostream>
#include <csignal>
#include <random>



namespace srt
{


namespace
{

bool signalCaught( false );

///
/// \brief signalHandler
/// \param signum
///
void
signalHandler( int signum )
{

  std::cout << "Caught signal" << std::endl;

  signalCaught = ( signum == SIGINT );

}



}


/////////////////////////////////////////////
/// \brief Renderer::Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
IOHandler::IOHandler(
                     World &world,
                     bool   printInfo
                     )
  :
  world_          ( world )
  , exitRequested_( false )
{

  signal ( SIGINT, signalHandler );

  if ( printInfo )
  {

    std::cout << "Press 'CTRL + C' or 'ESC' to exit" << std::endl;

  }

}



/////////////////////////////////////////////
/// \brief Renderer::~Renderer
///
/// \author Logan Barnes
/////////////////////////////////////////////
IOHandler::~IOHandler( )
{}



/////////////////////////////////////////////
/// \brief Renderer::render
/// \param alpha
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
IOHandler::showWorld( const double )
{} // IOHandler::showWorld



/////////////////////////////////////////////
/// \brief IOHandler::updateIO
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
IOHandler::updateIO( )
{

  exitRequested_ |= signalCaught;

}



/////////////////////////////////////////////
/// \brief IOHandler::onLoopExit
///
/// \author Logan Barnes
/////////////////////////////////////////////
void
IOHandler::onLoopExit( )
{}



} // namespace srt
