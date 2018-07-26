#include "../config.h"

__kernel void vsPowx(int n,
                     __global const float *restrict a,
                     float x,
                     __global float *restrict y) {
#pragma unroll VECTOR_UNROLL_SIZE
    for (int i = 0; i < n; i++) {
        y[i] = pow(a[i], x);
    }
}