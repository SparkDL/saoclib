//
// Created by pcz on 18-5-17.
//

#include "KernelArgLimit.h"

namespace saoclib {

    KernelArgLimit::KernelArgLimit(const std::shared_ptr<TypeTag> &argTypeTag, KernelArgMode mode)
            : argTypeTag(argTypeTag), mode(mode) {}

    bool KernelArgLimit::operator==(const KernelArgLimit &rhs) const {
        return mode == rhs.getMode() && argTypeTag->equals(rhs.getTypeTag());
    }

    KernelArgMode KernelArgLimit::getMode() const {
        return mode;
    }

    const std::shared_ptr<TypeTag> &KernelArgLimit::getTypeTag() const {
        return argTypeTag;
    }

    bool KernelArgLimit::isVoid() const {
        return argTypeTag->isVoid();
    }

    bool KernelArgLimit::isPrimitive() const {
        return argTypeTag->isPrimitive();
    }

    bool KernelArgLimit::isArray() const {
        return argTypeTag->isArray();
    }

    NativeTypeID KernelArgLimit::getTypeID() const {
        return argTypeTag->getTypeID();
    }

    NativeTypeID KernelArgLimit::getElemTypeID() const {
        return argTypeTag->getElemTypeID();
    }

    const std::shared_ptr<TypeTag> &KernelArgLimit::getElemType() const {
        return argTypeTag->getElemType();
    }

    size_t KernelArgLimit::getSize() const {
        return argTypeTag->getSize();
    }

    size_t KernelArgLimit::getElemSize() const {
        return argTypeTag->getElemSize();
    }

    size_t KernelArgLimit::getArrayLength() const {
        return argTypeTag->getArrayLength();
    }
}