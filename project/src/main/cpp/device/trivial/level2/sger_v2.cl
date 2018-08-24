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
                   __global const float *restrict x,
                   int incx,
                   __global const float *restrict y,
                   int incy,
                   __global float *restrict a,
                   int lda) {
    __local float x_local[LOCAL_CACHE_SIZE];
    __local float y_local[LOCAL_CACHE_SIZE];

    int x_slice_total = m / LOCAL_CACHE_SIZE;
    if (m - x_slice_total * LOCAL_CACHE_SIZE > 0) {
        x_slice_total++;
    }

    int y_slice_total = n / LOCAL_CACHE_SIZE;
    if (n - y_slice_total * LOCAL_CACHE_SIZE > 0) {
        y_slice_total++;
    }

    int loop_total = x_slice_total * y_slice_total;

    /** Calculate the multiplication product for every slice of x and y */
    for (int loop = 0; loop < loop_total; loop++) {
        // Determine the slice num
        int x_slice = loop / y_slice_total;
        int y_slice = loop - x_slice * y_slice_total;

        int x_cached = x_slice * LOCAL_CACHE_SIZE;
        int y_cached = y_slice * LOCAL_CACHE_SIZE;
        int x_remain = m - x_cached;
        int y_remain = n - y_cached;
        int x_pos = x_cached * incx;
        int y_pos = y_cached * incy;

        if (y_slice == 0) {
            // Refresh local cache of x
#pragma unroll
            for (int xi = x_pos, k = 0; k < LOCAL_CACHE_SIZE; xi += incx, k++) {
                if (k < x_remain) {
                    x_local[k] = x[xi];
                }
            }
        }

        //  Refresh local cache of y
#pragma unroll
        for (int yi = y_pos, k = 0; k < LOCAL_CACHE_SIZE; yi += incy, k++) {
            if (k < y_remain) {
                y_local[k] = y[yi];
            }
        }

        // x_local * y_local
#pragma unroll
#pragma ivdep
        for (int k = 0; k < LOCAL_CACHE_SIZE_POW; k++) {
            int x_local_pos = k / LOCAL_CACHE_SIZE;
            int y_local_pos = k - x_local_pos * LOCAL_CACHE_SIZE;
            if (x_local_pos < x_remain && y_local_pos < y_remain) {
                int a_row = x_cached + x_local_pos;
                int a_col = y_cached + y_local_pos;
                int target_pos = a_col * lda + a_row;
                a[target_pos] += alpha * x_local[x_local_pos] * y_local[y_local_pos];
            }
        }
    }
}
