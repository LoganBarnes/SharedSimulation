#ifndef Camera_hpp
#define Camera_hpp


#include "glm/glm.hpp"


class Camera
{
public:

  Camera( );

  ~Camera( );

  const glm::mat4 &getProjectionMatrix ( );
  const glm::mat4 &getViewMatrix ( );
  const glm::mat4 &getScaleMatrix ( );
  const glm::mat4 &getScaleViewInvMatrix ( );
  const glm::mat4 &getFrustumMatrix ( );
  const glm::vec4 &getLook ( );
  const glm::vec4 &getUp ( );
  const glm::vec4 &getRight ( );
  const glm::vec4 &getEye ( );

  float getAspectRatio ( );
  void setAspectRatio ( float a );


  void orientLook (
                   glm::vec4 &eye,
                   glm::vec4 &look,
                   glm::vec4 &up
                   );

  void moveHorizontal ( glm::vec2 dir );
  void moveAlongU ( float mag );
  void moveAlongUp ( float mag );
  void moveAlongLook ( float mag );

  void pitch ( float degrees );
  void yaw ( float degrees );
  void roll ( float degrees );

  void updateOrbit (
                    float zoomZ,
                    float deltaX,
                    float deltaY
                    );


protected:

  void setCameraSpace ( );
  void setViewMatrix ( );
  void setProjectionMatrix ( );
  void setFrustumMatrix ( );

  glm::vec4 m_u, m_v, m_w;
  glm::vec4 m_eye, m_look, m_up, m_right;

  glm::mat4 m_view, m_proj, m_frustum;
  glm::mat4 m_scale, m_scaleViewInv;

  // View variables
  float m_near, m_far, m_heightDegrees, m_aspectRatio;

  float m_orbitX, m_orbitY, m_zoomZ;

  float m_thirdDist;

};

#endif // Camera_hpp
