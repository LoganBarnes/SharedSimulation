#ifndef CUDA_RANDOM_CUH
#define CUDA_RANDOM_CUH

#include <curand_kernel.h>

extern "C"
{
    void cuda_initCuRand(curandState *state, uint64_t offset, uint64_t seed, dim3 texDim);
}

#endif // CUDA_RANDOM_CUH
