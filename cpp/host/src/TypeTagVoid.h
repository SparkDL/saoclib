//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_TYPETAGVOID_H
#define SAOCLIB_CPP_TYPETAGVOID_H

#include "TypeTag.h"

namespace saoclib {
    class TypeTagVoid : public TypeTag {
    public:
        static TypeTagVoid *getInstance();

        bool equals(const TypeTag *rhs) const override;

        bool isVoid() const override;

        bool isPrimitive() const override;

        bool isArray() const override;

        NativeTypeID getTypeID() const override;

        NativeTypeID getElemTypeID() const override;

        const TypeTag *getElemType() const override;

        size_t getSize() const override;

        size_t getElemSize() const override;

        size_t getArrayLength() const override;

        std::string toString() const override;

    private:
        TypeTagVoid();
    };
}

#endif //SAOCLIB_CPP_TYPETAGVOID_H
