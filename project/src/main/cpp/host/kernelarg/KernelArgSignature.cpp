//
// Created by pcz on 18-5-17.
//

#include "KernelArgSignature.h"

namespace saoclib {
    bool KernelArgSignature::operator==(const KernelArgSignature &rhs) const {
        return mode == rhs.mode
               && typeID == rhs.typeID
               && isArray_ == rhs.isArray_;
        // && arrayLength == rhs.arrayLength;
    }

    bool KernelArgSignature::isOutput() const {
        switch (mode) {
            case KernelArgMode::input:
                return false;
            case KernelArgMode::output:
            case KernelArgMode::input_output:
                return true;
        }
    }

    bool KernelArgSignature::isInput() const {
        switch (mode) {
            case KernelArgMode::output:
                return false;
            case KernelArgMode::input:
            case KernelArgMode::input_output:
                return true;
        }
    }

    bool KernelArgSignature::isPrimitive() const {
        return !isArray_;
    }

    bool KernelArgSignature::isArray() const {
        return isArray_;
    }

    KernelArgMode KernelArgSignature::getMode() const {
        return mode;
    }

    NativeTypeID KernelArgSignature::getTypeID() const {
        return typeID;
    }

    NativeTypeID KernelArgSignature::getElemTypeID() const {
        if (isArray_) {
            return typeID;
        }
        assert(false && "wrong branch");
    }

    size_t KernelArgSignature::getSize() const {
        if (isArray_) {
            return getElemSize() * arrayLength;
        }
        return primitiveSize(typeID);
    }

    size_t KernelArgSignature::getElemSize() const {
        return primitiveSize(typeID);
    }

    size_t KernelArgSignature::getArrayLength() const {
        return arrayLength;
    }

    std::string KernelArgSignature::toString() const {
        if (isArray_) {
            return std::string("<") + nativeTypeRepr(typeID)
                   + "," + argModeRepr(getMode()) + ">"
                   + "[" + std::to_string(arrayLength) + "]";
        }
        return std::string("<") + nativeTypeRepr(typeID) + "," + argModeRepr(getMode()) + ">";
    }

    KernelArgSignature::KernelArgSignature(KernelArgMode mode,
                                           NativeTypeID typeID,
                                           bool isArray_,
                                           size_t arrayLength)
            : mode(mode),
              typeID(typeID),
              isArray_(isArray_),
              arrayLength(arrayLength) {}

    size_t KernelArgSignature::primitiveSize(NativeTypeID id) {
        switch (id) {
            case NativeTypeID::c_byte:
                return sizeof(byte);
            case NativeTypeID::c_short:
                return sizeof(short);
            case NativeTypeID::c_int:
                return sizeof(int);
            case NativeTypeID::c_long:
                return sizeof(long);
            case NativeTypeID::c_float:
                return sizeof(float);
            case NativeTypeID::c_double:
                return sizeof(double);
            case NativeTypeID::c_array:
                assert(false && "wrong branch");
        }
        return 0;
    }
}