//
// Created by pcz on 18-5-17.
//

#include "TypeTagPrimitive.h"

namespace saoclib {
    const std::shared_ptr<TypeTagPrimitive> &TypeTagPrimitive::fromTypeId(NativeTypeID typeID) {
        switch (typeID) {
            case NativeTypeID::c_byte:
                return getByteTypeTag();
            case NativeTypeID::c_short:
                return getShortTypeTag();
            case NativeTypeID::c_int:
                return getIntTypeTag();
            case NativeTypeID::c_long:
                return getLongTypeTag();
            case NativeTypeID::c_float:
                return getFloatTypeTag();
            case NativeTypeID::c_double:
                return getDoubleTypeTag();
            default:
                assert(false && "wrong branch");
        }
    }

    const std::shared_ptr<TypeTagPrimitive> &TypeTagPrimitive::getByteTypeTag() {
        static auto instance = std::make_shared<TypeTagPrimitive>(NativeTypeID::c_byte);
        return instance;
    }

    const std::shared_ptr<TypeTagPrimitive> &TypeTagPrimitive::getShortTypeTag() {
        static auto instance = std::make_shared<TypeTagPrimitive>(NativeTypeID::c_short);
        return instance;
    }

    const std::shared_ptr<TypeTagPrimitive> &TypeTagPrimitive::getIntTypeTag() {
        static auto instance = std::make_shared<TypeTagPrimitive>(NativeTypeID::c_int);
        return instance;
    }

    const std::shared_ptr<TypeTagPrimitive> &TypeTagPrimitive::getLongTypeTag() {
        static auto instance = std::make_shared<TypeTagPrimitive>(NativeTypeID::c_long);
        return instance;
    }

    const std::shared_ptr<TypeTagPrimitive> &TypeTagPrimitive::getFloatTypeTag() {
        static auto instance = std::make_shared<TypeTagPrimitive>(NativeTypeID::c_float);
        return instance;
    }

    const std::shared_ptr<TypeTagPrimitive> &TypeTagPrimitive::getDoubleTypeTag() {
        static auto instance = std::make_shared<TypeTagPrimitive>(NativeTypeID::c_double);
        return instance;
    }

    bool TypeTagPrimitive::isVoid() const {
        return false;
    }

    bool TypeTagPrimitive::isPrimitive() const {
        return true;
    }

    bool TypeTagPrimitive::isArray() const {
        return false;
    }

    NativeTypeID TypeTagPrimitive::getTypeID() const {
        return this->typeID;
    }

    NativeTypeID TypeTagPrimitive::getElemTypeID() const {
        return NativeTypeID::c_void;
    }

    const std::shared_ptr<TypeTag> &TypeTagPrimitive::getElemType() const {
        return TypeTagVoid::getInstance();
    }

    size_t TypeTagPrimitive::getSize() const {
        return primitiveTypeSize(typeID);
    }

    size_t TypeTagPrimitive::getElemSize() const {
        return getSize();
    }

    size_t TypeTagPrimitive::getArrayLength() const {
        return 0;
    }

    std::string TypeTagPrimitive::toString() const {
        return std::string(nativeTypeRepr(typeID));
    }

    TypeTagPrimitive::TypeTagPrimitive(NativeTypeID typeID) : TypeTag(typeID) {}

    bool TypeTagPrimitive::equals(const std::shared_ptr<TypeTag> &rhs) const {
        return rhs->isPrimitive() && typeID == rhs->getTypeID();
    }
}