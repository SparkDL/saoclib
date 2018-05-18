//
// Created by pcz on 18-5-17.
//

#include "TypeTagPrimitive.h"

namespace saoclib {
    const TypeTagPrimitive *TypeTagPrimitive::getByteTypeTag() {
        static TypeTagPrimitive byteTypeTag(NativeTypeID::c_byte);
        return &byteTypeTag;
    }

    const TypeTagPrimitive *TypeTagPrimitive::getShortTypeTag() {
        static TypeTagPrimitive shortTypeTag(NativeTypeID::c_short);
        return &shortTypeTag;
    }

    const TypeTagPrimitive *TypeTagPrimitive::getIntTypeTag() {
        static TypeTagPrimitive intTypeTag(NativeTypeID::c_int);
        return &intTypeTag;
    }

    const TypeTagPrimitive *TypeTagPrimitive::getLongTypeTag() {
        static TypeTagPrimitive longTypeTag(NativeTypeID::c_long);
        return &longTypeTag;
    }

    const TypeTagPrimitive *TypeTagPrimitive::getFloatTypeTag() {
        static TypeTagPrimitive floatTypeTag(NativeTypeID::c_float);
        return &floatTypeTag;
    }

    const TypeTagPrimitive *TypeTagPrimitive::getDoubleTypeTag() {
        static TypeTagPrimitive doubleTypeTag(NativeTypeID::c_double);
        return &doubleTypeTag;
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

    const TypeTag *TypeTagPrimitive::getElemType() const {
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
        return nativeTypeRepr(typeID);
    }

    TypeTagPrimitive::TypeTagPrimitive(NativeTypeID typeID) : TypeTag(typeID) {}

    bool TypeTagPrimitive::equals(const saoclib::TypeTag *rhs) const {
        return rhs->isPrimitive() && typeID == rhs->getTypeID();
    }

    template<>
    const TypeTagPrimitive *TypeTagPrimitive::getTypeTag<byte>() {
        return getByteTypeTag();
    }

    template<>
    const TypeTagPrimitive *TypeTagPrimitive::getTypeTag<short>() {
        return getShortTypeTag();
    }

    template<>
    const TypeTagPrimitive *TypeTagPrimitive::getTypeTag<int>() {
        return getIntTypeTag();
    }

    template<>
    const TypeTagPrimitive *TypeTagPrimitive::getTypeTag<long>() {
        return getLongTypeTag();
    }

    template<>
    const TypeTagPrimitive *TypeTagPrimitive::getTypeTag<float>() {
        return getFloatTypeTag();
    }

    template<>
    const TypeTagPrimitive *TypeTagPrimitive::getTypeTag<double>() {
        return getDoubleTypeTag();
    }
}