//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_TYPETAGQUERY_H
#define SAOCLIB_CPP_TYPETAGQUERY_H

#include <cstddef>
#include <string>
#include <memory>
#include "NativeTypeID.h"


namespace saoclib {
    class TypeTag;

    class TypeTagQuery {
    public:
        virtual ~TypeTagQuery() = default;

        virtual bool isVoid() const = 0;

        virtual bool isPrimitive() const = 0;

        virtual bool isArray() const = 0;

        virtual NativeTypeID getTypeID() const = 0;

        virtual NativeTypeID getElemTypeID() const =0;

        virtual const std::shared_ptr<TypeTag> &getElemType() const = 0;

        /**
         * Get the total size of data stored in the inside pointer.
         * For Void class, it will be 0.
         * For Primitive class, it will be the size of the primitive data, for example, sizeof a int/float/char...
         * For AlignedBuffer class, it will be element size * array length.
         * @return the total size of data
         */
        virtual size_t getSize() const = 0;

        /**
         * Get size of an element, only used for AlignedBuffer class;
         * @return element size
         * @note this function is only used for AlignedBuffer class
         */
        virtual size_t getElemSize() const = 0;

        /**
         * Get length of array, only used for AlignedBuffer class;
         * @return length of array store
         * @note this function is only used for AlignedBuffer class
         */
        virtual size_t getArrayLength() const = 0;

        virtual std::string toString() const = 0;
    };
}
#endif //SAOCLIB_CPP_TYPETAGQUERY_H
