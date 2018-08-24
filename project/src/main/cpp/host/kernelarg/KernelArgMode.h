//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_KERNELARGMODE_H
#define SAOCLIB_CPP_KERNELARGMODE_H

namespace saoclib {
    enum class KernelArgMode : long {
        mode_input = 0,
        mode_output = 1,
        mode_input_output = 2
    };
}

#endif //SAOCLIB_CPP_KERNELARGMODE_H
