//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_TYPETAGPRIMITIVE_H
#define SAOCLIB_CPP_TYPETAGPRIMITIVE_H

#include "TypeTag.h"
#include "TypeTagVoid.h"

namespace saoclib {
    class TypeTagPrimitive : public TypeTag {
    public:
        template<class T>
        static const std::shared_ptr<TypeTagPrimitive> & getTypeTag();

        static const std::shared_ptr<TypeTagPrimitive> & getByteTypeTag();

        static const std::shared_ptr<TypeTagPrimitive> & getShortTypeTag();

        static const std::shared_ptr<TypeTagPrimitive> & getIntTypeTag();

        static const std::shared_ptr<TypeTagPrimitive> & getLongTypeTag();

        static const std::shared_ptr<TypeTagPrimitive> & getFloatTypeTag();

        static const std::shared_ptr<TypeTagPrimitive> & getDoubleTypeTag();

        bool equals(const std::shared_ptr<TypeTag> &rhs) const override;

        bool isVoid() const override;

        bool isPrimitive() const override;

        bool isArray() const override;

        NativeTypeID getTypeID() const override;

        NativeTypeID getElemTypeID() const override;

        const std::shared_ptr<TypeTag> & getElemType() const override;

        size_t getSize() const override;

        size_t getElemSize() const override;

        size_t getArrayLength() const override;

        std::string toString() const override;

        TypeTagPrimitive(NativeTypeID typeID);
    };
}

#endif //SAOCLIB_CPP_TYPETAGPRIMITIVE_H
