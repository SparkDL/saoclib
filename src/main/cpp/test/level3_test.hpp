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

float l2error(float *a, float *ref_output, int n) {
    float diff = 0.0f;
    float ref = 0.0f;
    for (int i = 0; i < n; i++) {
        float r = ref_output[i];
        float o = a[i];
        float d = o - r;
        diff += d * d;
        ref += r * r;
    }
    const float diff_l2norm = sqrtf(diff);
    const float ref_l2norm = sqrtf(ref);
    const float error = diff_l2norm / ref_l2norm;
    return error;
}

float rand_float() {
    return float(rand()) / float(RAND_MAX) * 20.0f - 10.0f;
}

void mm(float *a, float *b, float *c, int m, int n, int k) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0;
            for (int e = 0; e < k; e++) {
                sum += a[pos(i, e, m)] * b[pos(e, j, k)];
            }
            c[pos(i, j, m)] = sum;
        }
    }
}

TEST(level3, sgemm1) {
    using namespace acl;
    auto transa = CblasNoTrans;
    auto transb = CblasNoTrans;
    int M = 512;
    int N = 512;
    int K = 512;

    printf("M:%d,N:%d,K:%d\n", M, N, K);
    float alpha = 1;
    float beta = 0;
    int lda = M;
    int ldb = K;
    int ldc = M;

    auto a = new float[lda * K];
    auto b = new float[ldb * N];
    auto c = new float[ldc * N];
    auto d = new float[ldc * N];

    for (int i = 0; i < lda * K; i++) {
        a[i] = rand_float();
    }
    for (int i = 0; i < ldb * N; i++) {
        b[i] = rand_float();
    }
    for (int i = 0; i < ldc * N; i++) {
        c[i] = rand_float();
        d[i] = rand_float();
    }

    //printMatrix("a", a, M, K);
    //printMatrix("b", b, K, N);
    //printMatrix("c", c, M, N);

    testEnv->getAccelerator()
            ->cblas_sgemm(CblasColMajor,
                          transa, transb,
                          M, N, K,
                          alpha,
                          a, lda,
                          b, ldb,
                          beta,
                          c, ldc);

    mm(a, b, d, M, N, K);

    float e = l2error(c, d, ldc * N);
    ASSERT_LE(e, 1e-6);

    delete[] a;
    delete[] b;
    delete[] c;
    delete[] d;
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

    printMatrix("c", c, M, N);


    delete[] a;
    delete[] b;
    delete[] c;
    ASSERT_TRUE(true);
}

#endif //SAOCLIB_CPP_LEVEL3_TEST_HPP
