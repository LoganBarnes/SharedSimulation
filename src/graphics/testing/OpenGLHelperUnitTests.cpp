// OpenGLHelperUnitTests.cpp
#include <limits>

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


  template< typename IsItemFunc, typename CreateFunc >
  void
  checkDeleted(
               IsItemFunc isItem,
               CreateFunc createItem
               )
  {
    //
    // create initial id and show it doesn't correspond
    // to an OpenGL item yet
    //
    GLuint id = std::numeric_limits< GLuint >::max( );

    ASSERT_FALSE( isItem( id ) );

    //
    // create item in limited scope
    //
    {
      std::shared_ptr< GLuint > spId = createItem( );

      id = *spId;

      //
      // check item exists
      //
      ASSERT_TRUE( isItem( id ) );

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
    // id went out of scope so item
    // should be deleted now
    //
    ASSERT_FALSE( isItem( id ) );
  } // checkDeleted


  template< typename IsItemFunc, typename CreateFunc >
  void
  checkDeletedAndReplaced(
                          IsItemFunc isItem,
                          CreateFunc createItem
                          )
  {
    GLuint id1 = std::numeric_limits< GLuint >::max( );
    GLuint id2 = id2;

    ASSERT_FALSE( isItem( id1 ) );
    ASSERT_FALSE( isItem( id2 ) );

    //
    // create texture in limited scope
    //
    {
      std::shared_ptr< GLuint > spId = createItem( );

      id1 = *spId;

      //
      // check first texture exists
      //
      ASSERT_TRUE( isItem( id1 ) );
      ASSERT_FALSE( isItem( id2 ) );

      //
      // overwrite first id
      //
      spId = createItem( );

      id2 = *spId;

      //
      // check texture ids are different
      // and only the second one exists
      //
      ASSERT_NE( id1, id2 );
      ASSERT_FALSE( isItem( id1 ) );
      ASSERT_TRUE( isItem( id2 ) );
    }

    //
    // bot ids went out of scope so
    // no texture should exist now
    //
    ASSERT_FALSE( isItem( id1 ) );
    ASSERT_FALSE( isItem( id2 ) );
  } // checkDeletedAndReplaced


};


/////////////////////////////////////////////////////////////////
/// \brief TestTextureDeleted
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestTextureDeleted )
{
  checkDeleted( glIsTexture,
               [] ( )
               {
                 return graphics::OpenGLHelper::createTextureArray( 10, 10 );
               } );
}



/////////////////////////////////////////////////////////////////
/// \brief TestTextureDeletedAndReplaced
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestTextureDeletedAndReplaced )
{
  checkDeletedAndReplaced( glIsTexture,
                          [] ( )
                          {
                            return graphics::OpenGLHelper::createTextureArray( 10, 10 );
                          } );
}



/////////////////////////////////////////////////////////////////
/// \brief TestVboDeleted
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestVboDeleted )
{
  checkDeleted( glIsBuffer,
               [] ( )
               {    std::vector< float > data      = { 0.0f, 0.0f, 0.0f, 0.0f };
                    return graphics::OpenGLHelper::createBuffer(
                                                                data.data( ),
                                                                data.size( )
                                                                );
               } );
}



/////////////////////////////////////////////////////////////////
/// \brief TestVboDeletedAndReplaced
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestVboDeletedAndReplaced )
{
  checkDeletedAndReplaced( glIsBuffer,
                          [] ( )
                          {    std::vector< float > data      = { 0.0f, 0.0f, 0.0f, 0.0f };
                               return graphics::OpenGLHelper::createBuffer(
                                                                           data.data( ),
                                                                           data.size( )
                                                                           );
                          } );
}



/////////////////////////////////////////////////////////////////
/// \brief TestIboDeleted
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestIboDeleted )
{
  checkDeleted( glIsBuffer,
               [] ( )
               {    std::vector< float > data      = { 0.0f, 0.0f, 0.0f, 0.0f };
                    return graphics::OpenGLHelper::createBuffer(
                                                                data.data( ),
                                                                data.size( ),
                                                                GL_ELEMENT_ARRAY_BUFFER
                                                                );
               } );
}



/////////////////////////////////////////////////////////////////
/// \brief TestIboDeletedAndReplaced
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, TestIboDeletedAndReplaced )
{
  checkDeletedAndReplaced( glIsBuffer,
                          [] ( )
                          {    std::vector< float > data      = { 0.0f, 0.0f, 0.0f, 0.0f };
                               return graphics::OpenGLHelper::createBuffer(
                                                                           data.data( ),
                                                                           data.size( ),
                                                                           GL_ELEMENT_ARRAY_BUFFER
                                                                           );
                          } );
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
  // id went out of scope so framebuffer and
  // renderbuffer should be deleted now
  //
  ASSERT_FALSE( glIsFramebuffer( fbo ) );
  ASSERT_FALSE( glIsRenderbuffer( rbo ) );
}



/////////////////////////////////////////////////////////////////
/// \brief FramebufferDeletedAndReplaced
/////////////////////////////////////////////////////////////////
TEST_F( OpenGLHelperUnitTests, FramebufferDeletedAndReplaced )
{
  checkDeletedAndReplaced( glIsFramebuffer,
                          [] ( )
                          {    auto texId = graphics::OpenGLHelper::createTextureArray( 10, 10 );
                               return graphics::OpenGLHelper::createFramebuffer( 10, 10, texId );
                          } );
}



} // namespace
