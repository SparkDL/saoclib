#include <iostream>
#include "acl.h"

#define BLOCK_SIZE 64
using namespace acl;
using Sig = KernelArgSignature;
using Mode = KernelArgMode;
NDRangeKernel *kernel = NULL;

void sgemm(CBLAS_TRANSPOSE transa, CBLAS_TRANSPOSE transb,
           int M, int N, int K,
           float alpha,
           float *a, int lda,
           float *b, int ldb,
           float beta,
           float *c, int ldc);

int main(int argc, char **argv) {
    const int f = 1;
    auto transa = CblasNoTrans;
    auto transb = CblasNoTrans;
    int M = f * BLOCK_SIZE;
    int N = f * BLOCK_SIZE;
    int K = f * BLOCK_SIZE;
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
    }
    printMatrix("a", a, lda, K);
    std::cout << std::endl;
    for (int i = 0; i < ldb * N; i++) {
        b[i] = 1;
    }
    printMatrix("b", b, ldb, N);
    for (int i = 0; i < ldc * N; i++) {
        c[i] = 1;
    }
    printMatrix("c", c, ldc, N);
    std::cout << std::endl;

    sgemm(transa, transb, M, N, K, alpha, a, lda, b, ldb, beta, c, ldc);

    printMatrix("c", c, ldc, N);

    return 0;
}

bool init() {

    Context *context = new Context();
    context->initOpenCL();
    Program *program = new Program(context, "/home/pcz/aocx/sgemm_simd8_no_option.aocx");
    program->deploy(context->getDevices());

    static const int sgemmNumArgs = 6;
    static KernelArgSignature signature[sgemmNumArgs] = {
            Sig::primitive<int>(Mode::input),
            Sig::primitive<float>(Mode::input),
            Sig::array<float>(Mode::input, 0),
            Sig::array<float>(Mode::input, 0),
            Sig::primitive<float>(Mode::input),
            Sig::array<float>(Mode::input_output, 0),
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


void sgemm(CBLAS_TRANSPOSE transa, CBLAS_TRANSPOSE transb,
           int M, int N, int K,
           float alpha,
           float *a, int lda,
           float *b, int ldb,
           float beta,
           float *c, int ldc) {
    static bool inited = init();

    auto K_arg = IntArg(paddedSize(K, BLOCK_SIZE), Mode::input);
    auto alpha_arg = FloatArg(alpha, Mode::input);
    auto a_arg = Matrix<float>(a, lda, M, K, isTrans(transa), Mode::input);
    auto b_arg = Matrix<float>(b, ldb, K, N, isTrans(transb), Mode::input);
    auto beta_arg = FloatArg(beta, Mode::input);
    auto c_arg = Matrix<float>(c, ldc, M, N, false, Mode::input_output);

    KernelArg *args[] = {&K_arg,
                         &alpha_arg,
                         &a_arg,
                         &b_arg,
                         &beta_arg,
                         &c_arg};

    const size_t work_dim = 2;
    const size_t global_work_size[work_dim] = {static_cast<const size_t>(paddedSize(M, BLOCK_SIZE)),
                                               static_cast<const size_t>(paddedSize(N, BLOCK_SIZE)),};
    const size_t local_work_size[work_dim] = {BLOCK_SIZE, BLOCK_SIZE};
    kernel->call(work_dim,
                 global_work_size,
                 local_work_size,
                                                                                                                                                                                             args, 6);
}
