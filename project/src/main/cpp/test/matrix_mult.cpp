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
#include "saoclib.h"

using namespace saoclib;

float *matrixMult(float *a, int aH, int aW,
                  float *b, int bH, int bW);

// Entry point.
int matrix_mult_test(int argc, char **argv) {
    const int aH = 2, aW = 3, bH = 3, bW = 4;
    const int cH = aH, cW = bW;
    float a[aH][aW] = {
            {1, 2, 3},
            {1, 2, 3}
    };
    float b[bH][bW] = {
            {1, 1, 1, 1},
            {2, 2, 2, 2},
            {3, 3, 3, 3}
    };

    auto ret = matrixMult((float *) a, aH, aW, (float *) b, bH, bW);


    for (int i = 0; i < cH; i++) {
        for (int j = 0; j < cW; j++) {
            printf("%f,", ret[i * cW + j]);
        }
        printf("\n");
    }
}


static void test() {
    /* prepare input data */

    /* set size of matrices */
    int N = 4;
    int BLOCK_SIZE = 1;
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
        (*a)[i] = i + 1;
    }
    for (unsigned i = 0; i < B_size; i++) {
        (*b)[i] = i + 1;
    }
    /* wrap the raw data to kernelarg objects */
    IntArg A_width_data = IntArg(A_width, KernelArgMode::input);
    IntArg B_width_data = IntArg(B_width, KernelArgMode::input);
    FloatArrayArg A_data = FloatArrayArg(a->get(), A_size, KernelArgMode::input);
    FloatArrayArg B_data = FloatArrayArg(b->get(), B_size, KernelArgMode::input);
    FloatArrayArg C_data = FloatArrayArg(c->get(), C_size, KernelArgMode::output);
    KernelArg *args[5] = {&A_width_data, &B_width_data, &A_data, &B_data, &C_data};

    /* set inputs and output limits */
    using Sig = KernelArgSignature;
    using Mode = KernelArgMode;
    KernelArgSignature arg_limits[5] = {
            Sig::primitive<int>(Mode::input),
            Sig::primitive<int>(Mode::input),
            Sig::array<float>(Mode::input, A_size),
            Sig::array<float>(Mode::input, B_size),
            Sig::array<float>(Mode::output, C_size),
    };

    /* init an FPGA binary */
    CLEnv env;
    env.initOpenCL();

    CLBinary image(&env, "matrix_mult");
    auto device = env.getDeviceID(0);
    image.deploy(&device, 1);

    /* init the kernel */
    size_t global_work_size[2] = {C_width, C_height};
    size_t local_work_size[2] = {BLOCK_SIZE, BLOCK_SIZE};
    NDRangeKernel kernel(&image, device, "matrixMult", arg_limits, 5);
    /* call kernel with inputs and output */
    kernel.call(2, global_work_size, local_work_size, args, 5);

    /* print results */
    for (unsigned i = 0; i < C_size; i++) {
        printf("%f,", (*c)[i]);
    }
}

float *matrixMult(float *a, int aH, int aW,
                  float *b, int bH, int bW) {

    /* prepare input data */

    /* set size of matrices */
    int N = 4;
    int BLOCK_SIZE = 1;
    int cH = aH, cW = bW;
    size_t aSize = aH * aW, bSize = bH * bW, cSize = cH * cW;

    /* prepare raw data */
    scoped_aligned_ptr<float> *aSap, *bSap, *cSap;
    aSap = new scoped_aligned_ptr<float>();
    bSap = new scoped_aligned_ptr<float>();
    aSap->reset(a);
    bSap->reset(b);
    cSap = new scoped_aligned_ptr<float>(cSize);

    /* wrap the raw data to kernelarg objects */
    IntArg A_width_data = IntArg(aW, KernelArgMode::input);
    IntArg B_width_data = IntArg(bW, KernelArgMode::input);
    FloatArrayArg A_data = FloatArrayArg(aSap->get(), aSize, KernelArgMode::input);
    FloatArrayArg B_data = FloatArrayArg(bSap->get(), bSize, KernelArgMode::input);
    FloatArrayArg C_data = FloatArrayArg(cSap->get(), cSize, KernelArgMode::output);
    KernelArg *args[5] = {&A_width_data, &B_width_data, &A_data, &B_data, &C_data};

    /* set inputs and output limits */
    using Sig = KernelArgSignature;
    using Mode = KernelArgMode;
    KernelArgSignature arg_limits[5] = {
            Sig::primitive<int>(Mode::input),
            Sig::primitive<int>(Mode::input),
            Sig::array<float>(Mode::input, aSize),
            Sig::array<float>(Mode::input, bSize),
            Sig::array<float>(Mode::output, cSize),
    };

    /* init an FPGA binary */
    CLEnv env;
    env.initOpenCL();

    CLBinary image(&env, "matrix_mult");
    auto device = env.getDeviceID(0);
    image.deploy(&device, 1);

    /* init the kernel */
    size_t global_work_size[2] = {cW, cH};
    size_t local_work_size[2] = {BLOCK_SIZE, BLOCK_SIZE};
    NDRangeKernel kernel(&image, device, "matrixMult", arg_limits, 5);
    /* call kernel with inputs and output */
    kernel.call(2, global_work_size, local_work_size, args, 5);

    return cSap->get();
}

