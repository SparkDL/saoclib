//
// Created by pcz on 8/27/18.
//

#ifndef SAOCLIB_CPP_ACCELERATOR_H
#define SAOCLIB_CPP_ACCELERATOR_H

#include "AOCLUtils/aocl_utils.h"
#include "CLContext.h"
#include "CLProgram.h"
#include "NDRangeKernel.h"
#include "aclblas.h"

namespace saoclib {
    using namespace aocl_utils;
    using Sig = KernelArgSignature;
    using Mode = KernelArgMode;

    class ACLResourceManager;

    class ACLBlasAccelerator {
    public:
        ACLBlasAccelerator() = default;

        ACLBlasAccelerator(ACLResourceManager *manager, cl_device_id device);

        ~ACLBlasAccelerator();

        void sopvv(int opCode, int n, float *a, float *b, float *y);

        void sopv(int opCode, int n, float *a, float *y);

        void vsAdd(int n, float *a, float *b, float *y);

        void vsSub(int n, float *a, float *b, float *y);

        void vsMul(int n, float *a, float *b, float *y);

        void vsDiv(int n, float *a, float *b, float *y);

        void vsAbs(int n, float *a, float *y);

        void vsSqrt(int n, float *a, float *y);

        void vsExp(int n, float *a, float *y);

        void vsLn(int n, float *a, float *y);

        void vsLog1p(int n, float *a, float *y);

        void vsTanh(int n, float *a, float *y);

        void vsPowx(int n, float *a, float b, float *y);

        void cblas_saxpy(int n, float a, float *x, int incx, float *y, int incy);

        void cblas_sscal(int n, float scale, float *x, int incx);

        float cblas_sdot(int n, float *x, int incx, float *y, int incy);

        void cblas_sger(CBLAS_ORDER order,
                        int m, int n, float alpha,
                        float *x, int incx,
                        float *y, int incy,
                        float *a, int lda);

    private:
        NDRangeKernel *saxpyKernel;
        NDRangeKernel *sdotKernel;
        NDRangeKernel *sopvKernel;
        NDRangeKernel *sopvvKernel;
        NDRangeKernel *sscalKernel;
        NDRangeKernel *vsPowxKernel;
        NDRangeKernel *sgemvKernel;
        NDRangeKernel *sgerKernel;
        NDRangeKernel *sgemmKernel;

        NDRangeKernel *newAxpyKernel();

        NDRangeKernel *newSdotKernel();

        NDRangeKernel *newSopvKernel();

        NDRangeKernel *newSopvvKernel();

        NDRangeKernel *newSscalKernel();

        NDRangeKernel *newVspowxKernel();

        NDRangeKernel *newSgemvKernel();

        NDRangeKernel *newSgerKernel();

        NDRangeKernel *newSgemmKernel();

        ACLResourceManager *manager = NULL;
        cl_device_id device;
        // These resources need to be released:
        cl_command_queue queue = NULL;
    };
}
#endif //SAOCLIB_CPP_ACCELERATOR_H
