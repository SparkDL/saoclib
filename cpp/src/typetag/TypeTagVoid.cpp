//
// Created by pcz on 18-5-17.
//
#include <string>
#include "TypeTagVoid.h"

namespace saoclib {

    TypeTagVoid *TypeTagVoid::getInstance() {
        static TypeTagVoid instance;
        return &instance;
    }

    bool saoclib::TypeTagVoid::isVoid() const {
        return true;
    }

    bool TypeTagVoid::isPrimitive() const {
        return false;
    }

    bool TypeTagVoid::isArray() const {
        return false;
    }

    NativeTypeID TypeTagVoid::getTypeID() const {
        return NativeTypeID::c_void;
    }

    NativeTypeID TypeTagVoid::getElemTypeID() const {
        return NativeTypeID::c_void;
    }

    const TypeTag *TypeTagVoid::getElemType() const {
        return TypeTagVoid::getInstance();
    }

    size_t TypeTagVoid::getSize() const {
        return primitiveTypeSize(NativeTypeID::c_void);
    }

    size_t TypeTagVoid::getElemSize() const {
        return 0;
    }

    size_t TypeTagVoid::getArrayLength() const {
        return 0;
    }

    TypeTagVoid::TypeTagVoid() : TypeTag(NativeTypeID::c_void) {

    }

    std::string TypeTagVoid::toString() const {
        return "void";
    }

    bool TypeTagVoid::equals(const TypeTag *rhs) const {
        return rhs->isVoid();
    }
}