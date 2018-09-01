//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_KERNELARGMODE_H
#define SAOCLIB_CPP_KERNELARGMODE_H

namespace acl {
    enum class KernelArgMode : long {
        input = 0,
        output = 1,
        input_output = 2
    };
}

#endif //SAOCLIB_CPP_KERNELARGMODE_H
