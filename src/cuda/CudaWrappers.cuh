// CudaWrappers.cuh
#pragma once

#include <cuda_runtime.h>

typedef uint32_t GLenum;
typedef uint32_t GLuint;


extern "C"
{

//
// from 'wrappers.cu'
//
void cuda_init (
                const int    argc  = 0,
                const char **argv  = 0,
                const bool   print = true
                );
void cuda_destroy ( const bool print = true );

void cuda_malloc (
                  void **devPtr,
                  size_t size
                  );
void cuda_free ( void *devPtr );

void cuda_memcpy (
                  void               *dst,
                  const void         *src,
                  size_t              count,
                  enum cudaMemcpyKind kind
                  );

void cuda_graphicsGLRegisterImage (
                                   cudaGraphicsResource_t   *resource,
                                   GLuint                    tex,
                                   GLenum                    target,
                                   cudaGraphicsRegisterFlags flags
                                   );
void cuda_graphicsUnregisterResource ( cudaGraphicsResource_t resource );

void cuda_graphicsMapResources ( cudaGraphicsResource_t *res );
void cuda_graphicsUnmapResources ( cudaGraphicsResource_t *res );

void cuda_graphicsSubResourceGetMappedArray (
                                             cudaArray_t           *array,
                                             cudaGraphicsResource_t res,
                                             GLuint                 index,
                                             GLuint                 level
                                             );

void cuda_createSurfaceObject (
                               cudaSurfaceObject_t *surface,
                               cudaResourceDesc    *desc
                               );
void cuda_destroySurfaceObject ( cudaSurfaceObject_t surface );

void cuda_streamSynchronize ( cudaStream_t stream );
void cuda_deviceSynchronize ( );

void cuda_profilerStart ( );
void cuda_profilerStop ( );

} // extern "C"
