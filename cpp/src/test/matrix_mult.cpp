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
// This src program executes a vector addition kernel to perform:
//  C = A + B
// where A, B and C are vectors with N elements.
//
// This src program supports partitioning the problem across multiple OpenCL
// devices if available. If there are M available devices, the problem is
// divided so that each device operates on N/M points. The src program
// assumes that all devices are of the same type (that is, the same binary can
// be used), but the code can be generalized to support different device types
// easily.
//
// Verification is performed against the same computation on the src CPU.
///////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include "saoclib.h"

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
    scoped_aligned_ptr<float> *a, *b, *c;
    a = new scoped_aligned_ptr<float>(A_size);
    b = new scoped_aligned_ptr<float>(B_size);
    c = new scoped_aligned_ptr<float>(C_size);
    for (unsigned i = 0; i < A_size; i++) {
        (*a)[i] = 3.14;
    }
    for (unsigned i = 0; i < B_size; i++) {
        (*b)[i] = 3.14;
    }
    /* wrap the raw data to kernelarg objects */
    ArgInt A_width_data = ArgInt(A_width, KernelArgMode::mode_input);
    ArgInt B_width_data = ArgInt(B_width, KernelArgMode::mode_input);
    ArgBufferFloat A_data = ArgBufferFloat(a, A_size, KernelArgMode::mode_input);
    ArgBufferFloat B_data = ArgBufferFloat(b, B_size, KernelArgMode::mode_input);
    ArgBufferFloat C_data = ArgBufferFloat(c, C_size, KernelArgMode::mode_output);
    KernelArg *args[5] = {&A_width_data, &B_width_data, &A_data, &B_data, &C_data};

    /* set inputs and output limits */
    KernelArgLimit arg_limits[5] =
            {KernelArgLimit(TypeTagPrimitive::getTypeTag<int>(), KernelArgMode::mode_input),
             KernelArgLimit(TypeTagPrimitive::getTypeTag<int>(), KernelArgMode::mode_input),
             KernelArgLimit(TypeTagArray::getTypeTag<float>(A_size), KernelArgMode::mode_input),
             KernelArgLimit(TypeTagArray::getTypeTag<float>(B_size), KernelArgMode::mode_input),
             KernelArgLimit(TypeTagArray::getTypeTag<float>(C_size), KernelArgMode::mode_output)};

    /* init an FPGA image */
    CLEnv env;
    env.initOpenCL();

    ClImage image(&env, "matrix_mult");
    auto device = env.getDeviceID(0);
    image.deployImage(&device, 1);

    /* init the kernel */
    size_t global_work_size[2] = {C_width, C_height};
    size_t local_work_size[2] = {BLOCK_SIZE, BLOCK_SIZE};
    NDRangeKernel kernel(2, global_work_size, local_work_size,
                         &image, device, "matrixMult", arg_limits, 5);
    /* call kernel with inputs and output */
    kernel.call(args, 5);

    /* print results */
    for (unsigned i = 0; i < C_size; i++) {
        printf("%f,", (*c)[i]);
    }

    return 0;
}


