//
// Created by pcz on 18-5-7.
//

#ifndef SAOCLIB_CPP_KERNEL_ARG_H
#define SAOCLIB_CPP_KERNEL_ARG_H

#include <cstddef>

namespace saoclib {
    enum class KernelArgType {
        Void, /// Void
        Primitive, /// Primitive data, such as int, float...
        AlignedBuffer  /// Aligned buffer.
    };

    class KernelArgQuery {
    public:
        /**
         * Get data type, which could be KernelDataType::Void/Primitive/AlignedPointer...
         * @return data type
         */
        virtual KernelArgType getType() const = 0;

        /**
         * Get the total size of data stored in the inside pointer.
         * For Void class, it will be 0.
         * For Primitive class, it will be the size of the primitive data, for example, sizeof a int/float/char...
         * For AlignedBuffer class, it will be element size * array length.
         * @return the total size of data
         */
        virtual size_t getSize() const =0;

        /**
         * Get size of an element, only used for AlignedBuffer class;
         * @return element size
         * @note this function is only used for AlignedBuffer class
         */
        virtual size_t getElemSize() const =0;

        /**
         * Get length of array, only used for AlignedBuffer class;
         * @return length of array store
         * @note this function is only used for AlignedBuffer class
         */
        virtual size_t getArrayLength() const =0;


        /**
         * Print data info.
         */
        void print() {
            switch (getType()) {
                case KernelArgType::Void:
                    printf("<void>\n");
                    break;
                case KernelArgType::Primitive:
                    printf("<primitive,size: %lu>\n", getSize());
                    break;
                case KernelArgType::AlignedBuffer:
                    printf("<aligned buffer,elem size: %lu,length: %lu>\n", getElemSize(), getArrayLength());
                    break;
            }
        }
    };

    class KernelArgLimit : public KernelArgQuery {
    public:
        static KernelArgLimit VoidLimit() {
            return {KernelArgType::Void, 0, 0};
        }

        template<class T>
        static KernelArgLimit PrimitiveLimit() {
            return {KernelArgType::Primitive, sizeof(T), 0};
        }

        template<class T>
        static KernelArgLimit AlignedBufferLimit(size_t array_length) {
            return {KernelArgType::AlignedBuffer, sizeof(T), array_length};
        }

        KernelArgType getType() const override {
            return type;
        }

        size_t getSize() const override {
            if (type == KernelArgType::Void) {
                return 0;
            }
            if (type == KernelArgType::Primitive) {
                return elem_size;
            }
            if (type == KernelArgType::AlignedBuffer) {
                return elem_size * array_length;
            }
        }

        size_t getElemSize() const override {
            return elem_size;
        }

        size_t getArrayLength() const override {
            return array_length;
        }

    private:
        KernelArgLimit(KernelArgType type, size_t elem_size, size_t array_length)
                : type(type),
                  elem_size(elem_size),
                  array_length(array_length) {}


        KernelArgType type;
        size_t elem_size;
        size_t array_length;
    };


/**
 * This class is used to describe and store kernel parameters/arguments.
 */
    class KernelArg : public KernelArgQuery {
    public:
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
    };

    class Void : public KernelArg {
    public:
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
        Primitive(T data) : data(data) {}

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
        T data;
        static const size_t size = sizeof(T);
    };

    template<class T>
    class AlignedBuffer : public KernelArg {
    public:
        AlignedBuffer(const scoped_aligned_ptr <T> *data_container, size_t array_length)
                : data_container(data_container), array_length(array_length) {}

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
        const scoped_aligned_ptr <T> *data_container;
        static const size_t elem_size = sizeof(T);
        size_t array_length;

    };
}
#endif //SAOCLIB_CPP_KERNEL_ARG_H
