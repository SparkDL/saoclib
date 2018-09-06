#include "../config.h"

__kernel
void saxpy(float a,
           __global volatile float *restrict x,
           int incx,
           __global float *restrict y,
           int incy) {
    int i = get_global_id(0);
    y[incy * i] += x[incx * i] * a;
}