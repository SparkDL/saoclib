//
// Created by pcz on 18-5-18.
//

#include "TypeTagArray.h"
#include "TypeTagPrimitive.h"

namespace saoclib {

    std::shared_ptr<TypeTagArray>
    TypeTagArray::fromElemType(const std::shared_ptr<TypeTag> &elemType, size_t arrayLength) {
        return std::make_shared<TypeTagArray>(elemType, arrayLength);
    }

    bool TypeTagArray::equals(const std::shared_ptr<TypeTag> &rhs) const {
        return rhs->isArray()
               && elemType->equals(rhs->getElemType());
    }

    bool TypeTagArray::isVoid() const {
        return false;
    }

    bool TypeTagArray::isPrimitive() const {
        return false;
    }

    bool TypeTagArray::isArray() const {
        return true;
    }

    NativeTypeID TypeTagArray::getTypeID() const {
        return NativeTypeID::c_array;
    }

    NativeTypeID TypeTagArray::getElemTypeID() const {
        return elemType->getTypeID();
    }

    const std::shared_ptr<TypeTag> &TypeTagArray::getElemType() const {
        return elemType;
    }

    size_t TypeTagArray::getSize() const {
        return elemType->getSize() * arrayLength;
    }

    size_t TypeTagArray::getElemSize() const {
        return elemType->getSize();
    }

    size_t TypeTagArray::getArrayLength() const {
        return arrayLength;
    }

    std::string TypeTagArray::toString() const {
        return elemType->toString() + "[" + std::to_string(arrayLength) + "]";
    }

}