#include "gtest/gtest.h"
#include "aclblas.h"
#include <cmath>
#include <random>

static const int n = 1000;
static const float abs_error = 0.0001f;
extern BlasTestEnvironment *const testEnv;

TEST(level1, vsAdd) {
    float *a = new float[n];
    float *b = new float[n];
    float *c = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random();
        b[i] = random();
    }
    testEnv->getAccelerator()->vsAdd(n, a, b, c);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(a[i] + b[i], c[i]);
    }

    delete[] a;
    delete[] b;
    delete[] c;
}

TEST(level1, vsSub) {
    float *a = new float[n];
    float *b = new float[n];
    float *c = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random();
        b[i] = random();
    }
    testEnv->getAccelerator()->vsSub(n, a, b, c);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(a[i] - b[i], c[i]);
    }

    delete[] a;
    delete[] b;
    delete[] c;
}

TEST(level1, vsMul) {
    float *a = new float[n];
    float *b = new float[n];
    float *c = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random();
        b[i] = random();
    }
    testEnv->getAccelerator()->vsMul(n, a, b, c);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(a[i] * b[i], c[i]);
    }

    delete[] a;
    delete[] b;
    delete[] c;
}

TEST(level1, vsDiv) {
    float *a = new float[n];
    float *b = new float[n];
    float *c = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random();
        b[i] = random();
    }
    testEnv->getAccelerator()->vsDiv(n, a, b, c);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(a[i] / b[i], c[i]);
    }

    delete[] a;
    delete[] b;
    delete[] c;
}

TEST(level1, vsSqrt) {
    float *a = new float[n];
    float *y = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random();
    }
    testEnv->getAccelerator()->vsSqrt(n, a, y);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(std::sqrt(a[i]), y[i]);
    }

    delete[] a;
    delete[] y;
}

TEST(level1, vsExp) {
    float *a = new float[n];
    float *y = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random() + 0.1f;
    }
    testEnv->getAccelerator()->vsExp(n, a, y);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(std::exp(a[i]), y[i]);
    }

    delete[] a;
    delete[] y;
}

TEST(level1, vsLn) {
    float *a = new float[n];
    float *y = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random() + 0.1f;
    }
    testEnv->getAccelerator()->vsLn(n, a, y);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(std::log(a[i]), y[i]);
    }

    delete[] a;
    delete[] y;
}

TEST(level1, vsLog1p) {
    float *a = new float[n];
    float *y = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random() + 0.1f;
    }
    testEnv->getAccelerator()->vsLog1p(n, a, y);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(std::log1p(a[i]), y[i]);
    }

    delete[] a;
    delete[] y;
}

TEST(level1, vsTanh) {
    float *a = new float[n];
    float *y = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random() + 0.1f;
    }
    testEnv->getAccelerator()->vsTanh(n, a, y);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(std::tanh(a[i]), y[i]);
    }

    delete[] a;
    delete[] y;
}

TEST(level1, vsPowx) {
    float *a = new float[n];
    float b = random();
    float *y = new float[n];

    for (int i = 0; i < n; i++) {
        a[i] = random() + 0.1f;
    }
    testEnv->getAccelerator()->vsPowx(n, a, b, y);
    for (int i = 0; i < n; i++) {
        ASSERT_FLOAT_EQ(std::pow(a[i], b), y[i]);
    }

    delete[] a;
    delete[] y;
}

TEST(level1, saxpy) {
    for (int k = 0; k < 100; k++) {
        float a = random();
        int incx = rand() % 10 + 1;
        int incy = rand() % 10 + 1;

        float *x = new float[n * incx];
        float *y_expect = new float[n * incy];
        float *y_real = new float[n * incy];

        for (int i = 0; i < n; i++) {
            int xi = i * incx;
            int yi = i * incy;
            x[xi] = (random() % n) / (float) n;
            y_expect[yi] = (random() % n) / (float) n;
            y_real[yi] = y_expect[yi];
        }

        for (int i = 0; i < n; i++) {
            int xi = incx * i;
            int yi = incy * i;
            y_expect[yi] += a * x[xi];
        }

        testEnv->getAccelerator()->cblas_saxpy(n, a, x, incx, y_real, incy);

        for (int i = 0; i < n; i++) {
            int yi = incy * i;
            ASSERT_FLOAT_EQ(y_expect[yi], y_real[yi]);
        }

        delete[] x;
        delete[] y_expect;
        delete[] y_real;
    }
}

TEST(level1, sdot) {
    for (int k = 0; k < 5; k++) {
        int incx = rand() % 10 + 1;
        int incy = rand() % 10 + 1;

        float *x = new float[n * incx];
        float *y = new float[n * incy];

        for (int i = 0; i < n; i++) {
            x[i] = (random() % n) / (float) n + 0.001f;
            y[i] = (random() % n) / (float) n + 0.001f;
        }

        float expect = 0;
        float expect1 = 0;
        for (int i = 0; i < n; i++) {
            int xi = i * incx;
            int yi = i * incy;
            expect += x[xi] * y[yi];
        }
        for (int i = 0, xi = 0, yi = 0; i < n; i++, xi += incx, yi += incy) {
            expect1 += x[xi] * y[yi];
        }

        float result = testEnv->getAccelerator()->cblas_sdot(n, x, incx, y, incy);

        if (result - expect > abs_error) {
            printf("n:%d,incx:%d,incy:%d", n, incx, incy);
        }
        ASSERT_FLOAT_EQ(expect, result);

        delete[] x;
        delete[] y;
    }
}

TEST(level1, sscal) {
    float scale = random() + 0.1f;
    int incx = rand() % 10 + 1;
    float *x_expect = new float[n * incx];
    float *x = new float[n * incx];

    for (int i = 0; i < n * incx; i++) {
        x[i] = random() + 0.1f;
        x_expect[i] = scale * x[i];
    }
    testEnv->getAccelerator()->cblas_sscal(n, scale, x, incx);
    for (int i = 0; i < n; i++) {
        int xi = i * incx;
        if (x_expect[xi] != x[xi]) {
            std::cout << "";
        }
        ASSERT_FLOAT_EQ(x_expect[xi], x[xi]);
    }

    delete[] x;
    delete[] x_expect;
}