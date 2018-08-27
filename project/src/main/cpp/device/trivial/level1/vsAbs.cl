#include "config.h"

__kernel void vsAbs(int n,
                    __global const float *restrict a,
                    __global float *restrict y) {
#pragma unroll VECTOR_UNROLL_SIZE
    for (int i = 0; i < n; i++) {
        y[i] = fabs(a[i]);
    }
}