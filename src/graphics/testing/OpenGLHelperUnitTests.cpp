// OpenGLHelperUnitTests.cpp
#include "shared/graphics/GlfwWrapper.hpp"
#include "shared/graphics/OpenGLHelper.hpp"

#include "gmock/gmock.h"


namespace
{


///
/// \brief The OpenGLHelperUnitTests class
///
class OpenGLHelperUnitTests : public ::testing::Test
{

protected:

  /////////////////////////////////////////////////////////////////
  /// \brief OpenGLHelperUnitTests
  /////////////////////////////////////////////////////////////////
  OpenGLHelperUnitTests( )
    : glfw_( false ) // no print statements
  {
    glfw_.createNewWindow( "", 720, 640 ); // init opengl
  }


  /////////////////////////////////////////////////////////////////
  /// \brief ~OpenGLHelperUnitTests
  /////////////////////////////////////////////////////////////////
  virtual
  ~OpenGLHelperUnitTests( )
  {}


  graphics::GlfwWrapper glfw_;

};


/////////////////////////////////////////////////////////////////
/// \brief TestTextureIsDeleted
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestTextureIsDeleted )
{
  GLuint id;

  //
  // create texture in limited scope
  //
  {
    std::shared_ptr< GLuint > spId =
      graphics::OpenGLHelper::createTextureArray( 10, 10 );

    id = *spId;

    //
    // check texture exists
    //
    ASSERT_TRUE( glIsTexture( id ) );

    auto spId2 = spId;
    auto spId3 = spId;

    auto spId4 = spId2;

    auto gid5( std::move( spId4 ) );

    //
    // gid4 was moved so it should be invalid
    //
    ASSERT_FALSE( spId4 );

    //
    // all other copied variables should still be valid
    //
    ASSERT_TRUE ( spId );
    ASSERT_TRUE ( spId2 );
    ASSERT_TRUE ( spId3 );
    ASSERT_TRUE ( gid5 );

    ASSERT_EQ( id, *spId );
    ASSERT_EQ( id, *spId2 );
    ASSERT_EQ( id, *spId3 );
    ASSERT_EQ( id, *gid5 );
  }

  //
  // GLID went out of scope so texture
  // should be deleted now
  //
  ASSERT_FALSE( glIsTexture( id ) );
}



/////////////////////////////////////////////////////////////////
/// \brief TestVboIsDeleted
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestVboIsDeleted )
{
  GLuint id;

  //
  // create texture in limited scope
  //
  {
    std::vector< float > data      = { 0.0f, 0.0f, 0.0f, 0.0f };
    std::shared_ptr< GLuint > spId =
      graphics::OpenGLHelper::createBuffer(
                                           data.data( ),
                                           data.size( )
                                           );

    id = *spId;

    //
    // check buffer exists
    //
    ASSERT_TRUE( glIsBuffer( id ) );

    auto spId2 = spId;
    auto spId3 = spId;

    auto spId4 = spId2;

    auto gid5( std::move( spId4 ) );

    //
    // gid4 was moved so it should be invalid
    //
    ASSERT_FALSE( spId4 );

    //
    // all other copied variables should still be valid
    //
    ASSERT_TRUE ( spId );
    ASSERT_TRUE ( spId2 );
    ASSERT_TRUE ( spId3 );
    ASSERT_TRUE ( gid5 );

    ASSERT_EQ( id, *spId );
    ASSERT_EQ( id, *spId2 );
    ASSERT_EQ( id, *spId3 );
    ASSERT_EQ( id, *gid5 );
  }

  //
  // GLID went out of scope so buffer
  // should be deleted now
  //
  ASSERT_FALSE( glIsBuffer( id ) );

}



/////////////////////////////////////////////////////////////////
/// \brief TestIboIsDeleted
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestIboIsDeleted )
{
  GLuint id;

  //
  // create texture in limited scope
  //
  {
    std::vector< GLuint > data     = { 0, 1, 2, 1, 2, 3 };
    std::shared_ptr< GLuint > spId =
      graphics::OpenGLHelper::createBuffer(
                                           data.data( ),
                                           data.size( ),
                                           GL_ELEMENT_ARRAY_BUFFER
                                           );

    id = *spId;

    //
    // check buffer exists
    //
    ASSERT_TRUE( glIsBuffer( id ) );

    auto spId2 = spId;
    auto spId3 = spId;

    auto spId4 = spId2;

    auto spId5( std::move( spId4 ) );

    //
    // gid4 was moved so it should be invalid
    //
    ASSERT_FALSE( spId4 );

    //
    // all other copied variables should still be valid
    //
    ASSERT_TRUE ( spId );
    ASSERT_TRUE ( spId2 );
    ASSERT_TRUE ( spId3 );
    ASSERT_TRUE ( spId5 );

    ASSERT_EQ( id, *spId );
    ASSERT_EQ( id, *spId2 );
    ASSERT_EQ( id, *spId3 );
    ASSERT_EQ( id, *spId5 );
  }


  //
  // GLID went out of scope so buffer
  // should be deleted now
  //
  ASSERT_FALSE( glIsBuffer( id ) );

}



/////////////////////////////////////////////////////////////////
/// \brief FramebufferAndRenderbufferDeleted
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, FramebufferAndRenderbufferDeleted )
{
  GLuint fbo, rbo;

  //
  // create texture in limited scope
  //
  {
    auto texId = graphics::OpenGLHelper::createTextureArray( 10, 10 );

    std::shared_ptr< GLuint > spFbo;

    // get renderbuffer in limited scope
    {
      std::shared_ptr< GLuint > spRbo;
      spFbo = graphics::OpenGLHelper::createFramebuffer( 10, 10, texId, &spRbo );

      fbo = *spFbo;
      rbo = *spRbo;

      //
      // check buffers exist
      //
      ASSERT_TRUE( glIsFramebuffer( fbo ) );
      ASSERT_TRUE( glIsRenderbuffer( rbo ) );

    }

    //
    // renderbuffer is out of scope but it should
    // still exist until the framebuffer is deleted
    //
    ASSERT_TRUE( glIsFramebuffer( fbo ) );
    ASSERT_TRUE( glIsRenderbuffer( rbo ) );


    auto spFbo2 = spFbo;
    auto spFbo3 = spFbo;

    auto spFbo4 = spFbo2;

    auto spId5( std::move( spFbo4 ) );

    //
    // gid4 was moved so it should be invalid
    //
    ASSERT_FALSE( spFbo4 );

    //
    // all other copied variables should still be valid
    //
    ASSERT_TRUE ( spFbo );
    ASSERT_TRUE ( spFbo2 );
    ASSERT_TRUE ( spFbo3 );
    ASSERT_TRUE ( spId5 );

    ASSERT_EQ( fbo, *spFbo );
    ASSERT_EQ( fbo, *spFbo2 );
    ASSERT_EQ( fbo, *spFbo3 );
    ASSERT_EQ( fbo, *spId5 );
  }

  //
  // GLID went out of scope so framebuffer and
  // renderbuffer should be deleted now
  //
  ASSERT_FALSE( glIsFramebuffer( fbo ) );
  ASSERT_FALSE( glIsRenderbuffer( rbo ) );

}



} // namespace
