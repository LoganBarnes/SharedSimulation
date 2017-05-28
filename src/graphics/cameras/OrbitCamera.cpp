#include "shared/graphics/OrbitCamera.hpp"

#define GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.hpp"



namespace shg
{

///
/// \brief OrbitCamera<T>::OrbitCamera
///
template< typename T >
OrbitCamera< T >::OrbitCamera( )
  : GlmCamera< T >( )
{}


template class OrbitCamera< float >;
template class OrbitCamera< double >;


} // namespace shg
