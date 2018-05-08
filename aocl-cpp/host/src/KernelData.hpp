//
// Created by pcz on 18-5-7.
//

#ifndef AOCL_CPP_KERNEL_DATA_H
#define AOCL_CPP_KERNEL_DATA_H

#include <cstddef>

enum class KernelDataType {
    Void, /// Void
    Primitive, /// Primitive data, such as int, float...
    AlignedBuffer  /// Aligned buffer.
};

struct KernelDataLimit {
    KernelDataType type;
    size_t elem_size;
    size_t array_length;

    void print(){
        switch (type) {
            case KernelDataType ::Void:
                printf("<void>\n");
                break;
            case KernelDataType ::Primitive:
                printf("<primitive,size: %lu>\n",elem_size);
                break;
            case KernelDataType ::AlignedBuffer:
                printf("<aligned buffer,elem size: %lu,length: %lu>\n",elem_size,array_length);
                break;
        }
    }
};

/**
 * Readonly data used for kernel.
 */
class KernelData {
public:
    /**
     * Get data type, which could be KernelDataType::Void/Primitive/AlignedPointer...
     * @return data type
     */
    virtual KernelDataType getType() const = 0;

    /**
     * Get the raw data pointer.
     * For Void class, it should return NULL.
     * @return raw pointer of data
     */
    virtual const void *getDataPtr() const =0;

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
     * Check if the data meets a @se, including type and size.
     * @param limit
     * @return if valid
     */
    virtual bool checkValid(const KernelDataLimit &limit) const =0;

    KernelData() = default;
};

class Void : public KernelData {
public:
    KernelDataType getType() const override {
        return KernelDataType::Void;
    }

    const void *getDataPtr() const override {
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


    bool checkValid(const KernelDataLimit &limit) const override {
        return limit.type == KernelDataType::Void;
    }
};

class Primitive : public KernelData {
public:
    Primitive(const void *data, size_t size) : data(data), size(size) {}

    KernelDataType getType() const override {
        return KernelDataType::Primitive;
    }

    const void *getDataPtr() const override {
        return data;
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

    bool checkValid(const KernelDataLimit &limit) const override {
        return limit.type == KernelDataType::Primitive && limit.elem_size == size;
    }

private:
    const void *data;
    size_t size;
};

template<class T>
class AlignedBuffer : public KernelData {
public:
    AlignedBuffer(const scoped_aligned_ptr<T> *data_container, size_t array_length)
            : data_container(data_container), array_length(array_length) {}

    KernelDataType getType() const override {
        return KernelDataType::AlignedBuffer;
    }

    bool checkValid(const KernelDataLimit &limit) const override {
        return limit.type == KernelDataType::AlignedBuffer && limit.elem_size == elem_size &&
               limit.array_length == array_length;
    }

    const void *getDataPtr() const override {
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
    const scoped_aligned_ptr<T> *data_container;
    static const size_t elem_size = sizeof(T);
    size_t array_length;

};

#endif //AOCL_CPP_KERNEL_DATA_H
