#include "config.h"

__kernel void sscal(int n,
                    float scale,
                    __global float *restrict x,
                    int incx) {
#pragma unroll VECTOR_UNROLL_SIZE
    for (int xi = 0, k = 0; k < n; xi += incx, k++) {
        x[xi] *= scale;
    }
}