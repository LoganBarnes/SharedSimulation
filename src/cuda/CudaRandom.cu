#include <curand.h>
#include <curand_kernel.h>
#include "helper_cuda.h"
#include "helper_grid.h"


extern "C"
{
  /**
   * @brief setup_kernel
   * @param state
   * @param seed
   */
  __global__
  void
  initCuRand_kernel(
                    curandState *state,
                    uint64_t     offset,
                    uint64_t     seed,
                    dim3         texDim
                    )
  {
    uint x = blockIdx.x * blockDim.x + threadIdx.x;
    uint y = blockIdx.y * blockDim.y + threadIdx.y;

    if ( x < texDim.x && y < texDim.y )
    {
      uint id = y * texDim.x + x;
      curand_init( seed, id, 0, &state[ id ] + offset );
    }
  }



  /**
   * @brief cuda_LinitCuRand
   * @param state
   * @param seed
   */
  void
  cuda_initCuRand(
                  curandState *state,
                  uint64_t     offset,
                  uint64_t     seed,
                  dim3         texDim
                  )
  {
    dim3 thread( 32, 32 );
    dim3 block( 1 );

    computeGridSize( texDim.x, thread.x, block.x, thread.x );
    computeGridSize( texDim.y, thread.y, block.y, thread.y );

    initCuRand_kernel << < block, thread >> > ( state, offset, seed, texDim );
  }



}
