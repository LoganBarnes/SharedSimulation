#ifndef CUDA_FUNCTIONS_CUH
#define CUDA_FUNCTIONS_CUH

#include <cuda_runtime.h>

typedef uint32_t GLenum;
typedef uint32_t GLuint;
typedef unsigned int uint;

extern "C"
{
    /*
     * from 'wrappers.cu'
     */
    void cuda_init(int argc, const char **argv, bool print = true);
    void cuda_destroy();

    void cuda_allocateArray(void **devPtr, size_t size);
    void cuda_freeArray(void *devPtr);

    void cuda_copyArrayToDevice(void *device, const void *host, size_t offset, size_t size);
    void cuda_copyArrayFromDevice(void *host, const void *device, size_t size);

    void cuda_registerGLTexture(cudaGraphicsResource_t* resource, GLuint tex, GLenum target, cudaGraphicsRegisterFlags flags);
    void cuda_unregisterResource(cudaGraphicsResource_t resource);

    void cuda_graphicsMapResource(cudaGraphicsResource_t *res);
    void cuda_graphicsUnmapResource(cudaGraphicsResource_t *res);

    void cuda_graphicsSubResourceGetMappedArray(cudaArray_t *array, cudaGraphicsResource_t res, GLuint index, GLuint level);

    void cuda_createSurfaceObject(cudaSurfaceObject_t *surface, cudaResourceDesc *desc);
    void cuda_destroySurfaceObject(cudaSurfaceObject_t surface);

    void cuda_streamSynchronize(cudaStream_t stream);
    void cuda_deviceSynchronize();

    void cuda_profilerStart();
    void cuda_profilerStop();
}

#endif // CUDA_FUNCTIONS_CUH
