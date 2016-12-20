#include "graphics/Camera.hpp"

#define GLM_FORCE_RADIANS
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <cmath>



namespace graphics
{


Camera::Camera( )
  : m_orbitX( 0.f )
  , m_orbitY( 0.f )
  , m_zoomZ( 0.f )
{
  glm::vec4 eye  = glm::vec4( 0, 0, 5, 0 );
  glm::vec4 look = -eye;   // normalized in "orientLook()"
  glm::vec4 up   = glm::vec4( 0, 1, 0, 0 );

  orientLook( eye, look, up );

  // Projection Defaults
  m_heightDegrees = 60.f;
  m_aspectRatio   = 1.0f;
  m_near          = 0.1f;
  m_far           = 1000.0f;
  setProjectionMatrix( );

  m_thirdDist = 0.f;
  setFrustumMatrix( );
}



Camera::~Camera( )
{}



const glm::mat4&
Camera::getProjectionMatrix( ) const
{
  return m_proj;
}



const glm::mat4&
Camera::getViewMatrix( ) const
{
  return m_view;
}



const glm::mat4&
Camera::getScaleMatrix( ) const
{
  return m_scale;
}



const glm::mat4&
Camera::getScaleViewInvMatrix( ) const
{
  return m_scaleViewInv;
}



const glm::mat4&
Camera::getFrustumMatrix( ) const
{
  return m_frustum;
}



const glm::vec4&
Camera::getLook( ) const
{
  return m_look;
}



const glm::vec4&
Camera::getUp( ) const
{
  return m_up;
}



const glm::vec4&
Camera::getRight( ) const
{
  return m_right;
}



const glm::vec4&
Camera::getEye( ) const
{
  return m_eye;
}



float
Camera::getAspectRatio( ) const
{
  return m_aspectRatio;
}



void
Camera::setAspectRatio( float a )
{
  m_aspectRatio = a;
  setProjectionMatrix( );
  setFrustumMatrix( );
}



void
Camera::orientLook(
                   glm::vec4 &eye,
                   glm::vec4 &look,
                   glm::vec4 &up
                   )
{
  // Camera vecs
  m_eye  = eye;
  m_look = look;
  m_up   = up;

  setCameraSpace( );
  setViewMatrix( );
  setFrustumMatrix( );
}



void
Camera::moveHorizontal( glm::vec2 dir )
{
  m_eye += glm::normalize( glm::vec4( m_look.x, 0.f, m_look.z, 0.f ) ) * dir.x;
  m_eye += glm::normalize( glm::vec4( -m_look.z, 0.f, m_look.x, 0.f ) ) * dir.y;
  setViewMatrix( );
  setFrustumMatrix( );
}



void
Camera::moveAlongU( float mag )
{
  m_eye += m_u * mag;
  setViewMatrix( );
  setFrustumMatrix( );
}



void
Camera::moveAlongUp( float mag )
{
  m_eye += m_up * mag;
  setViewMatrix( );
  setFrustumMatrix( );
}



void
Camera::moveAlongLook( float mag )
{
  m_eye += m_look * mag;
  setViewMatrix( );
  setFrustumMatrix( );
}



void
Camera::pitch( float degrees )
{
  glm::vec4 oldLook = m_look;

  m_look =
    glm::rotate( m_look, glm::radians( degrees ), glm::cross( glm::vec3( m_up ), glm::vec3(
                                                                                           m_look ) ) );
  setCameraSpace( );

  if ( m_up.y < 0 )
  {
    m_look = oldLook;
    setCameraSpace( );
  }

  setViewMatrix( );
  setFrustumMatrix( );
}



void
Camera::yaw( float degrees )
{
  float radians = glm::radians( degrees );

  glm::vec3 vec = glm::vec3( 0.f, -1.f, 0.f );

  m_look = glm::rotate( m_look, radians, vec );
  m_up   = glm::rotate( m_up, radians, vec );
  setCameraSpace( );
  setViewMatrix( );
  setFrustumMatrix( );
}



void
Camera::roll( float degrees )
{
  m_up = glm::rotate( m_up, glm::radians( degrees ), glm::vec3( m_look ) );
  setCameraSpace( );
  setViewMatrix( );
  setFrustumMatrix( );
}



void
Camera::updateOrbit(
                    float zoomZ,
                    float deltaX,
                    float deltaY
                    )
{
  // x = dy and y = dx because orbits are angles around that axis
  // and deltas are translations in that direction
  m_zoomZ  += zoomZ;
  m_orbitX += deltaY;
  m_orbitY += deltaX;

  if ( m_zoomZ < 0.001f )
  {
    m_zoomZ = 0.001f;
  }

  glm::mat4 trans =
    glm::rotate( glm::radians( m_orbitY ), glm::vec3( 0.f, 1.f, 0.f ) )
    * glm::rotate( glm::radians( m_orbitX ), glm::vec3( 1.f, 0.f, 0.f ) )
    * glm::translate( glm::vec3( 0.f, 0.f, m_zoomZ ) );


  glm::vec4 eye  = trans * glm::vec4( 0, 0, 0, 1 );
  glm::vec4 look = -eye;   // normalized in "orientLook()"
  glm::vec4 up   = trans * glm::vec4( 0, 1, 0, 0 );
  this->orientLook( eye, look, up );

} // updateOrbit



///
/// \brief Camera::buildRayBasisVectors
/// \param pU
/// \param pV
/// \param pW
///
void
Camera::buildRayBasisVectors(
                             glm::vec3 *pU,
                             glm::vec3 *pV,
                             glm::vec3 *pW
                             ) const
{

  glm::vec3 &U = *pU;
  glm::vec3 &V = *pV;
  glm::vec3 &W = *pW;

  float ulen, vlen, wlen;
  W = -glm::vec3( m_eye );

  wlen = glm::length( W );
  U = glm::normalize( glm::cross( W, glm::vec3( m_up ) ) );
  V = glm::normalize( glm::cross( U, V ) );

  ulen = wlen * glm::tan( glm::radians( 0.5f * m_heightDegrees ) );
  U *= ulen;
  vlen = ulen / m_aspectRatio;
  V *= vlen;

}



void
Camera::setCameraSpace( )
{
  // Camera coordinate space
  m_look = glm::normalize( m_look );
  m_w    = -m_look;
  m_v    = glm::normalize( m_up - ( glm::dot( m_up, m_w ) * m_w ) );
  m_u    = glm::vec4( glm::normalize( glm::cross(
                                                 glm::vec3( m_v ),
                                                 glm::vec3( m_w )
                                                 ) ), 0 );
  m_up    = m_v;
  m_right = m_u;

}



void
Camera::setViewMatrix( )
{
  // View Matrices
  glm::mat4 trans = glm::mat4( );

  trans[ 3 ][ 0 ] = -m_eye[ 0 ];
  trans[ 3 ][ 1 ] = -m_eye[ 1 ];
  trans[ 3 ][ 2 ] = -m_eye[ 2 ];

  // row-major order
  glm::mat4 rot = glm::mat4(
                            m_u.x, m_u.y, m_u.z, 0.0,
                            m_v.x, m_v.y, m_v.z, 0.0,
                            m_w.x, m_w.y, m_w.z, 0.0,
                            0.0,   0.0,   0.0,  1.0
                            );

  // column-major order for glm and glsl
  rot = glm::transpose( rot );

  m_view = rot * trans;

  m_scaleViewInv = glm::inverse( m_scale * m_view );
} // setViewMatrix



void
Camera::setProjectionMatrix( )
{
  // Projection Matrices
  float h = m_far * glm::tan( glm::radians( m_heightDegrees / 2.0 ) );
  float w = m_aspectRatio * h;

  // row-major order
  m_scale = glm::mat4(
                      1.0 / w,       0.0,          0.0,        0.0,
                      0.0,        1.0 / h,         0.0,        0.0,
                      0.0,           0.0,       1.0 / m_far,   0.0,
                      0.0,           0.0,          0.0,        1.0
                      );

  // column-major order for glm and glsl
  m_scale = glm::transpose( m_scale );

  float c = -m_near / m_far;

  // row-major order
  glm::mat4 perspective = glm::mat4(
                                    1.0,   0.0,      0.0,               0.0,
                                    0.0,   1.0,      0.0,               0.0,
                                    0.0,   0.0, -1.0 / ( 1.0 + c ),  c / ( 1.0 + c ),
                                    0.0,   0.0,     -1.0,               0.0
                                    );

  // column-major order for glm and glsl
  perspective = glm::transpose( perspective );

  m_proj = perspective * m_scale;

  m_scaleViewInv = glm::inverse( m_scale * m_view );
} // setProjectionMatrix



void
Camera::setFrustumMatrix( )
{
  m_frustum = glm::transpose( m_proj * m_view );
}



} // namespace graphics
