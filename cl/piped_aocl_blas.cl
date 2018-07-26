#pragma OPENCL EXTENSION cl_intel_fpga_host_pipe : enable

__kernel void vsAdd(int n,
                    __attribute__((intel_host_accessible, blocking, depth(1024))) __read_only pipe float x,
                    __attribute__((intel_host_accessible, blocking, depth(1024))) __read_only pipe float y,
                    __global float *restrict z) {
    float xe, ye;
    int i;
    for (i = 0; i < n; i++) {
        xe = read_pipe(x, &xe);
        ye = read_pipe(y, &ye);
        z[i] = xe + ye;
    }
}
