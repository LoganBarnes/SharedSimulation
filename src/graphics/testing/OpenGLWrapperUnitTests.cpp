// OpenGLWrapperUnitTests.cpp
#include "shared/graphics/OpenGLWrapper.hpp"

#include "gmock/gmock.h"


namespace
{


///
/// \brief The OpenGLWrapperUnitTests class
///
class OpenGLWrapperUnitTests : public ::testing::Test
{

protected:

  /////////////////////////////////////////////////////////////////
  /// \brief OpenGLWrapperUnitTests
  /////////////////////////////////////////////////////////////////
  OpenGLWrapperUnitTests( )
  {}


  /////////////////////////////////////////////////////////////////
  /// \brief ~OpenGLWrapperUnitTests
  /////////////////////////////////////////////////////////////////
  virtual
  ~OpenGLWrapperUnitTests( )
  {}

};


/////////////////////////////////////////////////////////////////
/// \brief UnimplementedTest
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLWrapperUnitTests, UnimplementedTest )
{}



} // namespace
