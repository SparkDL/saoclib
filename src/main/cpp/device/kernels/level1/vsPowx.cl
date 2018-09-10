#include "../config.h"

__kernel
void vsPowx(__global const float *restrict a,
            float b,
            __global float *restrict y) {
    int i = get_global_id(0);
    y[i] = pow(a[i], b);
}