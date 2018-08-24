#include "../config.h"

__kernel void saxpy(int n,
                    float sa,
                    __global const float *restrict sx,
                    int incx,
                    __global float *restrict sy,
                    int incy) {
#pragma unroll VECTOR_UNROLL_SIZE
    for (int xi = 0, yi = 0, k = 0; k < n; xi += incx, yi += incy, k++) {
        sy[yi] += sx[xi] * sa;
    }
}