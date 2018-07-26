#include "../config.h"

__kernel void vsdot(int n,
                    __global const float *restrict x,
                    __global const float *restrict y,
                    __global float *restrict result) {
    // create shift registers to pipeline operations
    float shift_reg[II_CYCLES + 1];
#pragma unroll
    for (int i = 0; i < II_CYCLES + 1; i++) {
        shift_reg[i] = 0;
    }

#pragma unroll VECTOR_UNROLL_SIZE
    for (int i = 0; i < n; i++) {
        // Load ith element into end of shift register
        // if n > II_CYCLE, add to shift_reg[0] to preserve values
        shift_reg[II_CYCLES] = shift_reg[0] + (x[i] * y[i]);
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