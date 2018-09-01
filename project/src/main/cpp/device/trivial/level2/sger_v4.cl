#include "../config.h"

/**
Purpose:
 SGER   performs the rank 1 operation

    A := alpha*x*y**T + A,

 where alpha is a scalar, x is an m element vector, y is an n element
 vector and A is an m by n matrix.
 */
__kernel void sger(int m,
                   int n,
                   float alpha,
                   __global const float16 *restrict x,
                   __global const float16 *restrict y,
                   __global float16 *restrict a,
                   int lda) {
    float16 x_cache[16];
    float16 y_cache[16];

    for (int i = 0; i < 16; i++) {
        x_cache[i] = y[i];
        y_cache[i] = y[i];
    }

    for (int row = 0; row < 256; row++) {
        for (int col = 0; col < 256; col++)
            a[lda * col + row] += x_cache[row] * y_cache[col];
    }
}