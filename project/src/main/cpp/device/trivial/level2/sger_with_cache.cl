#include "../config.h"

/**
Purpose:
 SGER   performs the rank 1 operation

    A := alpha*x*y**T + A,

 where alpha is a scalar, x is an m element vector, y is an n element
 vector and A is an m by n matrix.
 */
__kernel
__attribute__((num_compute_units(GER_NUM_COMPUTE_UNITS)))
__attribute((reqd_work_group_size(SLICE_SIZE, SLICE_SIZE, 1)))
void sger_v1(int m,
             int n,
             float alpha,
             __global volatile const float *restrict x,
             int incx,
             __global volatile const float *restrict y,
             int incy,
             __global float *restrict a,
             int lda) {

    int global_x = get_global_id(0);
    int global_y = get_global_id(1);

    __local float x_cache[SLICE_SIZE];
    __local float y_cache[SLICE_SIZE];

    int slice_x = get_group_id(0);
    int slice_y = get_group_id(1);

    int local_x = get_local_id(0);
    int local_y = get_local_id(1);

    bool is_valid_range = global_x < m && global_y < n;

    if (is_valid_range && local_y == 0) {
        x_cache[local_x] = x[incx * global_x];
    }
    if (is_valid_range && local_x == 0) {
        y_cache[local_y] = y[incy * global_y];
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    if (is_valid_range) {
        int a_row = global_x;
        int a_col = global_y;
        a[a_col * lda + a_row] += alpha * x_cache[local_x] * y_cache[local_y];
    }
}