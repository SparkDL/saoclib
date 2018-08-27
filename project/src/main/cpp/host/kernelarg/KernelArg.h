//
// Created by pcz on 18-5-7.
//

#ifndef SAOCLIB_CPP_KERNEL_ARG_H
#define SAOCLIB_CPP_KERNEL_ARG_H

#include <cstddef>
#include "AOCLUtils/aocl_utils.h"

#include "KernelArgSignature.h"

namespace saoclib {
    using namespace aocl_utils;

    /**
     * This class is used to describe and store kernel parameters/arguments.
     */
    class KernelArg {
    public:
        explicit KernelArg(KernelArgSignature signature) : signature(signature) {};

        /**
         * Check if the data meets given Limit, including type and size.
         * @param rhs
         * @return if valid
         */
        bool verify(const KernelArgSignature &rhs) const {
            return (signature == rhs);
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

        const KernelArgSignature &getSignature() const {
            return signature;
        }

    private:
        KernelArgSignature signature;
    };

    template<class T>
    class Primitive : public KernelArg {
    public:
        Primitive(T data, KernelArgMode mode)
                : KernelArg(KernelArgSignature::primitive<T>(mode)),
                  data(data) {}

        const void *getReadonlyDataPtr() const override {
            return &data;
        }

        void *getWriteableDataPtr() override {
            return (void *) &data;
        }

    private:
        T data;
    };

    template<class T>
    class Array : public KernelArg {
    public:
        Array(T *dataContainer,
              size_t arrayLength,
              KernelArgMode mode)
                : KernelArg(KernelArgSignature::array<T>(mode, arrayLength)),
                  dataContainer(dataContainer) {
        }

        // TODO check if 'dataContainer' needs to be released
        ~Array() {
        }

        const void *getReadonlyDataPtr() const override {
            return dataContainer;
        }

        void *getWriteableDataPtr() override {
            return dataContainer;
        }

    private:
        // remember to release this container in destructor
        T *dataContainer;
    };


    typedef Primitive<signed char> ByteArg;
    typedef Primitive<short> ShortArg;
    typedef Primitive<int> IntArg;
    typedef Primitive<long> LongArg;
    typedef Primitive<float> FloatArg;
    typedef Primitive<double> DoubleArg;

    typedef Array<signed char> ByteArrayArg;
    typedef Array<short> ShortArrayArg;
    typedef Array<int> IntArrayArg;
    typedef Array<long> LongArrayArg;
    typedef Array<float> FloatArrayArg;
    typedef Array<double> DoubleArrayArg;
}
#endif //SAOCLIB_CPP_KERNEL_ARG_H
