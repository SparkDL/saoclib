//
// Created by pcz on 8/25/18.
//

#ifndef SAOCLIB_CPP_LEVEL2_TEST_HPP
#define SAOCLIB_CPP_LEVEL2_TEST_HPP

#endif //SAOCLIB_CPP_LEVEL2_TEST_HPP

#include <iostream>
#include <gtest/gtest.h>
#include "aclmkl.h"

extern BlasTestEnvironment *const testEnv;

TEST(level2, sger) {
    for (int i = 0; i < 10; i++) {
        float x[] = {1, 2, 3, 4, 5, 6, 7, 8};
        float y[] = {1, 1, 10, 10, 100, 100, 1000, 1000, 10000, 10000};
        float a[100] = {0,};

        int incx = 1;
        int incy = 1;
        int m = 8;
        int n = 10;
        float alpha = 1.0;
        int lda = 10;
        testEnv->getAccelerator()->cblas_sger(CblasColMajor, m, n, alpha, x, incx, y, incy, a, lda);
        int total = lda * n;
        for (int i = 0; i < total; i++) {
            if (i % n == 0) {
                std::cout << std::endl;
            }
            std::cout << a[i] << ",0";
        }
        std::cout << std::endl;

        EXPECT_EQ(1, 1);
    }
}

TEST(level2, sgemv) {
    using namespace acl;

    int m = 5;
    int n = 5;
    float alpha = 1;
    float beta = 1;
    int lda = m;

    auto a = new float[lda * n];
    auto x = new float[n];
    auto y = new float[m];
    for (int i = 0; i < lda * n; i++) {
        a[i] = i;
        std::cout << a[i] << ",";
    }
    std::cout << std::endl;
    for (int i = 0; i < n; i++) {
        x[i] = i;
        std::cout << a[i] << ",";
    }
    std::cout << std::endl;
    for (int i = 0; i < m; i++) {
        y[i] = 0;
        std::cout << y[i] << ",";
    }
    std::cout << std::endl;

    double time = executeTime(
            [=]() -> void {
                testEnv->getAccelerator()
                        ->cblas_sgemv(CblasColMajor,
                                      CblasNoTrans,
                                      m, n,
                                      alpha,
                                      a, lda,
                                      x, 1,
                                      beta,
                                      y, 1);
            },
            "sgemv"
    );

    for (int i = 0; i < m; i++) {
        std::cout << y[i] << ",";
    }
    std::cout << std::endl;

    ASSERT_TRUE(true);
}

