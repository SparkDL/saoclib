//
// Created by pcz on 18-5-17.
//
#include "KernelArg.h"

namespace saoclib {

    KernelArg::KernelArg(KernelArgLimit limit) : limit(limit) {}

    KernelArgMode KernelArg::getMode() const {
        return limit.getMode();
    }

    const std::shared_ptr<TypeTag> & KernelArg::getTypeTag() const {
        return limit.getTypeTag();
    }

    bool KernelArg::isVoid() const {
        return limit.isVoid();
    }

    bool KernelArg::isPrimitive() const {
        return limit.isPrimitive();
    }

    bool KernelArg::isArray() const {
        return limit.isArray();
    }

    NativeTypeID KernelArg::getTypeID() const {
        return limit.getTypeID();
    }

    NativeTypeID KernelArg::getElemTypeID() const {
        return limit.getElemTypeID();
    }

    const std::shared_ptr<TypeTag> & KernelArg::getElemType() const {
        return limit.getElemType();
    }

    size_t KernelArg::getSize() const {
        return limit.getSize();
    }

    size_t KernelArg::getElemSize() const {
        return limit.getElemSize();
    }

    size_t KernelArg::getArrayLength() const {
        return limit.getArrayLength();
    }

}