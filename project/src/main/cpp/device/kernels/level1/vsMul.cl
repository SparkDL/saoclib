#include "../config.h"

__kernel
__attribute__((num_compute_units(LEVEL1_NUM_COMPUTE_UNITS)))
void vsMul(__global const float *restrict a,
           __global const float *restrict b,
           __global float *restrict y) {
    int i = get_global_id(0);
    y[i] = a[i] * b[i];
}
