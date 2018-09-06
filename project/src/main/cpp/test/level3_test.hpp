//
// Created by pcz on 9/2/18.
//

#ifndef SAOCLIB_CPP_LEVEL3_TEST_HPP
#define SAOCLIB_CPP_LEVEL3_TEST_HPP

#include <iostream>
#include <cmath>
#include <gtest/gtest.h>
#include "acl.h"
#include "utils.h"
#include "config.h"
#include "BlasTestEnvironment.h"

extern BlasTestEnvironment *const testEnv;

TEST(level3, sgemm1) {
    using namespace acl;
    const float f = 10;
    auto transa = CblasNoTrans;
    auto transb = CblasNoTrans;
    int M = 5;
    int N = 5;
    int K = 5;

    printf("M:%d,N:%d,K:%d", M, N, K);
    float alpha = 1;
    float beta = 1;
    int lda = M;
    int ldb = K;
    int ldc = M;

    auto a = new float[lda * K];
    auto b = new float[ldb * N];
    auto c = new float[ldc * N];

    for (int i = 0; i < lda * K; i++) {
        a[i] = i;
    }
    for (int i = 0; i < ldb * N; i++) {
        b[i] = i;
    }
    for (int i = 0; i < ldc * N; i++) {
        c[i] = 0;
    }

    printMatrix("a", a, M, K);
    printMatrix("b", b, K, N);
    printMatrix("c", c, M, N);
    double time = executeTime(
            [=]() -> void {
                testEnv->getAccelerator()
                        ->cblas_sgemm(CblasColMajor,
                                      transa, transb,
                                      M, N, K,
                                      alpha,
                                      a, lda,
                                      b, ldb,
                                      beta,
                                      c, ldc);
            },
            "sgemm"
    );

    double flops = 4.0 * M * N * K / time;
    printf("\nSGEMM Throughput: %0.5f GFLOPS\n\n", flops * 1e-9);

    printMatrix("c", c, M, N);


    delete[] a;
    delete[] b;
    delete[] c;
    ASSERT_TRUE(true);
}

TEST(level3, sgemm2) {
    using namespace acl;
    const float f = 10;
    auto transa = CblasNoTrans;
    auto transb = CblasNoTrans;
    int M = 5;
    int N = 4;
    int K = 1;

    printf("M:%d,N:%d,K:%d", M, N, K);
    float alpha = 1;
    float beta = 1;
    int lda = M;
    int ldb = K;
    int ldc = M;

    auto a = new float[lda * K];
    auto b = new float[ldb * N];
    auto c = new float[ldc * N];

    for (int i = 0; i < lda * K; i++) {
        a[i] = i;
    }
    for (int i = 0; i < ldb * N; i++) {
        b[i] = i;
    }
    for (int i = 0; i < ldc * N; i++) {
        c[i] = 0;
    }

    printMatrix("a", a, M, K);
    printMatrix("b", b, K, N);
    printMatrix("c", c, M, N);
    double time = executeTime(
            [=]() -> void {
                testEnv->getAccelerator()
                        ->cblas_sgemm(CblasColMajor,
                                      transa, transb,
                                      M, N, K,
                                      alpha,
                                      a, lda,
                                      b, ldb,
                                      beta,
                                      c, ldc);
            },
            "sgemm"
    );

    double flops = 4.0 * M * N * K / time;
    printf("\nSGEMM Throughput: %0.5f GFLOPS\n\n", flops * 1e-9);

    printMatrix("c",c, M, N);


    delete[] a;
    delete[] b;
    delete[] c;
    ASSERT_TRUE(true);
}

#endif //SAOCLIB_CPP_LEVEL3_TEST_HPP
