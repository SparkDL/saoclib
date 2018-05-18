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
        static const TypeTagPrimitive *getTypeTag();

        static const TypeTagPrimitive *getByteTypeTag();

        static const TypeTagPrimitive *getShortTypeTag();

        static const TypeTagPrimitive *getIntTypeTag();

        static const TypeTagPrimitive *getLongTypeTag();

        static const TypeTagPrimitive *getFloatTypeTag();

        static const TypeTagPrimitive *getDoubleTypeTag();

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
        TypeTagPrimitive(NativeTypeID typeID);
    };
}

#endif //SAOCLIB_CPP_TYPETAGPRIMITIVE_H
