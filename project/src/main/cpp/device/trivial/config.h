//
// Created by pcz on 18-7-26.
//

#ifndef SAOCLIB_CONFIG_H
#define SAOCLIB_CONFIG_H

#define BLOCK_SIZE 64

#define SLICE_SIZE 16

#define LEVEL1_NUM_COMPUTE_UNITS 1

#define GER_NUM_COMPUTE_UNITS 1

#define GEMM_NUM_COMPUTE_UNITS 1

#define UNROLL_FACTOR 64

#define II_CYCLES 12


// vector operations
#define ABS 101
#define SQRT 102
#define EXP 103
#define LN 104
#define LOG1P 105
#define TANH 106

// vector,float operations
#define POWX 201
#define SCAL 202

// vector,vector operations
#define ADD 301
#define SUB 302
#define MUL 303
#define DIV 304


#endif //SAOCLIB_CONFIG_H
