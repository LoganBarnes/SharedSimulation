#ifndef HELPER_GRID_H
#define HELPER_GRID_H

#include <helper_cuda.h>


//Round a / b to nearest higher integer value
inline
uint
iDivUp(
       uint a,
       uint b
       )
{

  return ( a % b != 0 ) ? ( a / b + 1 ) : ( a / b );

}



// compute grid and thread block size for a given number of elements
inline
void
computeGridSize(
                uint  n,
                uint  blockSize,
                uint &numBlocks,
                uint &numThreads
                )
{

  numThreads = min( blockSize, n );
  numBlocks  = iDivUp( n, numThreads );

}



#endif // HELPER_GRID_H
