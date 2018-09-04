//
// Created by pcz on 9/2/18.
//

#ifndef SAOCLIB_CPP_LEVEL3_TEST_HPP
#define SAOCLIB_CPP_LEVEL3_TEST_HPP

#include <iostream>
#include <cmath>
#include "acl.h"
#include "utils.h"

#define BLOCK_SIZE 64
#define SIMD_WORK_ITEMS 16

using namespace acl;
using Sig = KernelArgSignature;
using Mode = KernelArgMode;
NDRangeKernel *kernel = NULL;

void sgemm(int transa, int transb,
           int M, int N, int K,
           float alpha,
           float *a, int lda,
           float *b, int ldb,
           float beta,
           float *c, int ldc);

int main(int argc, char **argv) {
    const int f = 1;
    int transa = 0;
    int transb = 0;
    int M = f * BLOCK_SIZE + 1;
    int N = f * BLOCK_SIZE + 1;
    int K = f * BLOCK_SIZE + 1;
    float alpha = 1;
    float beta = 1;
    int lda = M;
    int ldb = K;
    int ldc = M;

    auto a = new float[lda * K];
    auto b = new float[ldb * N];
    auto c = new float[ldc * N];
    for (int i = 0; i < lda * K; i++) {
        a[i] = 1;
        std::cout << a[i] << ",";
    }
    std::cout << std::endl;
    for (int i = 0; i < ldb * N; i++) {
        b[i] = 1;
        std::cout << a[i] << ",";
    }
    std::cout << std::endl;

    double time = executeTime(
            [=]() -> void {
                sgemm(transa, transb, M, N, K, alpha, a, lda, b, ldb, beta, c, ldc);
            },
            "sgemm"
    );

    double flops = 2.0 * M * N * K / time;
    printf("\nThroughput: %0.5f GFLOPS\n\n", flops * 1e-9);

    for (int i = 0; i < ldc * N; i++) {
        if (i % ldc == 0) {
            std::cout << std::endl;
        }
        std::cout << c[i] << ",";
    }

    return 0;
}

bool init() {
    const int sgemmNumArgs = 13;
    Context *context = new Context();
    context->initOpenCL();
    Program *program = new Program(context, "/home/pcz/develop/saoclib/project/target/sgemm");
    program->deploy(context->getDevices());

    KernelArgSignature signature[sgemmNumArgs] = {
            Sig::primitive<int>(Mode::input),
            Sig::primitive<int>(Mode::input),
            Sig::primitive<int>(Mode::input),
            Sig::primitive<int>(Mode::input),
            Sig::primitive<int>(Mode::input),
            Sig::primitive<float>(Mode::input),
            Sig::array<float>(Mode::input, 0),
            Sig::primitive<int>(Mode::input),
            Sig::array<float>(Mode::input, 0),
            Sig::primitive<int>(Mode::input),
            Sig::primitive<float>(Mode::input),
            Sig::array<float>(Mode::input_output, 0),
            Sig::primitive<int>(Mode::input)
    };

    cl_int status;
    cl_device_id device = program->getDeployedDevice(0);
    cl_command_queue queue = clCreateCommandQueue(context->getRawContext(),
                                                  device, NULL, &status);
    checkError(status, "Failed to create command queue");
    kernel = new NDRangeKernel(program,
                               program->getDeployedDevice(0),
                               queue,
                               "sgemm",
                               signature,
                               sgemmNumArgs);
    return true;
}


void sgemm(int transa, int transb,
           int M, int N, int K,
           float alpha,
           float *a, int lda,
           float *b, int ldb,
           float beta,
           float *c, int ldc) {
    static bool inited = init();
    auto transa_arg = IntArg(transa, Mode::input);
    auto transb_arg = IntArg(transb, Mode::input);
    auto M_arg = IntArg(M, Mode::input);
    auto N_arg = IntArg(N, Mode::input);
    auto K_arg = IntArg(K, Mode::input);
    auto alpha_arg = FloatArg(alpha, Mode::input);
    auto a_arg = FloatArrayArg(a, lda * K, Mode::input);
    auto lda_arg = IntArg(lda, Mode::input);
    auto b_arg = FloatArrayArg(b, ldb * N, Mode::input);
    auto ldb_arg = IntArg(ldb, Mode::input);
    auto beta_arg = FloatArg(beta, Mode::input);
    auto c_arg = FloatArrayArg(c, ldc * N, Mode::input_output);
    auto ldc_arg = IntArg(ldc, Mode::input);
    KernelArg *args[] = {&transa_arg, &transb_arg,
                         &M_arg, &N_arg, &K_arg,
                         &alpha_arg,
                         &a_arg, &lda_arg,
                         &b_arg, &ldb_arg,
                         &beta_arg,
                         &c_arg, &ldc_arg};
    const size_t work_dim = 2;
    int m_factor = std::ceil(M / float(BLOCK_SIZE));
    int n_factor = std::ceil(N / float(BLOCK_SIZE));
    const size_t global_work_size[work_dim] = {m_factor * BLOCK_SIZE, n_factor * BLOCK_SIZE};
    const size_t local_work_size[work_dim] = {BLOCK_SIZE, BLOCK_SIZE};
    kernel->call(work_dim,
                 global_work_size,
                 local_work_size,
                 args, 13);
}

#endif //SAOCLIB_CPP_LEVEL3_TEST_HPP
