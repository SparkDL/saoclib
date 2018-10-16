//
// Created by pcz on 10/15/18.
//

#include "KernelArg.h"
#include <vector>
#include <acl/KernelArgMode.h>

// Entry point.
int main(int argc, char **argv) {
    using namespace acl;
    using Mode = KernelArgMode;
    int m = 150, n = 64;
    float *a = new float[m * n];
    int lda = n                                                                                                                                                                                                                                                                                                                                                          ;
    auto a_arg = Matrix<float>(a, lda, m, n, true, Mode::input);

    a_arg.resizeContainer();
    a_arg.writeContainer();

    delete[] a;
}
