#include "../config.h"

__kernel
__attribute((reqd_work_group_size(BLOCK_SIZE, BLOCK_SIZE, 1)))
__attribute((num_simd_work_items(SIMD_WORK_ITEMS)))
void sgemm(int K,
           float alpha,
           __global volatile float *restrict a,
           __global volatile float *restrict b,
           float beta,
           __global float *restrict c) {
    // Local storage for a block of input matrices A and B
    __local float a_cache[BLOCK_SIZE][BLOCK_SIZE];
    __local float b_cache[BLOCK_SIZE][BLOCK_SIZE];

    // This kernel multiplies 2 matrices of size M*K and K*N, outputs the result of size M*N
    int M = get_global_size(0);
    int N = get_global_size(1);

    int block_row = get_group_id(0);
    int block_col = get_group_id(1);

    // Local ID index (offset within a block)
    int local_row = get_local_id(0);
    int local_col = get_local_id(1);

    int a_start = block_row * BLOCK_SIZE;
    int b_start = K * BLOCK_SIZE * block_col;
    int b_end = b_start + K;

    float running_sum = 0.0f;
    for (int b_pos = b_start, a_pos = a_start;
         b_pos < b_end;
         b_pos += BLOCK_SIZE, a_pos += M * BLOCK_SIZE) {

        a_cache[local_col][local_row] = a[a_pos + N * local_col + local_row];
        b_cache[local_col][local_row] = b[b_pos + K * local_col + local_row];

        // Wait for the entire block to be loaded.
        barrier(CLK_LOCAL_MEM_FENCE);

#pragma unroll
        for (int i = 0; i < BLOCK_SIZE; ++i) {
            running_sum += a_cache[i][local_row] * b_cache[local_col][i];
        }

        // Wait for the block to be fully consumed before loading the next
        // block.
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    int global_row = get_global_id(0);
    int global_col = get_global_id(1);
    int target_pos = M * global_col + global_row;
    float v = c[target_pos];
    c[target_pos] = alpha * running_sum + beta * v;
}