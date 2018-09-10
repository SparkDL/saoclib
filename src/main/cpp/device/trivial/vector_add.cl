
__kernel void vector_add(__global const float16 *restrict x,
                         __global const float16 *restrict y,
                         __global float16 *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = x[index] + y[index];
}