#include "../config.h"


__kernel
__attribute__((num_compute_units(LEVEL1_NUM_COMPUTE_UNITS)))
void sscal(float scale,
           __global float *restrict x,
           int incx) {
    int i = incx * get_global_id(0);
    x[i] *= scale;
}