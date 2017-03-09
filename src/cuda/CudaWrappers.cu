#if defined( WIN32 ) || defined( _WIN32 ) || defined( WIN64 ) || defined( _WIN64 )
#include <windows.h>
#endif
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <cuda_profiler_api.h>
#include "helper_cuda.h"
#include <iostream>

extern "C"
{

void
cuda_init(
          const int    argc,
          const char **argv,
          const bool   print
          )
{
  int devID;

  // use device with highest Gflops/s
  devID = findCudaDevice( argc, argv, print );

  if ( devID < 0 )
  {
    printf( "No CUDA Capable devices found, exiting...\n" );
    exit( EXIT_SUCCESS );
  }
  std::cout << std::flush;
}



void
cuda_destroy( const bool print )
{
  // cudaDeviceReset causes the driver to clean up all state. While
  // not mandatory in normal operation, it is good practice.  It is also
  // needed to ensure correct operation when the application is being
  // profiled. Calling cudaDeviceReset causes all profile data to be
  // flushed before the application exits
  cudaDeviceReset( );

  if ( print )
  {
    std::cout << "Cuda device reset" << std::endl;
  }
}



void
cuda_malloc(
            void **devPtr,
            size_t size
            )
{
  checkCudaErrors( cudaMalloc( devPtr, size ) );
}



void
cuda_free( void *devPtr )
{
  checkCudaErrors( cudaFree( devPtr ) );
}



void
cuda_memcpy(
            void               *dst,
            const void         *src,
            size_t              count,
            enum cudaMemcpyKind kind
            )
{
  checkCudaErrors( cudaMemcpy( dst, src, count, kind ) );
}



void
cuda_graphicsGLRegisterImage(
                             cudaGraphicsResource_t   *resource,
                             GLuint                    tex,
                             GLenum                    target,
                             cudaGraphicsRegisterFlags flags
                             )
{
  checkCudaErrors( cudaGraphicsGLRegisterImage( resource, tex, target, flags ) );
}



void
cuda_graphicsUnregisterResource( cudaGraphicsResource_t resource )
{
  checkCudaErrors( cudaGraphicsUnregisterResource( resource ) );
}



void
cuda_graphicsMapResources( cudaGraphicsResource_t *res )
{
  checkCudaErrors( cudaGraphicsMapResources( 1, res ) );
}



void
cuda_graphicsUnmapResources( cudaGraphicsResource_t *res )
{
  checkCudaErrors( cudaGraphicsUnmapResources( 1, res ) );
}



void
cuda_graphicsSubResourceGetMappedArray(
                                       cudaArray_t           *array,
                                       cudaGraphicsResource_t res,
                                       GLuint                 index,
                                       GLuint                 level
                                       )
{
  checkCudaErrors( cudaGraphicsSubResourceGetMappedArray( array, res, index, level ) );
}



void
cuda_createSurfaceObject(
                         cudaSurfaceObject_t *surface,
                         cudaResourceDesc    *desc
                         )
{
  checkCudaErrors( cudaCreateSurfaceObject( surface, desc ) );
}



void
cuda_destroySurfaceObject( cudaSurfaceObject_t surface )
{
  checkCudaErrors( cudaDestroySurfaceObject( surface ) );
}



void
cuda_streamSynchronize( cudaStream_t stream )
{
  checkCudaErrors( cudaStreamSynchronize( stream ) );
}



void
cuda_deviceSynchronize( )
{
  checkCudaErrors( cudaDeviceSynchronize( ) );
}



void
cuda_profilerStart( )
{
  checkCudaErrors( cudaProfilerStart( ) );
}



void
cuda_profilerStop( )
{
  checkCudaErrors( cudaProfilerStop( ) );
}



} // extern "C"
