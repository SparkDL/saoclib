#include "../config.h"

__kernel
__attribute__((num_compute_units(LEVEL1_NUM_COMPUTE_UNITS)))
void vsAbs(__global const float *restrict a,
           __global float *restrict y) {
    int i = get_global_id(0);
    y[i] = fabs(a[i]);
}