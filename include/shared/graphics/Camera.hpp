// Camera.hpp
#pragma once


#include "glm/glm.hpp"


namespace graphics
{


template< typename T >
class Camera
{
public:

  Camera( );

  virtual
  ~Camera( ) = default;

  const glm::tmat4x4< T > &getProjectionMatrix ( ) const;
  const glm::tmat4x4< T > &getViewMatrix ( ) const;
  const glm::tmat4x4< T > &getScaleMatrix ( ) const;
  const glm::tmat4x4< T > &getScaleViewInvMatrix ( ) const;
  const glm::tmat4x4< T > &getFrustumMatrix ( ) const;
  const glm::tvec4< T > &getLook ( ) const;
  const glm::tvec4< T > &getUp ( ) const;
  const glm::tvec4< T > &getRight ( ) const;
  const glm::tvec4< T > &getEye ( ) const;

  T getAspectRatio ( ) const;
  void setAspectRatio ( T a );


  void orientLook (
                   glm::tvec4< T > &eye,
                   glm::tvec4< T > &look,
                   glm::tvec4< T > &up
                   );

  void moveHorizontal ( glm::tvec2< T > dir );
  void moveAlongU ( T mag );
  void moveAlongUp ( T mag );
  void moveAlongLook ( T mag );

  void pitch ( T degrees );
  void yaw ( T degrees );
  void roll ( T degrees );

  void updateOrbit (
                    T zoomZ,
                    T deltaX,
                    T deltaY
                    );

  void buildRayBasisVectors (
                             glm::tvec3< T > *pU,
                             glm::tvec3< T > *pV,
                             glm::tvec3< T > *pW
                             ) const;


protected:

  void setCameraSpace ( );
  void setViewMatrix ( );
  void setProjectionMatrix ( );
  void setFrustumMatrix ( );

  glm::tvec4< T > u_, v_, w_;
  glm::tvec4< T > eye_, look_, up_, right_;

  glm::tmat4x4< T > view_, proj_, frustum_;
  glm::tmat4x4< T > scale_, scaleViewInv_;

  // View variables
  T near_, far_, heightDegrees_, aspectRatio_;

  T orbitX_, orbitY_, zoomZ_;

  T thirdDist_;

};


} // namespace graphics
