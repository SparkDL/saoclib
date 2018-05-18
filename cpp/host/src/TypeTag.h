//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_KERNELARGTYPE_H
#define SAOCLIB_CPP_KERNELARGTYPE_H

#include "NativeTypeID.h"
#include "TypeTagQuery.h"
#include "utils.h"

namespace saoclib {
    class TypeTag : public TypeTagQuery {
    public:
        virtual bool equals(const TypeTag *rhs) const = 0;

    protected:
        TypeTag(NativeTypeID typeID);;
        NativeTypeID typeID = NativeTypeID::c_void;
    };
}
#endif //SAOCLIB_CPP_KERNELARGTYPE_H
