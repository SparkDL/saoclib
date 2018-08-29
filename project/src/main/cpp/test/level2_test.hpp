//
// Created by pcz on 8/25/18.
//

#ifndef SAOCLIB_CPP_LEVEL2_TEST_HPP
#define SAOCLIB_CPP_LEVEL2_TEST_HPP

#endif //SAOCLIB_CPP_LEVEL2_TEST_HPP

#include <iostream>
#include <gtest/gtest.h>

#include "aclblas.h"


TEST(level2, sger) {
    for (int i = 0; i < 1000; i++) {
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
