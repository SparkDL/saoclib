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


    int x_loop_total = m / LOCAL_CACHE_SIZE;
    int y_loop_total = n / LOCAL_CACHE_SIZE;
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
            for (int xk = 0; xk < LOCAL_CACHE_SIZE; xk++) {
#pragma unroll
                for (int yk = 0; yk < LOCAL_CACHE_SIZE; yk++) {
                    int a_row = x_cached + xk;
                    int a_col = y_cached + yk;
                    a[a_col * lda + a_row] += alpha * x_local[xk] * y_local[yk];
                }
            }
            y_cached += LOCAL_CACHE_SIZE;
        }

        /** Deal with remain elements of y */
        int y_remain = n - y_loop_total * LOCAL_CACHE_SIZE;
#pragma unroll VECTOR_UNROLL_SIZE
        for (int k = 0; k < y_remain; yi += incy, k++) {
            y_local[k] = y[yi];
        }
        // x_local * y_local
#pragma unroll
        for (int xk = 0; xk < LOCAL_CACHE_SIZE; xk++) {
#pragma unroll VECTOR_UNROLL_SIZE
            for (int yk = 0; yk < y_remain; yk++) {
                int a_row = x_cached + xk;
                int a_col = y_cached + yk;
                a[a_col * lda + a_row] += alpha * x_local[xk] * y_local[yk];
            }
        }

        x_cached += LOCAL_CACHE_SIZE;
    }


    /** Deal with remain elements of x*/
    {
        int x_remain = m - x_loop_total * LOCAL_CACHE_SIZE;
#pragma unroll VECTOR_UNROLL_SIZE
        for (int k = 0; k < x_remain; xi += incx, k++) {
            x_local[k] = x[xi];
        }

        int yi = 0;
        int y_cached = 0;
        for (int y_loop = 0; y_loop < y_loop_total; y_loop++) {
            // Fetch elements of y from global memory
#pragma unroll
            for (int k = 0; k < LOCAL_CACHE_SIZE; yi += incy, k++) {
                y_local[k] = y[y_loop];
            }
            // x_local * y_local
#pragma unroll VECTOR_UNROLL_SIZE
            for (int xk = 0; xk < x_remain; xk++) {
#pragma unroll
                for (int yk = 0; yk < LOCAL_CACHE_SIZE; yk++) {
                    int a_row = x_cached + xk;
                    int a_col = y_cached + yk;
                    a[a_col * lda + a_row] += alpha * x_local[xk] * y_local[yk];
                }
            }
            y_cached += LOCAL_CACHE_SIZE;
        }

        /** Deal with remain elements of y */
        int y_remain = n - y_loop_total * LOCAL_CACHE_SIZE;
#pragma unroll VECTOR_UNROLL_SIZE
        for (int k = 0; k < y_remain; yi += incy, k++) {
            y_local[k] = y[yi];
        }
        // x_local * y_local
#pragma unroll VECTOR_UNROLL_SIZE
        for (int xk = 0; xk < x_remain; xk++) {
#pragma unroll VECTOR_UNROLL_SIZE
            for (int yk = 0; yk < y_remain; yk++) {
                int a_row = x_cached + xk;
                int a_col = y_cached + yk;
                a[a_col * lda + a_row] += alpha * x_local[xk] * y_local[yk];
            }
        }
    }
}