#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include "../src/saoclib.hpp"

using namespace saoclib;

/**
 * This definition is used to avoid link errors when lookup implementation for function 'cleanup' declared in AOCLUtils.
 */
void cleanup() {};


// Entry point.
int main(int argc, char **argv) {
    /* assign the size of vector */
    size_t N = 100000;

    /* prepare raw data */
    scoped_aligned_ptr<float> a(N), b(N), c(N);
    for (unsigned i = 0; i < N; i++) {
        a[i] = 3.14;
    }
    for (unsigned i = 0; i < N; i++) {
        b[i] = 3.14;
    }

    /* wrap the raw data to KernelArg objects */
    AlignedBuffer<float> A_data{&a, N};
    AlignedBuffer<float> B_data{&b, N};
    AlignedBuffer<float> C_data{&c, N};
    std::vector<const KernelArg *> args{&A_data, &B_data};

    /* set inputs and output limits */
    std::vector<KernelArgLimit> input_limits = {KernelArgLimit::AlignedBufferLimit<float>(N),
                                                 KernelArgLimit::AlignedBufferLimit<float>(N)};
    KernelArgLimit output_limit = KernelArgLimit::AlignedBufferLimit<float>(N);

    /* init an FPGA image */
    FImage image("vector_add");
    image.init_opencl();
    /* init the kernel */
    size_t *global_work_size = &N;
    size_t *local_work_size = NULL;
    NDRangeKernel kernel(1, global_work_size, local_work_size,
                         &image, "vector_add", input_limits, output_limit);
    /* call kernel with inputs and output */
    kernel.call(args, &C_data);

    /* print results */
    for (unsigned i = 0; i < N; i++) {
        printf("%f,", c[i]);
    }

    return 0;
}


