//
// Created by pcz on 18-5-11.
//

#ifndef SAOCLIB_CPP_KERNELARGSIGNATURE_H
#define SAOCLIB_CPP_KERNELARGSIGNATURE_H

#include <cstddef>
#include <cstdio>
#include <string>

#include "utils.h"
#include "NativeTypeID.h"
#include "KernelArgMode.h"

namespace saoclib {

    class KernelArgSignature {
    public:
        template<typename T>
        static KernelArgSignature primitive(KernelArgMode mode) {
            return KernelArgSignature(mode,
                                      NativeTypeIDMapping<T>::typeID,
                                      false,
                                      1);
        }

        template<typename T>
        static KernelArgSignature array(KernelArgMode mode, size_t arrayLength) {
            return KernelArgSignature(mode,
                                      NativeTypeIDMapping<T>::typeID,
                                      true,
                                      arrayLength);
        }

        static KernelArgSignature primitiveFromTypeID(KernelArgMode mode, NativeTypeID id) {
            return KernelArgSignature(mode,
                                      id,
                                      false,
                                      1);
        }

        static KernelArgSignature arrayFromTypeID(KernelArgMode mode, NativeTypeID id, size_t arrayLength) {
            return KernelArgSignature(mode,
                                      id,
                                      true,
                                      arrayLength);
        }


        KernelArgSignature() = default;

        KernelArgSignature(const KernelArgSignature &) = default;

        bool operator==(const KernelArgSignature &rhs) const;

        bool isInput() const;

        bool isOutput() const;

        bool isVoid() const;

        bool isPrimitive() const;

        bool isArray() const;

        KernelArgMode getMode() const;

        NativeTypeID getTypeID() const;

        NativeTypeID getElemTypeID() const;

        size_t getSize() const;

        size_t getElemSize() const;

        size_t getArrayLength() const;

        std::string toString() const;

    private:
        KernelArgSignature(KernelArgMode mode,
                           NativeTypeID typeID,
                           bool isArray_,
                           size_t arrayLength);

        KernelArgMode mode = KernelArgMode::input;
        NativeTypeID typeID;
        bool isArray_;
        size_t arrayLength;

        static size_t primitiveSize(NativeTypeID id);
    };

}
#endif //SAOCLIB_CPP_KERNELARGSIGNATURE_H
