//
// Created by pcz on 8/27/18.
//

#ifndef SAOCLIB_CPP_ACLMKLACCELERATOR_H
#define SAOCLIB_CPP_ACLMKLACCELERATOR_H

#include "AOCLUtils/aocl_utils.h"
#include "Context.h"
#include "Program.h"
#include "NDRangeKernel.h"
#include "aclmkl.h"

namespace acl {
    using namespace aocl_utils;
    using Sig = KernelArgSignature;
    using Mode = KernelArgMode;

    class ACLResourceManager;

    class ACLMKLAccelerator {
    public:
        ACLMKLAccelerator() = default;

        ACLMKLAccelerator(ACLResourceManager *manager, cl_device_id device);

        ~ACLMKLAccelerator();

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

        void cblas_sgemv(CBLAS_ORDER order,
                         CBLAS_TRANSPOSE trans,
                         int m, int n,
                         float alpha,
                         float *a, int lda,
                         float *x, int incx,
                         float beta,
                         float *y, int incy);

        void cblas_sgemm(CBLAS_ORDER order,
                         CBLAS_TRANSPOSE transa, CBLAS_TRANSPOSE transb,
                         int M, int N, int K,
                         float alpha,
                         float *a, int lda,
                         float *b, int ldb,
                         float beta,
                         float *c, int ldc);

    private:
        NDRangeKernel *saxpyKernel;
        NDRangeKernel *sdotKernel;
        NDRangeKernel *sopvKernel;
        NDRangeKernel *sopvvKernel;
        NDRangeKernel *sscalKernel;
        NDRangeKernel *vsPowxKernel;
        NDRangeKernel *sgemmKernel;

        NDRangeKernel *newAxpyKernel();

        NDRangeKernel *newSdotKernel();

        NDRangeKernel *newSopvKernel();

        NDRangeKernel *newSopvvKernel();

        NDRangeKernel *newSscalKernel();

        NDRangeKernel *newVspowxKernel();

        NDRangeKernel *newSgemmKernel();

        ACLResourceManager *manager = NULL;
        cl_device_id device;
        // These resources need to be released:
        cl_command_queue queue = NULL;
    };
}
#endif //SAOCLIB_CPP_ACLMKLACCELERATOR_H
