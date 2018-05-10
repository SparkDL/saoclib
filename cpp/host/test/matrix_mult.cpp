// Copyright (C) 2013-2017 Altera Corporation, San Jose, California, USA. All rights reserved.
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to
// whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// This agreement shall be governed in all respects by the laws of the State of California and
// by the laws of the United States of America.

///////////////////////////////////////////////////////////////////////////////////
// This host program executes a vector addition kernel to perform:
//  C = A + B
// where A, B and C are vectors with N elements.
//
// This host program supports partitioning the problem across multiple OpenCL
// devices if available. If there are M available devices, the problem is
// divided so that each device operates on N/M points. The host program
// assumes that all devices are of the same type (that is, the same binary can
// be used), but the code can be generalized to support different device types
// easily.
//
// Verification is performed against the same computation on the host CPU.
///////////////////////////////////////////////////////////////////////////////////

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
    /* prepare input data */

    /* set size of matrices */
    int N = 2;
    int BLOCK_SIZE = 64;
    int A_height = N * BLOCK_SIZE, A_width = N * BLOCK_SIZE;
    int B_height = A_width, B_width = N * BLOCK_SIZE;
    int C_height = A_height, C_width = B_width;
    size_t A_size = A_height * A_width, B_size = B_height * B_width, C_size = C_height * C_width;

    /* prepare raw data */
    scoped_aligned_ptr<float> a(A_size), b(B_size), c(C_size);
    for (unsigned i = 0; i < A_size; i++) {
        a[i] = 3.14;
    }
    for (unsigned i = 0; i < B_size; i++) {
        b[i] = 3.14;
    }
    /* wrap the raw data to KernelArg objects */
    Primitive<int> A_width_data(A_width);
    Primitive<int> B_width_data(B_width);
    AlignedBuffer<float> A_data{&a, A_size};
    AlignedBuffer<float> B_data{&b, B_size};
    AlignedBuffer<float> C_data{&c, C_size};
    std::vector<const KernelArg *> args{&A_width_data, &B_width_data, &A_data, &B_data};

    /* set inputs and output limits */
    std::vector<KernelArgLimit> input_limits = {KernelArgLimit::PrimitiveLimit<int>(),
                                                 KernelArgLimit::PrimitiveLimit<int>(),
                                                 KernelArgLimit::AlignedBufferLimit<float>(A_size),
                                                 KernelArgLimit::AlignedBufferLimit<float>(B_size)};
    KernelArgLimit output_limit = KernelArgLimit::AlignedBufferLimit<float>(C_size);

    /* init an FPGA image */
    FImage image("matrix_mult");
    image.init_opencl();
    image.load_image();
    /* init the kernel */
    size_t global_work_size[2] = {C_width, C_height};
    size_t local_work_size[2] = {BLOCK_SIZE, BLOCK_SIZE};
    NDRangeKernel kernel(2, global_work_size, local_work_size,
                         &image, "matrixMult", input_limits, output_limit);
    /* call kernel with inputs and output */
    kernel.call(args, &C_data);

    /* print results */
    for (unsigned i = 0; i < C_size; i++) {
        printf("%f,", c[i]);
    }

    return 0;
}


