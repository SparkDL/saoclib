#include "../config.h"

__kernel
__attribute__((num_compute_units(LEVEL1_NUM_COMPUTE_UNITS)))
void saxpy(float sa,
           __global const float *restrict sx,
           int incx,
           __global float *restrict sy,
           int incy) {
    int i = get_global_id(0);
    sy[incy * i] += sx[incx * i] * sa;
}