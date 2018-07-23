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
    if (argc != 2) {
        printf("please assign the operation op_name\n");
        exit(-1);
    }
    const char *op_name = argv[1];
    /* assign the size of vector */
    const size_t N = 100;
    const unsigned num_args = 2;

    /* prepare raw data */
    scoped_aligned_ptr<float> *a, *c;
    a = new scoped_aligned_ptr<float>(N);
    c = new scoped_aligned_ptr<float>(N);
    for (unsigned i = 0; i < N; i++) {
        (*a)[i] = 3.14;
    }


    /* wrap the raw data to kernelarg objects */
    ArgBufferFloat A_data = ArgBufferFloat(a, N, KernelArgMode::mode_input);
    ArgBufferFloat C_data = ArgBufferFloat(c, N, KernelArgMode::mode_output);
    KernelArg *args[num_args] = {&A_data, &C_data};

    /* set inputs and output limits */
    KernelArgLimit arg_limits[num_args] = {
            KernelArgLimit(TypeTagArray::getTypeTag<float>(N), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagArray::getTypeTag<float>(N), KernelArgMode::mode_output)
    };
    /* init an FPGA image */
    CLEnv env;
    env.initOpenCL();

    CLImage image(&env, "vsblas");
    auto device = env.getDeviceID(0);
    image.deployImage(&device, 1);

    /* init the kernel */
    size_t global_work_size[1] = {N};
    NDRangeKernel kernel(1, global_work_size, NULL,
                         &image, device, op_name, arg_limits, num_args);
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


