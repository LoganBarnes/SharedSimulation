// DriverUnitTests.cpp

#include "shared/core/Driver.hpp"

#include "gmock/gmock.h"


namespace
{


///
/// \brief The DriverUnitTests class
///
class DriverUnitTests : public ::testing::Test
{

protected:

  /////////////////////////////////////////////////////////////////
  /// \brief DriverUnitTests
  /////////////////////////////////////////////////////////////////
  DriverUnitTests( )
  {}


  /////////////////////////////////////////////////////////////////
  /// \brief ~DriverUnitTests
  /////////////////////////////////////////////////////////////////
  virtual
  ~DriverUnitTests( )
  {}

};


/////////////////////////////////////////////////////////////////
/// \brief UnimplementedTest
/////////////////////////////////////////////////////////////////
TEST_F( DriverUnitTests, UnimplementedTest )
{}



} // namespace