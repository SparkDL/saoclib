#define BLOCK_SIZE 64
#define SIMD_WORK_ITEMS 16

#define pos(row, col, ld) ((ld)*(col)+(row))
#define tpos(row, col, ld) ((ld)*(row)+(col))

__kernel
__attribute((reqd_work_group_size(1, 1, 1)))
void sgemm(int transa, int transb,
           int M, int N, int K, float alpha,
           __global volatile float *restrict a, int lda,
           __global volatile float *restrict b, int ldb,
           float beta,
           __global float *restrict c, int ldc) {
    // Local storage for a tile of input matrices 'a' and 'b'
    float a_cache[BLOCK_SIZE][BLOCK_SIZE];
    float b_cache[BLOCK_SIZE][BLOCK_SIZE];

    float result[BLOCK_SIZE][BLOCK_SIZE];

#pragma unroll
    for (int i = 0; i < BLOCK_SIZE; i++) {
#pragma unroll
        for (int j = 0; j < BLOCK_SIZE; j++) {
            result[i][j] = 0;
        }
    }

    int global_row = get_global_id(0) * BLOCK_SIZE;
    int global_col = get_global_id(1) * BLOCK_SIZE;

    int global_a_row = global_row;
    int global_b_col = global_col;

    int y_remain = N - global_col;
    if (y_remain > BLOCK_SIZE) {
        y_remain = BLOCK_SIZE;
    }

    int n_tiles = K / BLOCK_SIZE;
    if (K - n_tiles * BLOCK_SIZE > 0) {
        n_tiles++;
    }

    for (int i = 0, global_a_col = 0, global_b_row = 0;
         i < n_tiles;
         i++, global_a_col += BLOCK_SIZE, global_b_row += BLOCK_SIZE) {

        // Load one tile of 'a' and 'b' into cache
        for (int col = 0; col < BLOCK_SIZE; col++) {
#pragma unroll
            for (int row = 0; row < BLOCK_SIZE; row++) {

                int target_a_row = global_a_row + row;
                int target_a_col = global_a_col + col;
                if (target_a_row < M && target_a_col < K) {
                    a_cache[row][col] = a[pos(target_a_row, target_a_col, lda)];
                } else {
                    a_cache[row][col] = 0;
                }

                int target_b_row = global_b_row + row;
                int target_b_col = global_b_col + col;
                if (target_b_row < K && target_b_col < N) {
                    b_cache[row][col] = b[pos(target_b_row, target_b_col, ldb)];
                } else {
                    b_cache[row][col] = 0;
                }

            }
        }

        for (int y = 0; y < y_remain; y++) {
#pragma unroll SIMD_WORK_ITEMS
            for (int x = 0; x < BLOCK_SIZE; x++) {
#pragma unroll
                for (int e = 0; e < BLOCK_SIZE; e++) {
                    result[x][y] += a_cache[x][e] * b_cache[e][y];
                }
            }
        }
    }

    // Store results into matrix C, 1 column once
    for (int y = 0; y < y_remain; y++) {
#pragma unroll
        for (int x = 0; x < BLOCK_SIZE; x++) {
            int target_row = global_row + x;
            int target_col = global_col + y;
            if (target_row < M) {
                c[pos(target_row, target_col, ldc)] = result[x][y];
            }
        }
    }
}
