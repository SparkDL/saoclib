#include "../config.h"

__kernel
__attribute((reqd_work_group_size(BLOCK_SIZE, BLOCK_SIZE, 1)))
__attribute((num_simd_work_items(SIMD_WORK_ITEMS)))
void sgemm(int transa, int transb,
           int M, int N, int K, float alpha,
           __global volatile float *restrict a, int lda,
           __global volatile float *restrict b, int ldb,
           float beta,
           __global float *restrict c, int ldc) {
    // Local storage for a block of input matrices A and B
    __local float a_cache[BLOCK_SIZE][BLOCK_SIZE];
    __local float b_cache[BLOCK_SIZE][BLOCK_SIZE];

    // Global ID index, thread (x,y) calculate the result of c[y,x]
    int global_col = get_global_id(0);
    int global_row = get_global_id(1);
    // Local ID index (offset within a block)
    int local_col = get_local_id(0);
    int local_row = get_local_id(1);

    bool is_transa = transa == 1;
    bool is_transb = transb == 1;

    int n_tiles = K / BLOCK_SIZE;
    if (K - n_tiles * BLOCK_SIZE > 0) {
        n_tiles++;
    }

    float running_sum = 0.0f;

    for (int i = 0, global_a_col = local_col, global_b_row = local_row;
         i < n_tiles;
         i++, global_a_col += BLOCK_SIZE, global_b_row += BLOCK_SIZE) {

        int global_a_row = global_row;
        int global_b_col = global_col;

        if (global_a_row < M && global_a_col < K) {
            if (is_transa) {
                a_cache[local_row][local_col] = a[tpos(global_a_row, global_a_col, lda)];
            } else {
                a_cache[local_row][local_col] = a[pos(global_a_row, global_a_col, lda)];
            }
        } else {
            a_cache[local_row][local_col] = 0;
        }

        if (global_b_row < K && global_b_col < N) {
            if (is_transb) {
                b_cache[local_col][local_row] = b[tpos(global_b_row, global_b_col, ldb)];
            } else {
                b_cache[local_col][local_row] = b[pos(global_b_row, global_b_col, ldb)];
            }
        } else {
            b_cache[local_col][local_row] = 0;
        }

        // Wait for the entire block to be loaded.
        barrier(CLK_LOCAL_MEM_FENCE);

#pragma unroll
        for (int k = 0; k < BLOCK_SIZE; ++k) {
            running_sum += a_cache[local_row][k] * b_cache[local_col][k];
        }

        // Wait for the block to be fully consumed before loading the next
        // block.
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Store result in matrix C
    if (global_row < M && global_col < N) {
        int target_pos = pos(global_row, global_col, ldc);
        float v = c[target_pos];
        c[target_pos] = alpha * running_sum + beta * v;
    }
}




