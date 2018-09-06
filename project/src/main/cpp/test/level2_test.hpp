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
    int m = 5;
    int n = 4;
    auto x = new float[m];
    auto y = new float[n];
    auto a = new float[m * n];
    for (int i = 0; i < m; i++) {
        x[i] = i;
    }
    for (int i = 0; i < n; i++) {
        y[i] = i;
    }
    memset(a, 0, sizeof(float) * m * n);
    int incx = 1;
    int incy = 1;
    float alpha = 1;
    int lda = m;
    testEnv->getAccelerator()->cblas_sger(CblasColMajor, m, n, alpha, x, incx, y, incy, a, lda);

    printMatrix("x", x, m, 1);
    printMatrix("y", y, 1, n);
    printMatrix("a", a, m, n);
    delete[] x;
    delete[] y;
    delete[] a;
    EXPECT_TRUE(true);
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

