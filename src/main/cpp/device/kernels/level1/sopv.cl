#include "../config.h"

__kernel
void sopv(int op,
          __global const float *restrict x,
          __global float *restrict a) {
    int i = get_global_id(0);
    float value = 0;
    switch (op) {
        case ABS:
            value = fabs(x[i]);
            break;
        case SQRT:
            value = sqrt(x[i]);
            break;
        case EXP:
            value = exp(x[i]);
            break;
        case LN:
            value = log(x[i]);
            break;
        case LOG1P:
            value = log1p(x[i]);
            break;
        case TANH:
            value = tanh(x[i]);
            break;
    }
    a[i] = value;
}