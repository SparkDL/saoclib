//
// Created by pcz on 18-5-7.
//

#ifndef SAOCLIB_CPP_KERNEL_ARG_H
#define SAOCLIB_CPP_KERNEL_ARG_H

#include <cstddef>
#include "KernelArgLimit.hpp"

namespace saoclib {

    /**
     * This class is used to describe and store kernel parameters/arguments.
     */
    class KernelArg : public KernelArgQuery {
    public:
        KernelArg(KernelArgMode mode) : mode(mode) {}

        KernelArgMode getMode() const override {
            return mode;
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

        /**
         * Check if the data meets given Limit, including type and size.
         * @param limit
         * @return if valid
         */
        virtual bool checkValid(const KernelArgLimit &limit) const =0;

    private:
        KernelArgMode mode;
    };

    class Void : public KernelArg {
    public:
        Void() : KernelArg(KernelArgMode::Output) {}

        KernelArgType getType() const override {
            return KernelArgType::Void;
        }

        const void *getReadonlyDataPtr() const override {
            return NULL;
        }

        void *getWriteableDataPtr() override {
            return NULL;
        }

        size_t getSize() const override {
            return 0;
        }

        size_t getElemSize() const override {
            return 0;
        }

        size_t getArrayLength() const override {
            return 0;
        }

        bool checkValid(const KernelArgLimit &limit) const override {
            return limit.getType() == KernelArgType::Void;
        }
    };

    template<class T>
    class Primitive : public KernelArg {
    public:
        static Primitive Input(T data) {
            return {KernelArgMode::Input, data};
        }

        static Primitive Output(T data) {
            return {KernelArgMode::Output, data};
        }

        KernelArgType getType() const override {
            return KernelArgType::Primitive;
        }

        const void *getReadonlyDataPtr() const override {
            return &data;
        }

        void *getWriteableDataPtr() override {
            return (void *) &data;
        }

        size_t getSize() const override {
            return size;
        }

        size_t getElemSize() const override {
            return 0;
        }

        size_t getArrayLength() const override {
            return 0;
        }

        bool checkValid(const KernelArgLimit &limit) const override {
            return limit.getType() == KernelArgType::Primitive && limit.getElemSize() == size;
        }

    private:
        Primitive(KernelArgMode mode, T data) : KernelArg(mode), data(data) {}

        T data;
        static const size_t size = sizeof(T);
    };

    template<class T>
    class AlignedBuffer : public KernelArg {
    public:
        static AlignedBuffer Input(const scoped_aligned_ptr<T> *data_container, size_t array_length) {
            return {KernelArgMode::Input, data_container, array_length};
        }

        static AlignedBuffer Output(const scoped_aligned_ptr<T> *data_container, size_t array_length) {
            return {KernelArgMode::Output, data_container, array_length};
        }

        KernelArgType getType() const override {
            return KernelArgType::AlignedBuffer;
        }

        bool checkValid(const KernelArgLimit &limit) const override {
            return limit.getType() == KernelArgType::AlignedBuffer && limit.getElemSize() == elem_size &&
                   limit.getArrayLength() == array_length;
        }

        const void *getReadonlyDataPtr() const override {
            return data_container->get();
        }

        void *getWriteableDataPtr() override {
            return data_container->get();
        }

        size_t getElemSize() const override {
            return elem_size;
        }

        size_t getArrayLength() const override {
            return array_length;
        }

        size_t getSize() const override {
            return elem_size * array_length;
        }

    private:
        AlignedBuffer(KernelArgMode mode, const scoped_aligned_ptr<T> *data_container, size_t array_length)
                : KernelArg(mode), data_container(data_container), array_length(array_length) {}

        const scoped_aligned_ptr<T> *data_container;
        static const size_t elem_size = sizeof(T);
        size_t array_length;

    };


    typedef Primitive<char> Char;
    typedef Primitive<int> Int;
    typedef Primitive<unsigned> Unsigned;
    typedef Primitive<float> Float;

    typedef AlignedBuffer<char> CharBuffer;
    typedef AlignedBuffer<int> IntBuffer;
    typedef AlignedBuffer<unsigned> UnsignedBuffer;
    typedef AlignedBuffer<float> FloatBuffer;
}
#endif //SAOCLIB_CPP_KERNEL_ARG_H
