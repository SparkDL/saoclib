#include "../config.h"

__kernel
void sopvv(int op,
           __global const float *restrict x,
           __global const float *restrict y,
           __global float *restrict a) {
    int i = get_global_id(0);
    float value = 0;
    switch (op) {
        case ADD:
            value = x[i] + y[i];
            break;
        case SUB:
            value = x[i] - y[i];
            break;
        case MUL:
            value = x[i] * y[i];
            break;
        case DIV:
            value = x[i] / y[i];
            break;
    }
    a[i] = value;
}