//
// Created by pcz on 18-5-7.
//

#ifndef SAOCLIB_CPP_KERNEL_ARG_H
#define SAOCLIB_CPP_KERNEL_ARG_H

#include <cstddef>
#include "AOCLUtils/aocl_utils.h"

#include "KernelArgLimit.h"
#include "TypeTagVoid.h"
#include "TypeTagPrimitive.h"
#include "TypeTagArray.h"

namespace saoclib {
    using namespace aocl_utils;

    /**
     * This class is used to describe and store kernel parameters/arguments.
     */
    class KernelArg : public KernelArgQuery {
    public:

        KernelArg(KernelArgLimit limit);

        /**
         * Check if the data meets given Limit, including type and size.
         * @param rhs
         * @return if valid
         */
        virtual bool verify(const KernelArgLimit &rhs) const {
            return (limit == rhs);
        }

        /**
         * Get the raw data pointer.
         * For Void class, it should return NULL.
         * @return raw pointer of data
         * @warning this function is dangerous, you should carefully use the returned pointer.
         */
        virtual const void *getReadonlyDataPtr() const =0;

        /**
         * Get the raw data pointer.
         * For Void class, it should return NULL.
         * @return raw pointer of data
         * @warning this function is dangerous, you should carefully use the returned pointer.
         */
        virtual void *getWriteableDataPtr() =0;

        KernelArgMode getMode() const override;

        const std::shared_ptr<TypeTag> & getTypeTag() const override;

        bool isVoid() const override;

        bool isPrimitive() const override;

        bool isArray() const override;

        NativeTypeID getTypeID() const override;

        NativeTypeID getElemTypeID() const override;

        const std::shared_ptr<TypeTag> & getElemType() const override;

        size_t getSize() const override;

        size_t getElemSize() const override;

        size_t getArrayLength() const override;

    private:
        KernelArgLimit limit;
    };

    class Void : public KernelArg {
    public:
        Void() : KernelArg(KernelArgLimit(TypeTagVoid::getInstance(), KernelArgMode::mode_output)) {}

        const void *getReadonlyDataPtr() const override {
            return NULL;
        }

        void *getWriteableDataPtr() override {
            return NULL;
        }
    };

    template<class T>
    class Primitive : public KernelArg {
    public:
        Primitive(T data, KernelArgMode mode)
                : KernelArg(KernelArgLimit(TypeTagPrimitive::getTypeTag<T>(), mode)),
                  data(data) {}

        const void *getReadonlyDataPtr() const override {
            return &data;
        }

        void *getWriteableDataPtr() override {
            return (void *) &data;
        }

    private:
        T data;
        static const size_t size = sizeof(T);
    };

    template<class T>
    class AlignedBuffer : public KernelArg {
    public:
        AlignedBuffer(const scoped_aligned_ptr<T> *dataContainer,
                      size_t arrayLength,
                      KernelArgMode mode)
                : KernelArg(KernelArgLimit(TypeTagArray::getTypeTag<T>(arrayLength), mode)),
                  dataContainer(dataContainer) {}

        ~AlignedBuffer() {
            if (dataContainer) {
                delete dataContainer;
            }
        }

        const void *getReadonlyDataPtr() const override {
            return dataContainer->get();
        }

        void *getWriteableDataPtr() override {
            return dataContainer->get();
        }


    private:
        // remember to release this container in destructor
        const scoped_aligned_ptr<T> *dataContainer;
        static const size_t elemSize = sizeof(T);

    };


    typedef Primitive<signed char> ArgByte;
    typedef Primitive<short> ArgShort;
    typedef Primitive<int> ArgInt;
    typedef Primitive<long> ArgLong;
    typedef Primitive<float> ArgFloat;
    typedef Primitive<double> ArgDouble;

    typedef AlignedBuffer<signed char> ArgBufferByte;
    typedef AlignedBuffer<short> ArgBufferShort;
    typedef AlignedBuffer<int> ArgBufferInt;
    typedef AlignedBuffer<long> ArgBufferLong;
    typedef AlignedBuffer<float> ArgBufferFloat;
    typedef AlignedBuffer<double> ArgBufferDouble;
}
#endif //SAOCLIB_CPP_KERNEL_ARG_H
