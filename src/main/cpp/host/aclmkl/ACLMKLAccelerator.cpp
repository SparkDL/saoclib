//
// Created by pcz on 8/27/18.
//

#include "config.h"
#include "ACLMKLAccelerator.h"
#include "ACLResourceManager.h"
#include "common.h"

namespace acl {
    using Sig = KernelArgSignature;
    using Mode = KernelArgMode;

    ACLMKLAccelerator::ACLMKLAccelerator(ACLResourceManager *manager, cl_device_id device)
            : manager(manager), device(device) {
        cl_int status;
        queue = clCreateCommandQueue(manager->getContext()->getRawContext(), device, NULL, &status);
        checkError(status, "Failed to create command queue");
        saxpyKernel = newAxpyKernel();
        sdotKernel = newSdotKernel();
        sopvKernel = newSopvKernel();
        sopvvKernel = newSopvvKernel();
        sscalKernel = newSscalKernel();
        vsPowxKernel = newVspowxKernel();
        sgemmKernel = newSgemmKernel();
    }

    ACLMKLAccelerator::~ACLMKLAccelerator() {
        clReleaseCommandQueue(queue);
        delete this->saxpyKernel;
        delete this->sdotKernel;
        delete this->sopvKernel;
        delete this->sopvvKernel;
        delete this->vsPowxKernel;
        delete this->sgemmKernel;
    }

    bool ACLMKLAccelerator::refreshBenchmarkResultFile() {
#ifdef BENCHMARK
        return this->sgemmKernel->refreshBenchmarkResultFile();
#else
        return true;
#endif
    }

    void ACLMKLAccelerator::sopvv(int opCode, int n, float *a, float *b, float *y) {
        static const int sopvvNumArgs = 4;
        IntArg arg_opCode = IntArg(opCode, Mode::input);
        FloatArrayArg arg_a = FloatArrayArg(a, static_cast<size_t>(n), Mode::input);
        FloatArrayArg arg_b = FloatArrayArg(b, static_cast<size_t>(n), Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, static_cast<size_t>(n), Mode::output);
        KernelArg *args[] = {&arg_opCode, &arg_a, &arg_b, &arg_y};

        size_t global_work_size[1] = {static_cast<size_t>(n)};
        sopvvKernel->call(1, global_work_size, NULL, args, sopvvNumArgs);
    }

    void ACLMKLAccelerator::sopv(int opCode, int n, float *a, float *y) {
        static const int sopvNumArgs = 3;
        IntArg arg_opCode = IntArg(opCode, Mode::input);
        FloatArrayArg arg_a = FloatArrayArg(a, static_cast<size_t>(n), Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, static_cast<size_t>(n), Mode::output);
        KernelArg *args[] = {&arg_opCode, &arg_a, &arg_y};

        size_t global_work_size[1] = {static_cast<size_t>(n)};
        sopvKernel->call(1, global_work_size, NULL, args, sopvNumArgs);
    }

    void ACLMKLAccelerator::vsAdd(int n, float *a, float *b, float *y) {
        sopvv(ADD, n, a, b, y);
    }

    void ACLMKLAccelerator::vsSub(int n, float *a, float *b, float *y) {
        sopvv(SUB, n, a, b, y);
    }

    void ACLMKLAccelerator::vsMul(int n, float *a, float *b, float *y) {
        sopvv(MUL, n, a, b, y);
    }

    void ACLMKLAccelerator::vsDiv(int n, float *a, float *b, float *y) {
        sopvv(DIV, n, a, b, y);
    }

    void ACLMKLAccelerator::vsAbs(int n, float *a, float *y) {
        sopv(ABS, n, a, y);
    }

    void ACLMKLAccelerator::vsSqrt(int n, float *a, float *y) {
        sopv(SQRT, n, a, y);
    }

    void ACLMKLAccelerator::vsExp(int n, float *a, float *y) {
        sopv(EXP, n, a, y);
    }

    void ACLMKLAccelerator::vsLn(int n, float *a, float *y) {
        sopv(LN, n, a, y);
    }

    void ACLMKLAccelerator::vsLog1p(int n, float *a, float *y) {
        sopv(LOG1P, n, a, y);
    }

    void ACLMKLAccelerator::vsTanh(int n, float *a, float *y) {
        sopv(TANH, n, a, y);
    }

    void ACLMKLAccelerator::vsPowx(int n, float *a, float b, float *y) {
        static const int vsPowxNumArgs = 3;

        FloatArrayArg arg_a = FloatArrayArg(a, static_cast<size_t>(n), Mode::input);
        FloatArg arg_b = FloatArg(b, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, static_cast<size_t>(n), Mode::output);
        KernelArg *args[vsPowxNumArgs] = {&arg_a, &arg_b, &arg_y};

        size_t global_work_size[1] = {static_cast<size_t>(n)};
        vsPowxKernel->call(1, global_work_size, NULL, args, vsPowxNumArgs);
    }

    void ACLMKLAccelerator::cblas_saxpy(int n, float a, float *x, int incx, float *y, int incy) {
        static const int saxpyNumArgs = 5;

        int xLength = incx * n;
        int yLength = incy * n;

        assert(xLength != 0 && yLength != 0);
        FloatArg arg_a = FloatArg(a, Mode::input);
        FloatArrayArg arg_x = FloatArrayArg(x, xLength, Mode::input);
        IntArg arg_incx = IntArg(incx, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, yLength, Mode::input_output);
        IntArg arg_incy = IntArg(incy, Mode::input);
        KernelArg *args[saxpyNumArgs] = {&arg_a, &arg_x, &arg_incx, &arg_y, &arg_incy};

        size_t global_work_size[1] = {static_cast<size_t>(n)};
        saxpyKernel->call(1, global_work_size, NULL, args, saxpyNumArgs);
    }

    void ACLMKLAccelerator::cblas_sscal(int n, float scale, float *x, int incx) {
        static const int sscalNumArgs = 3;
        int xLength = incx * n;
        assert(xLength != 0);

        FloatArg arg_scale = FloatArg(scale, Mode::input);
        FloatArrayArg arg_x = FloatArrayArg(x, xLength, Mode::input_output);
        IntArg arg_incx = IntArg(incx, Mode::input);
        KernelArg *args[sscalNumArgs] = {&arg_scale, &arg_x, &arg_incx};

        size_t global_work_size[1] = {static_cast<size_t>(n)};
        sscalKernel->call(1, global_work_size, NULL, args, sscalNumArgs);
    }

    float ACLMKLAccelerator::cblas_sdot(int n, float *x, int incx, float *y, int incy) {
        static const int sdotNumArgs = 6;

        int xLength = incx * n;
        int yLength = incy * n;
        float result = 0;

        assert(xLength != 0 && yLength != 0);
        IntArg arg_n = IntArg(n, Mode::input);
        FloatArrayArg arg_x = FloatArrayArg(x, xLength, Mode::input);
        IntArg arg_incx = IntArg(incx, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, yLength, Mode::input);
        IntArg arg_incy = IntArg(incy, Mode::input);
        FloatArrayArg arg_result = FloatArrayArg(&result, 1, Mode::output);
        KernelArg *args[sdotNumArgs] = {&arg_n, &arg_x, &arg_incx, &arg_y, &arg_incy, &arg_result};

        size_t global_work_size[1] = {1};
        sdotKernel->call(1, global_work_size, NULL, args, sdotNumArgs);
        return result;
    }

    void
    ACLMKLAccelerator::cblas_sger(CBLAS_ORDER order,
                                  int m, int n, float alpha,
                                  float *x, int incx,
                                  float *y, int incy,
                                  float *a, int lda) {
        assert(order == CblasColMajor && "Only col-major storage is supported now.");
        assert(incx == 1 && incy == 1 && "Only incx=1 and incy=1 is supported now.");
        int M = m, K = 1, N = n;
        cblas_sgemm(order,
                    CblasNoTrans, CblasNoTrans,
                    M, N, K,
                    alpha,
                    x, M,
                    y, 1,
                    1,
                    a, lda);
    }

    void ACLMKLAccelerator::cblas_sgemv(CBLAS_ORDER order,
                                        CBLAS_TRANSPOSE trans,
                                        int m, int n,
                                        float alpha,
                                        float *a, int lda,
                                        float *x, int incx,
                                        float beta,
                                        float *y, int incy) {
        assert(incx == 1 && incy == 1 && "Only incx=1 and incy=1 is supported now.");
        int M = m, K = n, N = 1;
        cblas_sgemm(order,
                    trans, CblasNoTrans,
                    M, N, K,
                    alpha,
                    a, lda,
                    x, K,
                    beta,
                    y, K);
    }

    void ACLMKLAccelerator::cblas_sgemm(CBLAS_ORDER order,
                                        CBLAS_TRANSPOSE transa, CBLAS_TRANSPOSE transb,
                                        int M, int N, int K,
                                        float alpha,
                                        float *a, int lda,
                                        float *b, int ldb,
                                        float beta,
                                        float *c, int ldc) {
        assert(order == CblasColMajor && "Only col major order is supported now.");
        log("M:%d,N:%d,K:%d,transa:%d:transb:%d\n", M, N, K, isTrans(transa), isTrans(transb));
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
        sgemmKernel->call(work_dim,
                          global_work_size,
                          local_work_size,
                          args, 6);
    }

    NDRangeKernel *ACLMKLAccelerator::newAxpyKernel() {
        static const int sdotNumArgs = 5;

        static KernelArgSignature sdotSignature[sdotNumArgs] = {
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::output)
        };

        return new NDRangeKernel(manager->getProgram(),
                                 device,
                                 queue,
                                 "saxpy",
                                 sdotSignature,
                                 sdotNumArgs);
    }

    NDRangeKernel *ACLMKLAccelerator::newSdotKernel() {
        static const int sdotNumArgs = 6;

        static KernelArgSignature sdotSignature[sdotNumArgs] = {
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::output, 0)
        };

        return new NDRangeKernel(manager->getProgram(),
                                 device,
                                 queue,
                                 "sdot",
                                 sdotSignature,
                                 sdotNumArgs);
    }

    NDRangeKernel *ACLMKLAccelerator::newSopvKernel() {
        static const int sopvNumArgs = 3;

        static KernelArgSignature sopvSignature[sopvNumArgs] = {
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::array<float>(Mode::output, 0)
        };

        return new NDRangeKernel(manager->getProgram(),
                                 device,
                                 queue,
                                 "sopv",
                                 sopvSignature,
                                 sopvNumArgs);
    }

    NDRangeKernel *ACLMKLAccelerator::newSopvvKernel() {
        static const int sopvvNumArgs = 4;
        static KernelArgSignature signature[] = {
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::array<float>(Mode::input, 0),
                Sig::array<float>(Mode::output, 0)
        };

        return new NDRangeKernel(manager->getProgram(),
                                 device,
                                 queue,
                                 "sopvv",
                                 signature,
                                 sopvvNumArgs);
    }

    NDRangeKernel *ACLMKLAccelerator::newSscalKernel() {
        static const int sscalNumArgs = 3;

        static KernelArgSignature sscalSignature[sscalNumArgs] = {
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::input_output, 0),
                Sig::primitive<int>(Mode::input),
        };

        return new NDRangeKernel(manager->getProgram(),
                                 device,
                                 queue,
                                 "sscal",
                                 sscalSignature,
                                 sscalNumArgs);
    }

    NDRangeKernel *ACLMKLAccelerator::newVspowxKernel() {
        static const int vsPowxNumArgs = 3;

        static KernelArgSignature vsPowxSignature[vsPowxNumArgs] = {
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::output, 0)
        };

        return new NDRangeKernel(manager->getProgram(),
                                 device,
                                 queue,
                                 "vsPowx",
                                 vsPowxSignature,
                                 vsPowxNumArgs);
    }

    NDRangeKernel *ACLMKLAccelerator::newSgemmKernel() {
        static const int sgemmNumArgs = 6;
        static KernelArgSignature signature[sgemmNumArgs] = {
                Sig::primitive<int>(Mode::input),
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::input_output, 0),
        };

        return new NDRangeKernel(manager->getProgram(),
                                 device,
                                 queue,
                                 "sgemm",
                                 signature,
                                 sgemmNumArgs);
    }


}
