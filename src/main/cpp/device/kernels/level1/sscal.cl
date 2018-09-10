#include "../config.h"

__kernel
void sscal(float scale,
           __global float *restrict x,
           int incx) {
    int i = incx * get_global_id(0);
    x[i] *= scale;
}