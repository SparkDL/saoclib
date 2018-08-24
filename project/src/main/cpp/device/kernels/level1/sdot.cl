#include "../config.h"

__kernel
__attribute__((num_compute_units(LEVEL1_NUM_COMPUTE_UNITS)))
void sdot(int n,
          __global const float *restrict sx,
          int incx,
          __global const float *restrict sy,
          int incy,
          __global float *restrict result) {
    // Create shift register to pipeline operations
    float shift_reg[II_CYCLES + 1];
#pragma unroll
    for (int i = 0; i < II_CYCLES + 1; i++) {
        shift_reg[i] = 0;
    }

    for (int xi = 0, yi = 0, k = 0; k < n; xi += incx, yi += incy, k++) {
        // Load ith element into end of shift register
        // if n > II_CYCLE, add to shift_reg[0] to preserve values
        shift_reg[II_CYCLES] = shift_reg[0] + (sx[xi] * sy[yi]);
        //Shift every element of shift register
#pragma unroll
        for (int j = 0; j < II_CYCLES; ++j) {
            shift_reg[j] = shift_reg[j + 1];
        }
    }

    //Sum every element of shift register
    float temp_sum = 0;
#pragma unroll
    for (int i = 0; i < II_CYCLES; ++i) {
        temp_sum += shift_reg[i];
    }
    *result = temp_sum;
}