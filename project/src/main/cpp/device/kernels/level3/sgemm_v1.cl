#define BLOCK_SIZE 64
#define SIMD_WORK_ITEMS 16
#define WORK_GROUP_SIZE0 (BLOCK_SIZE/SIMD_WORK_ITEMS)

#define pos(row, col, ld) ((ld)*(col)+(row))
#define tpos(row, col, ld) ((ld)*(row)+(col))

__kernel
__attribute((reqd_work_group_size(4, 4, 1)))
void sgemm(int transa, int transb,
           int M, int N, int K, float alpha,
           __global volatile float *restrict a, int lda,
           __global volatile float *restrict b, int ldb,
           float beta,
           __global float *restrict c, int ldc) {
    // Local storage for a block of input matrices A and B
    __local float
//    __attribute__((numbanks(BLOCK_SIZE), bankwidth(sizeof(float)),
//    singlepump, numreadports(1), numwriteports(1)))
            a_cache[BLOCK_SIZE][BLOCK_SIZE];
    __local float
//    __attribute__((numbanks(BLOCK_SIZE), bankwidth(sizeof(float)),
//    singlepump, numreadports(1), numwriteports(1)))
            b_cache[BLOCK_SIZE][BLOCK_SIZE];

    float acc[SIMD_WORK_ITEMS][SIMD_WORK_ITEMS];
    float a_reg[BLOCK_SIZE];
    // Initialise the accumulation registers
#pragma unroll
    for (int i = 0; i < SIMD_WORK_ITEMS; i++) {
#pragma unroll
        for (int j = 0; j < SIMD_WORK_ITEMS; j++) {
            acc[i][j] = 0.0f;
        }
    }

    // Block index
    int block_x = get_group_id(0);
    int block_y = get_group_id(1);

    // Local ID index (offset within a block)
    int local_row = get_local_id(0) * SIMD_WORK_ITEMS;
    int local_col = get_local_id(1) * SIMD_WORK_ITEMS;

    int global_row = block_x * BLOCK_SIZE + local_row;
    int global_col = block_y * BLOCK_SIZE + local_col;

    int n_tiles = K / BLOCK_SIZE;
    if (K - n_tiles * BLOCK_SIZE > 0) {
        n_tiles++;
    }

    for (int i = 0, global_a_col = local_col, global_b_row = local_row;
         i < n_tiles;
         i++, global_a_col += BLOCK_SIZE, global_b_row += BLOCK_SIZE) {
        // Load one tile of 'a' and 'b' into local memory
        for (int x = 0; x < SIMD_WORK_ITEMS; x++) {
            int global_a_row = global_row + x;
            int global_b_col = global_col + x;
            // Load 1 row of 'a' and 1 column of 'b' into local memory
#pragma unroll
            for (int y = 0; y < SIMD_WORK_ITEMS; y++) {
                a_cache[local_row + x][local_col + y] = a[pos(global_a_row, global_a_col + y, lda)];
                b_cache[local_col + x][local_row + y] = b[pos(global_b_row + y, global_b_col, ldb)];
            }
        }
        // Wait for the entire block to be loaded.
        barrier(CLK_LOCAL_MEM_FENCE);

        for (int x = 0; x < SIMD_WORK_ITEMS; x++) {
#pragma unroll
            // Load a row of a_cache to registers
            for (int e = 0; e < BLOCK_SIZE; e++) {
                a_reg[e] = a_cache[local_row + x][e];
            }
#pragma unroll
            // Perform the computation
            for (int y = 0; y < SIMD_WORK_ITEMS; y++) {
#pragma unroll
                for (int e = 0; e < BLOCK_SIZE; e++) {
                    acc[x][y] += a_reg[e] * b_cache[local_col + y][e];
                }
            }
        }

        // Wait for the block to be fully consumed before loading the next
        // block.
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Store result in matrix C
    // One column once
    for (int y = 0; y < SIMD_WORK_ITEMS; y++) {
#pragma unroll
        for (int x = 0; x < SIMD_WORK_ITEMS; x++) {
            c[pos(global_row + x, global_col + y, ldc)] = acc[x][y];
        }
    }
}
