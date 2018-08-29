//
// Created by pcz on 8/27/18.
//

#include "config.h"
#include "ACLBlasAccelerator.h"
#include "ACLResourceManager.h"

namespace saoclib {
    using Sig = KernelArgSignature;
    using Mode = KernelArgMode;

    ACLBlasAccelerator::ACLBlasAccelerator(ACLResourceManager *manager, cl_device_id device)
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
        sgemvKernel = newSgemvKernel();
        sgerKernel = newSgerKernel();
        sgemmKernel = newSgemmKernel();
    }

    ACLBlasAccelerator::~ACLBlasAccelerator() {
        clReleaseCommandQueue(queue);
    }

    void ACLBlasAccelerator::sopvv(int opCode, int n, float *a, float *b, float *y) {
        static const int sopvvNumArgs = 4;
        IntArg arg_opCode = IntArg(opCode, Mode::input);
        FloatArrayArg arg_a = FloatArrayArg(a, n, Mode::input);
        FloatArrayArg arg_b = FloatArrayArg(b, n, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, n, Mode::output);
        KernelArg *args[] = {&arg_opCode, &arg_a, &arg_b, &arg_y};

        size_t global_work_size[1] = {n};
        sopvvKernel->call(1, global_work_size, NULL, args, sopvvNumArgs);
    }

    void ACLBlasAccelerator::sopv(int opCode, int n, float *a, float *y) {
        static const int sopvNumArgs = 3;
        IntArg arg_opCode = IntArg(opCode, Mode::input);
        FloatArrayArg arg_a = FloatArrayArg(a, n, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, n, Mode::output);
        KernelArg *args[] = {&arg_opCode, &arg_a, &arg_y};

        size_t global_work_size[1] = {n};
        sopvKernel->call(1, global_work_size, NULL, args, sopvNumArgs);
    }

    void ACLBlasAccelerator::vsAdd(int n, float *a, float *b, float *y) {
        sopvv(ADD, n, a, b, y);
    }

    void ACLBlasAccelerator::vsSub(int n, float *a, float *b, float *y) {
        sopvv(SUB, n, a, b, y);
    }

    void ACLBlasAccelerator::vsMul(int n, float *a, float *b, float *y) {
        sopvv(MUL, n, a, b, y);
    }

    void ACLBlasAccelerator::vsDiv(int n, float *a, float *b, float *y) {
        sopvv(DIV, n, a, b, y);
    }

    void ACLBlasAccelerator::vsAbs(int n, float *a, float *y) {
        sopv(ABS, n, a, y);
    }

    void ACLBlasAccelerator::vsSqrt(int n, float *a, float *y) {
        sopv(SQRT, n, a, y);
    }

    void ACLBlasAccelerator::vsExp(int n, float *a, float *y) {
        sopv(EXP, n, a, y);
    }

    void ACLBlasAccelerator::vsLn(int n, float *a, float *y) {
        sopv(LN, n, a, y);
    }

    void ACLBlasAccelerator::vsLog1p(int n, float *a, float *y) {
        sopv(LOG1P, n, a, y);
    }

    void ACLBlasAccelerator::vsTanh(int n, float *a, float *y) {
        sopv(TANH, n, a, y);
    }

    void ACLBlasAccelerator::vsPowx(int n, float *a, float b, float *y) {
        static const int vsPowxNumArgs = 3;

        FloatArrayArg arg_a = FloatArrayArg(a, n, Mode::input);
        FloatArg arg_b = FloatArg(b, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, n, Mode::output);
        KernelArg *args[vsPowxNumArgs] = {&arg_a, &arg_b, &arg_y};

        size_t global_work_size[1] = {n};
        vsPowxKernel->call(1, global_work_size, NULL, args, vsPowxNumArgs);
    }

    void ACLBlasAccelerator::cblas_saxpy(int n, float a, float *x, int incx, float *y, int incy) {
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

        size_t global_work_size[1] = {n};
        saxpyKernel->call(1, global_work_size, NULL, args, saxpyNumArgs);
    }

    void ACLBlasAccelerator::cblas_sscal(int n, float scale, float *x, int incx) {
        static const int sscalNumArgs = 3;
        int xLength = incx * n;
        assert(xLength != 0);

        FloatArg arg_scale = FloatArg(scale, Mode::input);
        FloatArrayArg arg_x = FloatArrayArg(x, xLength, Mode::input_output);
        IntArg arg_incx = IntArg(incx, Mode::input);
        KernelArg *args[sscalNumArgs] = {&arg_scale, &arg_x, &arg_incx};

        size_t global_work_size[1] = {n};
        sscalKernel->call(1, global_work_size, NULL, args, sscalNumArgs);
    }

    float ACLBlasAccelerator::cblas_sdot(int n, float *x, int incx, float *y, int incy) {
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
    ACLBlasAccelerator::cblas_sger(CBLAS_ORDER order,
                                   int m, int n, float alpha,
                                   float *x, int incx,
                                   float *y, int incy,
                                   float *a, int lda) {
        static const int numArgs = 9;
        long xLength = m * incx;
        long yLength = n * incy;
        long aLength = n * lda;
        log("\nm:%d,n:%d,alpha:%0.3f,incx:%d,incy:%d\n", m, n, alpha, incx, incy);
        log("xLength:%d,yLength:%d,aLength:%d\n", xLength, yLength, aLength);
        assert(xLength != 0 && yLength != 0 && aLength != 0);

        /* wrap the raw data to kernelarg objects */
        IntArg arg_m = IntArg(m, Mode::input);
        IntArg arg_n = IntArg(n, Mode::input);
        FloatArg arg_alpha = FloatArg(alpha, Mode::input);
        FloatArrayArg arg_x = FloatArrayArg(x, xLength, Mode::input);
        IntArg arg_incx = IntArg(incx, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, yLength, Mode::input);
        IntArg arg_incy = IntArg(incy, Mode::input);
        FloatArrayArg arg_a = FloatArrayArg(a, aLength, Mode::input_output);
        IntArg arg_lda = IntArg(lda, Mode::input);

        KernelArg *args[numArgs] = {&arg_m, &arg_n,
                                    &arg_alpha,
                                    &arg_x, &arg_incx,
                                    &arg_y, &arg_incy,
                                    &arg_a, &arg_lda};

        /* init the kernel */
        int global_x = xLength / SLICE_SIZE;
        if (xLength - global_x > 0) {
            global_x++;
        }
        int global_y = yLength / SLICE_SIZE;
        if (yLength - global_y > 0) {
            global_y++;
        }

        /* call kernel with inputs and output */
        size_t global_work_size[2] = {global_x * SLICE_SIZE, global_y * SLICE_SIZE};
        size_t local_work_size[2] = {SLICE_SIZE, SLICE_SIZE};
        sgerKernel->call(2, global_work_size, local_work_size, args, numArgs);
    }

    NDRangeKernel *ACLBlasAccelerator::newAxpyKernel() {
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

    NDRangeKernel *ACLBlasAccelerator::newSdotKernel() {
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

    NDRangeKernel *ACLBlasAccelerator::newSopvKernel() {
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

    NDRangeKernel *ACLBlasAccelerator::newSopvvKernel() {
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


    NDRangeKernel *ACLBlasAccelerator::newSscalKernel() {
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

    NDRangeKernel *ACLBlasAccelerator::newVspowxKernel() {
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

    NDRangeKernel *ACLBlasAccelerator::newSgemvKernel() {
        return nullptr;
    }

    NDRangeKernel *ACLBlasAccelerator::newSgerKernel() {
        static const int sgerNumArgs = 9;
        static KernelArgSignature signature[sgerNumArgs] = {
                Sig::primitive<int>(Mode::input),
                Sig::primitive<int>(Mode::input),
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input_output, 0),
                Sig::primitive<int>(Mode::input)
        };

        return new NDRangeKernel(manager->getProgram(),
                                 device,
                                 queue,
                                 "sger",
                                 signature,
                                 sgerNumArgs);
    }

    NDRangeKernel *ACLBlasAccelerator::newSgemmKernel() {
        return nullptr;
    }

}
