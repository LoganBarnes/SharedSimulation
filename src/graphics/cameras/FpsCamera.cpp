#include "shared/graphics/FpsCamera.hpp"

#define GLFORCE_RADIANS_
#include "glm/gtc/matrix_transform.hpp"



namespace shg
{

///
/// \brief FpsCamera<T>::FpsCamera
///
template< typename T >
FpsCamera< T >::FpsCamera( )
{
  throw std::runtime_error( "FpsCamera not yet implemented" );
}


template class FpsCamera< float >;
template class FpsCamera< double >;


} // namespace shg
