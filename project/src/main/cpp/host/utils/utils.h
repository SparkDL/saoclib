//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_UTILS_H
#define SAOCLIB_CPP_UTILS_H

#include <cstddef>
#include <cassert>
#include <functional>
#include "NativeTypeID.h"
#include "KernelArgMode.h"
#include "aclmkl.h"

namespace acl {
    void log(const std::string &format, ...);

    std::string getThreadString();

    double executeTime(const std::function<void()> &function, const char *name);

    size_t primitiveTypeSize(NativeTypeID typeID);

    const char *nativeTypeRepr(NativeTypeID typeID);

    const char *argModeRepr(KernelArgMode mode);

    bool isInput(KernelArgMode mode);

    bool isOutput(KernelArgMode mode);
    
    bool isTrans(CBLAS_TRANSPOSE trans);
}
#endif //SAOCLIB_CPP_UTILS_H
