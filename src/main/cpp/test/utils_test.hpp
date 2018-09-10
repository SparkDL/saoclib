//
// Created by pcz on 9/5/18.
//

#ifndef SAOCLIB_CPP_UTILS_TEST_HPP
#define SAOCLIB_CPP_UTILS_TEST_HPP

#include "gtest/gtest.h"
#include "utils.h"

TEST(utils, matrixCopy) {
    using namespace acl;
    int an = 5;
    int bn = 3;
    int lda = an + 2;
    float *a = new float[lda * an];
    float *b = new float[bn * bn];
    memset(b, 0, bn * bn);
    for (int i = 0; i < lda * an; i++) {
        a[i] = i;
    }

    printMatrix("a", a, lda, an);
    executeTime(
            [=]() -> void {
                matrixCopy(a, an, an, lda, b, bn, bn, bn);
            },
            "matrixCopy",
            100);

    printMatrix("b", b, bn, bn);
}

TEST(utils, matrixTransposeCopy) {
    using namespace acl;
    int a_cols = 4;
    int a_rows = a_cols + 2;

    int b_rows = 6;
    int b_cols = b_rows + 2;

    float *a = new float[a_rows * a_cols];
    float *b = new float[b_rows * b_cols];
    memset(b, 0, sizeof(float) * b_rows * b_cols);

    for (int i = 0; i < a_rows * a_cols; i++) {
        a[i] = i;
    }

    printMatrix("a", a, a_rows, a_cols);
    printMatrix("b-before", b, b_rows, b_cols);
    executeTime(
            [=]() -> void {
                matrixTransposeCopy<float>(a, a_rows - 2, a_cols, a_rows, b, b_rows, b_cols - 2, b_rows);
            },
            "matrixTransposeCopy",
            1);

    printMatrix("b", b, b_rows, b_cols);
}

#endif //SAOCLIB_CPP_UTILS_TEST_HPP
