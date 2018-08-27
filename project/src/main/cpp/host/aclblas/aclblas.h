//
// Created by pcz on 18-7-24.
//

#ifndef SAOCLIB_CPP_ACLBLAS_H
#define SAOCLIB_CPP_ACLBLAS_H

#ifdef  __cplusplus

#include "saoclib.h"
#include "config.h"


extern "C" {
#endif

namespace aclblas {
    using namespace saoclib;
    using Sig = KernelArgSignature;
    using Mode = KernelArgMode;

    enum CBLAS_ORDER {
        CblasRowMajor = 101, CblasColMajor = 102
    };
    enum CBLAS_TRANSPOSE {
        CblasNoTrans = 111, CblasTrans = 112, CblasConjTrans = 113,
        AtlasConj = 114
    };
    enum CBLAS_UPLO {
        CblasUpper = 121, CblasLower = 122
    };
    enum CBLAS_DIAG {
        CblasNonUnit = 131, CblasUnit = 132
    };
    enum CBLAS_SIDE {
        CblasLeft = 141, CblasRight = 142
    };


    extern CLEnv env;
    extern CLBinary binary;
    extern bool inited;

    bool requestAccelerator();

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
}
}

#ifdef  __cplusplus
#endif

#endif //SAOCLIB_CPP_ACLBLAS_H
