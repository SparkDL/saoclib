#include "config.h"

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
                   __global const float *restrict x,
                   int incx,
                   __global const float *restrict y,
                   int incy,
                   __global float *restrict a,
                   int lda) {
    __local float x_local[LOCAL_CACHE_SIZE];
    __local float y_local[LOCAL_CACHE_SIZE];

    int x_loop_total = m / LOCAL_CACHE_SIZE;
    if (m - x_loop_total * LOCAL_CACHE_SIZE > 0) {
        x_loop_total++;
    }

    int y_loop_total = n / LOCAL_CACHE_SIZE;
    if (n - y_loop_total * LOCAL_CACHE_SIZE > 0) {
        y_loop_total++;
    }

    int x_cached = 0;

    /** Calculate the multiplication product for every slice of x */
    for (int x_loop = 0; x_loop < x_loop_total; x_loop++) {
        // Fetch elements of x from global memory
#pragma unroll
        for (int xi = 0, k = 0; k < LOCAL_CACHE_SIZE; xi += incx, k++) {
            x_local[k] = x[xi];
        }

        int y_cached = 0;
        /** Calculate the multiplication product for every slice of y */
        int yi = 0;
        for (int y_loop = 0; y_loop < y_loop_total; y_loop++) {
            // Fetch elements of y from global memory
#pragma unroll
            for (int k = 0; k < LOCAL_CACHE_SIZE; yi += incy, k++) {
                y_local[k] = y[yi];
            }
            // x_local * y_local

#pragma unroll
            for (int k = 0; k < LOCAL_CACHE_SIZE_POW; k++) {
                int x_pos = k / LOCAL_CACHE_SIZE;
                int y_pos = k - x_pos * LOCAL_CACHE_SIZE;
                if (x_pos < m && y_pos < n) {
                    int a_row = x_cached + x_pos;
                    int a_col = y_cached + y_pos;
                    a[a_col * lda + a_row] += alpha * x_local[x_pos] * y_local[y_pos];
                }
            }
            y_cached += LOCAL_CACHE_SIZE;
        }
        x_cached += LOCAL_CACHE_SIZE;
    }
}
