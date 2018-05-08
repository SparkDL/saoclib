//
// Created by pcz on 18-5-7.
//

#ifndef AOCL_CPP_KERNEL_ARG_H
#define AOCL_CPP_KERNEL_ARG_H

#include <cstddef>

enum class KernelDataType {
    Void, /// Void
    Primitive, /// Primitive data, such as int, float...
    AlignedBuffer  /// Aligned buffer.
};

class KernelDataQuery {
public:
    /**
     * Get data type, which could be KernelDataType::Void/Primitive/AlignedPointer...
     * @return data type
     */
    virtual KernelDataType getType() const = 0;

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
            case KernelDataType::Void:
                printf("<void>\n");
                break;
            case KernelDataType::Primitive:
                printf("<primitive,size: %lu>\n", getSize());
                break;
            case KernelDataType::AlignedBuffer:
                printf("<aligned buffer,elem size: %lu,length: %lu>\n", getElemSize(), getArrayLength());
                break;
        }
    }
};

class KernelDataLimit : public KernelDataQuery {
public:
    static KernelDataLimit VoidLimit() {
        return {KernelDataType::Void, 0, 0};
    }

    template<class T>
    static KernelDataLimit PrimitiveLimit() {
        return {KernelDataType::Primitive, sizeof(T), 0};
    }

    template<class T>
    static KernelDataLimit AlignedBufferLimit(size_t array_length) {
        return {KernelDataType::AlignedBuffer, sizeof(T), array_length};
    }

    KernelDataType getType() const override {
        return type;
    }

    size_t getSize() const override {
        if (type == KernelDataType::Void) {
            return 0;
        }
        if (type == KernelDataType::Primitive) {
            return elem_size;
        }
        if (type == KernelDataType::AlignedBuffer) {
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
    KernelDataLimit(KernelDataType type, size_t elem_size, size_t array_length)
            : type(type),
              elem_size(elem_size),
              array_length(array_length) {}


    KernelDataType type;
    size_t elem_size;
    size_t array_length;
};


/**
 * This class is used to describe and store kernel parameters/arguments.
 */
class KernelArg : public KernelDataQuery {
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
    virtual bool checkValid(const KernelDataLimit &limit) const =0;
};

class Void : public KernelArg {
public:
    KernelDataType getType() const override {
        return KernelDataType::Void;
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


    bool checkValid(const KernelDataLimit &limit) const override {
        return limit.getType() == KernelDataType::Void;
    }
};

template<class T>
class Primitive : public KernelArg {
public:
    Primitive(T data) : data(data) {}

    KernelDataType getType() const override {
        return KernelDataType::Primitive;
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

    bool checkValid(const KernelDataLimit &limit) const override {
        return limit.getType() == KernelDataType::Primitive && limit.getElemSize() == size;
    }

private:
    T data;
    static const size_t size = sizeof(T);
};

template<class T>
class AlignedBuffer : public KernelArg {
public:
    AlignedBuffer(const scoped_aligned_ptr<T> *data_container, size_t array_length)
            : data_container(data_container), array_length(array_length) {}

    KernelDataType getType() const override {
        return KernelDataType::AlignedBuffer;
    }

    bool checkValid(const KernelDataLimit &limit) const override {
        return limit.getType() == KernelDataType::AlignedBuffer && limit.getElemSize() == elem_size &&
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
    const scoped_aligned_ptr<T> *data_container;
    static const size_t elem_size = sizeof(T);
    size_t array_length;

};

#endif //AOCL_CPP_KERNEL_ARG_H
