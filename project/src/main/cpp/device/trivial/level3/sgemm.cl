#define BLOCK_SIZE 64

__kernel
__a
__attribute((reqd_work_group_size(BLOCK_SIZE, BLOCK_SIZE, 1)))
void sgemm(int M, int N, int K,
        // Input and output matrices
           __global float *restrict A,
           __global float *restrict B,
           __global float *restrict C) {
    // Local storage for a block of input matrices A and B
    __local float a_cache[BLOCK_SIZE][BLOCK_SIZE];
    __local float b_cache[BLOCK_SIZE][BLOCK_SIZE];

    // Block index
    int block_x = get_group_id(0);
    int block_y = get_group_id(1);

    // Local ID index (offset within a block)
    int row = get_local_id(0);
    int col = get_local_id(1);

    // Compute loop bounds
    int a_start = M * BLOCK_SIZE * block_y;
    int a_end = a_start + M - 1;
    int b_start = BLOCK_SIZE * block_x;

    float running_sum = 0.0f;
    for (int a = a_start, b = b_start; a <= a_end; a += BLOCK_SIZE, b += (BLOCK_SIZE * N)) {
        a_cache[col][row] = A[a + M * col + row];
        b_cache[row][col] = B[b + N * col + row];

        // Wait for the entire block to be loaded.
        barrier(CLK_LOCAL_MEM_FENCE);

#pragma unroll
        for (int k = 0; k < BLOCK_SIZE; ++k) {
            running_sum += a_cache[col][k] * b_cache[row][k];
        }

        // Wait for the block to be fully consumed before loading the next
        // block.
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Store result in matrix C
    C[get_global_id(1) * get_global_size(0) + get_global_id(0)] = running_sum;
}
