#define BLOCK_SIZE 64
#define SIMD_WORK_ITEMS 16

#define pos(row, col, ld) ((ld)*(col)+(row))
#define tpos(row, col, ld) ((ld)*(row)+(col))

__kernel
__attribute((reqd_work_group_size(BLOCK_SIZE/SIMD_WORK_ITEMS, BLOCK_SIZE, 1)))
void sgemm(int transa, int transb,
           int M, int N, int K, float alpha,
           __global volatile float *restrict a, int lda,
           __global volatile float *restrict b, int ldb,
           float beta,
           __global float *restrict c, int ldc) {
    // Local storage for a block of input matrices A and B
    __local float a_cache[BLOCK_SIZE][BLOCK_SIZE];
    __local float b_cache[BLOCK_SIZE][BLOCK_SIZE];

    // One thread compute 16 rows * 1 column
    float acc[SIMD_WORK_ITEMS];
    // Initialise the accumulation registers
#pragma unroll
    for (int i = 0; i < SIMD_WORK_ITEMS; i++) {
        acc[i] = 0.0f;
    }

    // Global ID index
    int global_row = get_global_id(0) * SIMD_WORK_ITEMS;
    int global_col = get_global_id(1);

    // Local ID index (offset within a block)
    int local_row = get_local_id(0) * SIMD_WORK_ITEMS;
    int local_col = get_local_id(1);

    int n_tiles = K / BLOCK_SIZE;
    if (K - n_tiles * BLOCK_SIZE > 0) {
        n_tiles++;
    }

    for (int i = 0, global_a_col = local_col, global_b_row = local_row;
         i < n_tiles;
         i++, global_a_col += BLOCK_SIZE, global_b_row += BLOCK_SIZE) {
        // Load one tile of 'a' and 'b' into local memory
#pragma unroll
        for (int x = 0; x < SIMD_WORK_ITEMS; x++) {
            a_cache[local_col][local_row + x] = a[pos(global_row + x, global_a_col, lda)];
            b_cache[local_col][local_row + x] = b[pos(global_b_row + x, global_col, ldb)];
        }

        // Wait for the entire block to be loaded.
        barrier(CLK_LOCAL_MEM_FENCE);
#pragma unroll
        for (int e = 0; e < BLOCK_SIZE; e++) {
#pragma unroll
            for (int x = 0; x < SIMD_WORK_ITEMS; x++) {
                acc[x] += a_cache[e][local_row + x] * b_cache[local_col][e];
            }
        }

        // Wait for the block to be fully consumed before loading the next block.
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Store results into matrix C, 1 column once
#pragma unroll
    for (int x = 0; x < SIMD_WORK_ITEMS; x++) {
        c[pos(global_row + x, global_col, ldc)] = acc[x];
    }
}
