#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <windows.h>
#endif
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <cuda_profiler_api.h>
#include "helper_cuda.h"

typedef unsigned int uint;
typedef unsigned long ulong;

extern "C"
{
  void
  cuda_init(
            int          argc,
            const char **argv,
            bool         print
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
  }



  void
  cuda_destroy( )
  {
    // cudaDeviceReset causes the driver to clean up all state. While
    // not mandatory in normal operation, it is good practice.  It is also
    // needed to ensure correct operation when the application is being
    // profiled. Calling cudaDeviceReset causes all profile data to be
    // flushed before the application exits
    cudaDeviceReset( );
  }



  void
  cuda_allocateArray(
                     void **devPtr,
                     size_t size
                     )
  {
    checkCudaErrors( cudaMalloc( devPtr, size ) );
  }



  void
  cuda_freeArray( void *devPtr )
  {
    checkCudaErrors( cudaFree( devPtr ) );
  }



  void
  cuda_copyArrayToDevice(
                         void       *device,
                         const void *host,
                         size_t      offset,
                         size_t      size
                         )
  {
    checkCudaErrors( cudaMemcpy( ( char* ) device + offset, host, size, cudaMemcpyHostToDevice ) );
  }



  void
  cuda_copyArrayFromDevice(
                           void       *host,
                           const void *device,
                           size_t      size
                           )
  {
    checkCudaErrors( cudaMemcpy( host, device, size, cudaMemcpyDeviceToHost ) );
  }



  void
  cuda_registerGLTexture(
                         cudaGraphicsResource_t   *resource,
                         GLuint                    tex,
                         GLenum                    target,
                         cudaGraphicsRegisterFlags flags
                         )
  {
    checkCudaErrors( cudaGraphicsGLRegisterImage( resource, tex, target, flags ) );
  }



  void
  cuda_unregisterResource( cudaGraphicsResource_t resource )
  {
    checkCudaErrors( cudaGraphicsUnregisterResource( resource ) );
  }



  void
  cuda_graphicsMapResource( cudaGraphicsResource_t *res )
  {
    checkCudaErrors( cudaGraphicsMapResources( 1, res ) );
  }



  void
  cuda_graphicsUnmapResource( cudaGraphicsResource_t *res )
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



}
