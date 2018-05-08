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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "CL/opencl.h"
#include "AOCLUtils/aocl_utils.h"
#include "Kernel.hpp"

using namespace aocl_utils;


void cleanup();


// Entry point.
int main(int argc, char **argv) {
    /* prepare input data */

    // 128*128 matrix
    int N = 2;
    int BLOCK_SIZE = 64;
    int A_height = N * BLOCK_SIZE, A_width = N * BLOCK_SIZE;
    int B_height = A_width, B_width = N * BLOCK_SIZE;
    int C_height = A_height, C_width = B_width;
    size_t A_size = A_height * A_width, B_size = B_height * B_width, C_size = C_height * C_width;
    scoped_aligned_ptr<float> a(A_size), b(B_size), c(C_size);
    for (unsigned i = 0; i < A_size; i++) {
        a[i] = 3.14;
    }
    for (unsigned i = 0; i < B_size; i++) {
        b[i] = 3.14;
    }


    /* prepare data */
    Primitive A_width_data{&A_width, sizeof(A_width)};
    Primitive B_width_data{&B_width, sizeof(B_width)};
    AlignedBuffer<float> a_data{&a, A_size};
    AlignedBuffer<float> b_data{&b, B_size};
    std::vector<const KernelData *> args{&A_width_data, &B_width_data, &a_data, &b_data};

    size_t global_work_size[2] = {C_width, C_height};
    size_t local_work_size[2] = {BLOCK_SIZE, BLOCK_SIZE};
    printf("global_work_size:%lu,%lu\n", global_work_size[0], global_work_size[1]);
    printf("local_work_size:%lu,%lu\n", local_work_size[0], local_work_size[1]);

    /* invoke binary */
    std::vector<KernelDataLimit> input_limits = {{KernelDataType::Primitive,     sizeof(int),   0},
                                                 {KernelDataType::Primitive,     sizeof(int),   0},
                                                 {KernelDataType::AlignedBuffer, sizeof(float), A_size},
                                                 {KernelDataType::AlignedBuffer, sizeof(float), B_size}};

    KernelDataLimit output_limit = {KernelDataType::AlignedBuffer, sizeof(float), C_size};

    FImage image("matrix_mult");
    image.init_opencl();
    NDRangeKernel kernel(2, global_work_size, local_work_size,
                         &image, "matrixMult", input_limits, output_limit);
    kernel.call(args, c.get(), C_size* sizeof(float));

    for (unsigned i = 0; i < C_size; i++) {
        if (c[i] == 0) {
            printf("i: %lu, total: %lu, ratio: %d", i, C_size, C_size / i);
            break;
        }
    }

    return 0;
}

void cleanup() {

}

