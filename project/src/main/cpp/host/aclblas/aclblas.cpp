//
// Created by pcz on 18-7-24.
//

#include "aclblas.h"
#include <thread>
#include <mutex>

namespace aclblas {
    static const char *binaryPath = "/home/pcz/develop/saoclib/project/target/aclblas";
    CLEnv env;
    CLBinary binary(&env, binaryPath);
    bool inited = false;

    class Accelerator {
    public:

        ~Accelerator() {
            delete saxpyKernel;
            delete sdotKernel;
            delete sopvKernel;
            delete sopvvKernel;
            delete sscalKernel;
            delete vsPowxKernel;
            delete sgemvKernel;
            delete sgerKernel;
            delete sgemmKernel;
        }

        bool init(cl_device_id device, std::string &msg) {
            return true;
        }

    private:
        CLEnv *env = NULL;
        CLBinary *binary = NULL;
        cl_device_id device;
        NDRangeKernel *saxpyKernel = NULL;
        NDRangeKernel *sdotKernel = NULL;
        NDRangeKernel *sopvKernel = NULL;
        NDRangeKernel *sopvvKernel = NULL;
        NDRangeKernel *sscalKernel = NULL;
        NDRangeKernel *vsPowxKernel = NULL;
        NDRangeKernel *sgemvKernel = NULL;
        NDRangeKernel *sgerKernel = NULL;
        NDRangeKernel *sgemmKernel = NULL;

    private:
        void initOpvv() {
            const int sopvvNumArgs = 4;
            KernelArgSignature signature[] = {
                    Sig::primitive<int>(Mode::input),
                    Sig::array<float>(Mode::input, 0),
                    Sig::array<float>(Mode::input, 0),
                    Sig::array<float>(Mode::output, 0)
            };

            sopvvKernel = new NDRangeKernel(binary,
                                            device,
                                            "sopvv",
                                            signature,
                                            sopvvNumArgs);
        }
    };

    static std::map<std::thread::id, Accelerator> accelerators;
    std::mutex accLock;

    bool requestAccelerator() {
//        auto tid = std::thread::id();
//        if (accelerators.find(tid) != accelerators.end()) {
//
//        }

        if (!inited) {
            env.initOpenCL();
            auto device = env.getDeviceID(0);
            binary.deploy(&device, 1);
            inited = true;
        }
    }

    void sopvv(int opCode, int n, float *a, float *b, float *y) {
        static const int sopvvNumArgs = 4;
        static KernelArgSignature signature[] = {
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::array<float>(Mode::input, 0),
                Sig::array<float>(Mode::output, 0)
        };

        static NDRangeKernel sopvvKernel(&binary,
                                         binary.getDeployedDevice(0),
                                         "sopvv",
                                         signature,
                                         sopvvNumArgs);

        IntArg arg_opCode = IntArg(opCode, Mode::input);
        FloatArrayArg arg_a = FloatArrayArg(a, n, Mode::input);
        FloatArrayArg arg_b = FloatArrayArg(b, n, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, n, Mode::output);
        KernelArg *args[] = {&arg_opCode, &arg_a, &arg_b, &arg_y};

        size_t global_work_size[1] = {n};
        sopvvKernel.call(1, global_work_size, NULL, args, sopvvNumArgs);
    }

    void sopv(int opCode, int n, float *a, float *y) {
        static const int sopvNumArgs = 3;

        static KernelArgSignature sopvSignature[sopvNumArgs] = {
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::array<float>(Mode::output, 0)
        };

        static NDRangeKernel sopvKernel(&binary,
                                        binary.getDeployedDevice(0),
                                        "sopv",
                                        sopvSignature,
                                        sopvNumArgs);

        IntArg arg_opCode = IntArg(opCode, Mode::input);
        FloatArrayArg arg_a = FloatArrayArg(a, n, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, n, Mode::output);
        KernelArg *args[] = {&arg_opCode, &arg_a, &arg_y};

        size_t global_work_size[1] = {n};
        sopvKernel.call(1, global_work_size, NULL, args, sopvNumArgs);
    }

    void vsAdd(int n, float *a, float *b, float *y) {
        sopvv(ADD, n, a, b, y);
    }

    void vsSub(int n, float *a, float *b, float *y) {
        sopvv(SUB, n, a, b, y);
    }

    void vsMul(int n, float *a, float *b, float *y) {
        sopvv(MUL, n, a, b, y);
    }

    void vsDiv(int n, float *a, float *b, float *y) {
        sopvv(DIV, n, a, b, y);
    }

    void vsAbs(int n, float *a, float *y) {
        sopv(ABS, n, a, y);
    }

    void vsSqrt(int n, float *a, float *y) {
        sopv(SQRT, n, a, y);
    }

    void vsExp(int n, float *a, float *y) {
        sopv(EXP, n, a, y);
    }

    void vsLn(int n, float *a, float *y) {
        sopv(LN, n, a, y);
    }

    void vsLog1p(int n, float *a, float *y) {
        sopv(LOG1P, n, a, y);
    }

    void vsTanh(int n, float *a, float *y) {
        sopv(TANH, n, a, y);
    }

    void vsPowx(int n, float *a, float b, float *y) {
        static const int vsPowxNumArgs = 3;

        static KernelArgSignature vsPowxSignature[vsPowxNumArgs] = {
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::output, 0)
        };

        static NDRangeKernel sopvKernel(&binary,
                                        binary.getDeployedDevice(0),
                                        "vsPowx",
                                        vsPowxSignature,
                                        vsPowxNumArgs);


        FloatArrayArg arg_a = FloatArrayArg(a, n, Mode::input);
        FloatArg arg_b = FloatArg(b, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, n, Mode::output);
        KernelArg *args[vsPowxNumArgs] = {&arg_a, &arg_b, &arg_y};

        size_t global_work_size[1] = {n};
        sopvKernel.call(1, global_work_size, NULL, args, vsPowxNumArgs);
    }

    void cblas_saxpy(int n, float a, float *x, int incx, float *y, int incy) {
        static const int sdotNumArgs = 5;

        static KernelArgSignature saxpySignature[sdotNumArgs] = {
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input_output, 0),
                Sig::primitive<int>(Mode::input),
        };

        static NDRangeKernel saxpyKernel(&binary,
                                         binary.getDeployedDevice(0),
                                         "saxpy",
                                         saxpySignature,
                                         sdotNumArgs);

        int xLength = incx * n;
        int yLength = incy * n;

        assert(xLength != 0 && yLength != 0);
        FloatArg arg_a = FloatArg(a, Mode::input);
        FloatArrayArg arg_x = FloatArrayArg(x, xLength, Mode::input);
        IntArg arg_incx = IntArg(incx, Mode::input);
        FloatArrayArg arg_y = FloatArrayArg(y, yLength, Mode::input_output);
        IntArg arg_incy = IntArg(incy, Mode::input);
        KernelArg *args[sdotNumArgs] = {&arg_a, &arg_x, &arg_incx, &arg_y, &arg_incy};

        size_t global_work_size[1] = {n};
        saxpyKernel.call(1, global_work_size, NULL, args, sdotNumArgs);
    }

    void cblas_sscal(int n, float scale, float *x, int incx) {

        static const int sscalNumArgs = 3;

        static KernelArgSignature sscalSignature[sscalNumArgs] = {
                Sig::primitive<float>(Mode::input),
                Sig::array<float>(Mode::input_output, 0),
                Sig::primitive<int>(Mode::input),
        };

        static NDRangeKernel sscalKernel(&binary,
                                         binary.getDeployedDevice(0),
                                         "sscal",
                                         sscalSignature,
                                         sscalNumArgs);

        int xLength = incx * n;
        assert(xLength != 0);

        FloatArg arg_scale = FloatArg(scale, Mode::input);
        FloatArrayArg arg_x = FloatArrayArg(x, xLength, Mode::input_output);
        IntArg arg_incx = IntArg(incx, Mode::input);
        KernelArg *args[sscalNumArgs] = {&arg_scale, &arg_x, &arg_incx};

        size_t global_work_size[1] = {n};
        sscalKernel.call(1, global_work_size, NULL, args, sscalNumArgs);
    }

    float cblas_sdot(int n, float *x, int incx, float *y, int incy) {
        static const int sdotNumArgs = 6;

        static KernelArgSignature sdotSignature[sdotNumArgs] = {
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::input, 0),
                Sig::primitive<int>(Mode::input),
                Sig::array<float>(Mode::output, 0)
        };

        static NDRangeKernel sdotKernel(&binary,
                                        binary.getDeployedDevice(0),
                                        "sdot",
                                        sdotSignature,
                                        sdotNumArgs);

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
        sdotKernel.call(1, global_work_size, NULL, args, sdotNumArgs);
        return result;
    }

    void cblas_sger(CBLAS_ORDER order,
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

        static KernelArgSignature signature[numArgs] = {
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

        static NDRangeKernel kernel(&binary,
                                    binary.getDeployedDevice(0),
                                    "sger",
                                    signature,
                                    numArgs);

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
        kernel.call(2, global_work_size, local_work_size, args, numArgs);
    }
}

void cleanup() {
    aclblas::binary.cleanup();
    aclblas::env.cleanup();
}