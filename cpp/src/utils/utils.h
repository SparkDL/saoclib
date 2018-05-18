//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_UTILS_H
#define SAOCLIB_CPP_UTILS_H

#include <cstddef>
#include <cassert>
#include "NativeTypeID.h"
#include "KernelArgMode.h"

namespace saoclib {
    size_t primitiveTypeSize(NativeTypeID typeID);

    const char *nativeTypeRepr(NativeTypeID typeID);

    const char *argModeRepr(KernelArgMode mode);
}
#endif //SAOCLIB_CPP_UTILS_H
