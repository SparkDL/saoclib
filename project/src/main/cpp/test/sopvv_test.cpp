#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include "saoclib.h"

using namespace saoclib;

// Entry point.
int sopvv_test(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: *THIS_FILE* image_path function_name\n");
        exit(-1);
    }
    const char *image_path = argv[1];
    const char *op_name = argv[2];
    /* assign the size of vector */
    const size_t N = 100;
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
    FloatArrayArg A_data = FloatArrayArg(a->get(), N, KernelArgMode::input);
    FloatArrayArg B_data = FloatArrayArg(b->get(), N, KernelArgMode::input);
    FloatArrayArg C_data = FloatArrayArg(c->get(), N, KernelArgMode::output);
    KernelArg *args[num_args] = {&A_data, &B_data, &C_data};

    /* set inputs and output limits */
    using Sig = KernelArgSignature;
    using Mode = KernelArgMode;
    KernelArgSignature arg_limits[num_args] = {
            Sig::array<float>(Mode::input, N),
            Sig::array<float>(Mode::input, N),
            Sig::array<float>(Mode::output, N),
    };
    /* init an FPGA binary */
    CLEnv env;
    env.initOpenCL();

    CLBinary image(&env, image_path);
    auto device = env.getDeviceID(0);
    image.deploy(&device, 1);

    /* init the kernel */
    size_t global_work_size[1] = {N};
    NDRangeKernel kernel(&image, device, op_name, arg_limits, num_args);
    /* call kernel with inputs and output */
    kernel.call(1, global_work_size, NULL, args, num_args);

    /* print results */
    for (unsigned i = 0; i < N; i++) {
        printf("%f,", (*c)[i]);
    }

    /* test for multi calls */
    printf("call kernel of 2 times:\n");
    kernel.call(1, global_work_size, NULL,args, num_args);
    /* print results */
    for (unsigned i = 0; i < N; i++) {
        printf("%f,", (*c)[i]);
    }

    return 0;
}


