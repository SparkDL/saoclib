//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_TYPETAGVOID_H
#define SAOCLIB_CPP_TYPETAGVOID_H

#include <memory>
#include "TypeTag.h"

namespace saoclib {
    class TypeTagVoid : public TypeTag {
    public:
        TypeTagVoid();

        virtual ~TypeTagVoid() = default;

        static const std::shared_ptr<TypeTagVoid> &getInstance();

        bool equals(const std::shared_ptr<TypeTag> &rhs) const override;

        bool isVoid() const override;

        bool isPrimitive() const override;

        bool isArray() const override;

        NativeTypeID getTypeID() const override;

        NativeTypeID getElemTypeID() const override;

        const std::shared_ptr<TypeTag> &getElemType() const override;

        size_t getSize() const override;

        size_t getElemSize() const override;

        size_t getArrayLength() const override;

        std::string toString() const override;


    };
}

#endif //SAOCLIB_CPP_TYPETAGVOID_H
