#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include "../saoclib.h"
#include "../../device/kernels/config.h"


using namespace saoclib;

/**
 * This definition is used to avoid link errors when lookup implementation for function 'cleanup' declared in AOCLUtils.
 */
void cleanup() {};


void sger(int m,
          int n,
          float alpha,
          float *x,
          int x_size,
          int incx,
          float *y,
          int y_size,
          int incy,
          float *a,
          int a_size,
          int lda) {
    const char *image_path = "/home/pcz/develop/saoclib/project/target/classes/aocl_blas";
    const char *op_name = "sger_v1";

    /* wrap the raw data to kernelarg objects */
    ArgInt m_data = ArgInt(m, KernelArgMode::mode_input);
    ArgInt n_data = ArgInt(n, KernelArgMode::mode_input);
    ArgFloat alpha_data = ArgFloat(alpha, KernelArgMode::mode_input);
    ArgBufferFloat x_data = ArgBufferFloat(x, x_size, KernelArgMode::mode_input);
    ArgInt incx_data = ArgInt(incx, KernelArgMode::mode_input);
    ArgBufferFloat y_data = ArgBufferFloat(y, y_size, KernelArgMode::mode_input);
    ArgInt incy_data = ArgInt(incy, KernelArgMode::mode_input);

    ArgBufferFloat a_data = ArgBufferFloat(a, a_size, KernelArgMode::mode_input_output);
    ArgInt lda_data = ArgInt(lda, KernelArgMode::mode_input);


    const int num_args = 9;
    KernelArg *args[num_args] = {&m_data, &n_data,
                                 &alpha_data,
                                 &x_data, &incx_data,
                                 &y_data, &incy_data,
                                 &a_data, &lda_data};

    /* set inputs and output limits */
    KernelArgLimit arg_limits[num_args] = {
            KernelArgLimit(TypeTagPrimitive::getTypeTag<int>(), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagPrimitive::getTypeTag<int>(), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagPrimitive::getTypeTag<float>(), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagArray::getTypeTag<float>(x_size), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagPrimitive::getTypeTag<int>(), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagArray::getTypeTag<float>(y_size), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagPrimitive::getTypeTag<int>(), KernelArgMode::mode_input),
            KernelArgLimit(TypeTagArray::getTypeTag<float>(a_size), KernelArgMode::mode_input_output),
            KernelArgLimit(TypeTagPrimitive::getTypeTag<int>(), KernelArgMode::mode_input)
    };
    /* init an FPGA image */
    CLEnv env;
    env.initOpenCL();

    CLImage image(&env, image_path);
    auto device = env.getDeviceID(0);
    image.deployImage(&device, 1);

    /* init the kernel */
    int global_x = x_size / SLICE_SIZE;
    if (x_size - global_x > 0) {
        global_x++;
    }
    int global_y = y_size / SLICE_SIZE;
    if (y_size - global_y > 0) {
        global_y++;
    }

    size_t global_work_size[2] = {global_x * SLICE_SIZE, global_y * SLICE_SIZE};
    size_t local_work_size[2] = {SLICE_SIZE, SLICE_SIZE};
    NDRangeKernel kernel(2, global_work_size, local_work_size,
                         &image, device, op_name, arg_limits, num_args);
    /* call kernel with inputs and output */
    kernel.call(args, num_args);
}


int main() {
    float a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    float b[] = {1, 1, 10, 10, 100, 100, 1000, 1000, 10000, 10000};
    float c[100] = {0,};

    int m = 8;
    int n = 10;
    float alpha = 1.0;
    int lda = 10;
    sger(m, n, alpha, a, 8, 1, b, 10, 1, c, 100, lda);

    int total = lda * n;
    for (int i = 0; i < total; i++) {
        if (i % n == 0) {
            std::cout << std::endl;
        }
        std::cout << c[i] << ",";
    }
    return 0;
}

