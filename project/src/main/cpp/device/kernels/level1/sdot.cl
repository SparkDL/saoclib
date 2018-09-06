#include "../config.h"

__kernel
void sdot(int n,
          __global volatile float *restrict x,
          int incx,
          __global volatile float *restrict y,
          int incy,
          __global float *restrict result) {
    float sum = 0;
    for (int i = 0, xi = 0, yi = 0; i < n; i++, xi += incx, yi += incy) {
        sum += x[xi] * y[yi];
    }
    *result = sum;
}