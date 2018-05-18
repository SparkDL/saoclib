#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include "../saoclib.h"

using namespace saoclib;

/**
 * This definition is used to avoid link errors when lookup implementation for function 'cleanup' declared in AOCLUtils.
 */
void cleanup() {};


// Entry point.
int main(int argc, char **argv) {
    /* assign the size of vector */
    const size_t N = 100000;
    const unsigned num_args = 3;

    /* prepare raw data */
    scoped_aligned_ptr<float> *a, *b, *c;
    a = new scoped_aligned_ptr<float>(N);
    b = new scoped_aligned_ptr<float>(N);
    c = new scoped_aligned_ptr<float>(N);
    for (unsigned i = 0; i < N; i++) {
        (*a)[i] = 3.14;
        (*b)[i] = 3.14;
    }


    /* wrap the raw data to kernelarg objects */
    ArgBufferFloat A_data = ArgBufferFloat(a, N, KernelArgMode::mode_input);
    ArgBufferFloat B_data = ArgBufferFloat(b, N, KernelArgMode::mode_input);
    ArgBufferFloat C_data = ArgBufferFloat(c, N, KernelArgMode::mode_output);
    KernelArg *args[num_args] = {&A_data, &B_data, &C_data};

    /* set inputs and output limits */
    KernelArgLimit arg_limits[num_args] = {
            KernelArgLimit(TypeTagArray::newTypeTag<float>(N), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagArray::newTypeTag<float>(N), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagArray::newTypeTag<float>(N), KernelArgMode::mode_output)
    };
    /* init an FPGA image */
    CLEnv env;
    env.initOpenCL();

    ClImage image(&env, "vector_add");
    auto device = env.getDeviceID(0);
    image.deployImage(&device, 1);

    /* init the kernel */
    size_t global_work_size[1] = {N};
    NDRangeKernel kernel(1, global_work_size, NULL,
                         &image, device, "vector_add", arg_limits, num_args);
    /* call kernel with inputs and output */
    kernel.call(args, num_args);

    /* print results */
    for (unsigned i = 0; i < N; i++) {
        printf("%f,", (*c)[i]);
    }

    /* test for multi calls */
    printf("call kernel of 2 times:\n");
    kernel.call(args, num_args);
    /* print results */
    for (unsigned i = 0; i < N; i++) {
        printf("%f,", (*c)[i]);
    }

    return 0;
}


