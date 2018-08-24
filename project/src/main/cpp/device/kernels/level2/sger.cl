#include "../config.h"

/**
Purpose:
 SGER   performs the rank 1 operation

    A := alpha*x*y**T + A,

 where alpha is a scalar, x is an m element vector, y is an n element
 vector and A is an m by n matrix.
 */
__kernel
__attribute((reqd_work_group_size(SLICE_SIZE, SLICE_SIZE, 1)))
__attribute__((num_compute_units(GER_NUM_COMPUTE_UNITS)))
void sger(int m,
          int n,
          float alpha,
          __global const float *restrict x,
          int incx,
          __global const float *restrict y,
          int incy,
          __global float *restrict a,
          int lda) {
    int global_x = get_global_id(0);
    int global_y = get_global_id(1);

    if (global_x < m && global_y < n) {
        float xe = x[incx * global_x];
        float ye = y[incy * global_y];
        int a_row = global_x;
        int a_col = global_y;
        a[a_col * lda + a_row] += alpha * xe * ye;
    }
}