//
// Created by pcz on 18-5-18.
//

#ifndef SAOCLIB_CPP_TYPETAGARRAY_H
#define SAOCLIB_CPP_TYPETAGARRAY_H

#include "TypeTag.h"
#include "TypeTagPrimitive.h"

namespace saoclib {
    class TypeTagArray : public TypeTag {
    public:
        template<class T>
        static std::shared_ptr<TypeTagArray> getTypeTag(size_t arrayLength) {
            return fromElemType(TypeTagPrimitive::getTypeTag<T>(), arrayLength);
        }

        static std::shared_ptr<TypeTagArray> fromElemType(const std::shared_ptr<TypeTag> &elemType, size_t arrayLength);


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

        TypeTagArray(const std::shared_ptr<TypeTag> &elemType, size_t arrayLength)
                : TypeTag(NativeTypeID::c_array),
                  elemType(elemType),
                  arrayLength(arrayLength) {

        }

    private:
        std::shared_ptr<TypeTag> elemType;
        size_t arrayLength;
    };
}

#endif //SAOCLIB_CPP_TYPETAGARRAY_H
