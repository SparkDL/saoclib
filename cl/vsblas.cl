//public native static void vsAdd(int n, float[] a, int aOffset, float[] b, int bOffset,
//                                float[] y, int yOffset);
//
//public native static void vdAdd(int n, double[] a, int aOffset, double[] b, int bOffset,
//                                double[] y, int yOffset);
//
//public native static void vsSub(int n, float[] a, int aOffset, float[] b, int bOffset,
//                                float[] y, int yOffset);
//
//public native static void vdSub(int n, double[] a, int aOffset, double[] b, int bOffset,
//                                double[] y, int yOffset);
//
//public native static void vsMul(int n, float[] a, int aOffset, float[] b, int bOffset,
//                                float[] y, int yOffset);
//
//public native static void vdMul(int n, double[] a, int aOffset, double[] b, int bOffset,
//                                double[] y, int yOffset);
//
//public native static void vsDiv(int n, float[] a, int aOffset, float[] b, int bOffset,
//                                float[] y, int yOffset);
//
//public native static void vdDiv(int n, double[] a, int aOffset, double[] b, int bOffset,
//                                double[] y, int yOffset);
//
//public native static void vsPowx(int n, float[] a, int aOffset, float b, float[] y, int yOffset);
//
//public native static void vdPowx(int n, double[] a, int aOffset, double b, double[] y, int yOffset);
//
//public native static void vsLn(int n, float[] a, int aOffset, float[] y, int yOffset);
//
//public native static void vdLn(int n, double[] a, int aOffset, double[] y, int yOffset);
//
//public native static void vsExp(int n, float[] a, int aOffset, float[] y, int yOffset);
//
//public native static void vdExp(int n, double[] a, int aOffset, double[] y, int yOffset);
//
//public native static void vsSqrt(int n, float[] a, int aOffset, float[] y, int yOffset);
//
//public native static void vdSqrt(int n, double[] a, int aOffset, double[] y, int yOffset);
//
//public native static void vdTanh(int n, double[] a, int aOffset, double[] y, int yOffset);
//
//public native static void vsTanh(int n, float[] a, int aOffset, float[] y, int yOffset);
//
//public native static void vsLog1p(int n, float[] a, int aOffset, float[] y, int yOffset);
//
//public native static void vdLog1p(int n, double[] a, int aOffset, double[] y, int yOffset);
//
//public native static void vsAbs(int n, float[] a, int aOffset, float[] y, int yOffset);
//
//public native static void vdAbs(int n, double[] a, int aOffset, double[] y, int yOffset);

__kernel void vsAdd(__global const float *restrict x,
                    __global const float *restrict y,
                    __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = x[index] + y[index];
}

__kernel void vsSub(__global const float *restrict x,
                    __global const float *restrict y,
                    __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = x[index] - y[index];
}

__kernel void vsMul(__global const float *restrict x,
                    __global const float *restrict y,
                    __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = x[index] * y[index];
}

__kernel void vsDiv(__global const float *restrict x,
                    __global const float *restrict y,
                    __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = native_divide(x[index], y[index]);
}

__kernel void vsPowx(__global const float *restrict x,
                     const float y,
                     __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = pow(x[index], y);
}

__kernel void vsLn(__global const float *restrict x,
                   __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = log(x[index]);
}

__kernel void vsExp(__global const float *restrict x,
                    __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = exp(x[index]);
}

__kernel void vsSqrt(__global const float *restrict x,
                     __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = sqrt(x[index]);
}


__kernel void vsTanh(__global const float *restrict x,
                     __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = tanh(x[index]);
}

__kernel void vsLog1p(__global const float *restrict x,
                      __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = log1p(x[index]);
}

__kernel void vsAbs(__global const float *restrict x,
                    __global float *restrict z) {
    // get index of the work item
    int index = get_global_id(0);
    z[index] = fabs(x[index]);
}