#include "../config.h"

__kernel
__attribute__((num_compute_units(LEVEL1_NUM_COMPUTE_UNITS)))
void vsPowx(__global const float *restrict a,
            float x,
            __global float *restrict y) {
    int i = get_global_id(0);
    y[i] = pow(a[i], x);
}