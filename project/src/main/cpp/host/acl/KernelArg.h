//
// Created by pcz on 18-5-7.
//

#ifndef SAOCLIB_CPP_KERNEL_ARG_H
#define SAOCLIB_CPP_KERNEL_ARG_H

#include <new>
#include <cstddef>
#include "AOCLUtils/aocl_utils.h"
#include "KernelArgSignature.h"


namespace acl {
    using namespace aocl_utils;

    template<typename T>
    struct aligned_allocator {
        using value_type = T;

        T *allocate(std::size_t num) {
            void *ptr = NULL;
            ptr = alignedMalloc(num * sizeof(T));
            if (ptr == NULL) {
                throw std::bad_alloc();
            }
            return reinterpret_cast<T *>(ptr);
        }

        void deallocate(T *p, std::size_t num) {
            alignedFree(p);
        }
    };

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

        const KernelArgSignature &getSignature() const {
            return signature;
        }

        virtual void setArg(cl_kernel kernel, int argi)=0;

        virtual void bindBuffer(cl_mem *buffer)=0;

        virtual void syncRead(int argi, cl_command_queue queue)=0;

        virtual void syncWrite(int argi, cl_command_queue queue)=0;

        virtual void asyncRead(int argi, cl_command_queue queue, cl_event *waitEvent)=0;

        virtual void asyncWrite(int argi, cl_command_queue queue, cl_event *waitEvent)=0;

    protected:
        KernelArgSignature signature;
    };

    template<class T>
    class Primitive : public KernelArg {
    public:
        Primitive(T data, KernelArgMode mode)
                : KernelArg(KernelArgSignature::primitive<T>(mode)),
                  data(data) {}

        void setArg(cl_kernel kernel, int argi) override {
            cl_int status;
            size_t size = signature.getSize();
            status = clSetKernelArg(kernel, argi, size, (void *) &data);
            checkError(status, "Failed to set argument %d", argi);
        }

        void bindBuffer(cl_mem *buffer) override {}

        void syncRead(int argi, cl_command_queue queue) override {}

        void syncWrite(int argi, cl_command_queue queue) override {}

        void asyncRead(int argi, cl_command_queue queue, cl_event *waitEvent) override {}

        void asyncWrite(int argi, cl_command_queue queue, cl_event *waitEvent) override {}

    private:
        T data;
    };


    template<class T>
    class Array : public KernelArg {
    public:
        Array(T *rawData, size_t length, KernelArgMode mode)
                : KernelArg(KernelArgSignature::array<T>(mode, length)),
                  rawData(rawData),
                  length(length),
                  needCopy((long) rawData % 1024 != 0) {
        }

        void setArg(cl_kernel kernel, int argi) override {
            cl_int status;
            status = clSetKernelArg(kernel, argi, sizeof(cl_mem), buffer);
            checkError(status, "Failed to set argument %d", argi);
        }

        void bindBuffer(cl_mem *buffer) override {
            this->buffer = buffer;
        }

        void syncRead(int argi, cl_command_queue queue) override {
            cl_int status;
            void *ptr = rawData;
            if (needCopy) {
                container.resize(length);
                ptr = container.data();
            }
            status = clEnqueueReadBuffer(queue,
                                         *buffer,
                                         CL_TRUE,
                                         0,
                                         signature.getSize(),
                                         ptr,
                                         0,
                                         NULL,
                                         NULL);
            checkError(status, "Failed to transfer input %d", argi);
            if (needCopy) {
                memcpy(rawData, ptr, sizeof(T) * length);
            }
        }

        void syncWrite(int argi, cl_command_queue queue) override {
            cl_int status;
            void *ptr = rawData;
            if (needCopy) {
                container.resize(length);
                container.assign(rawData, rawData + length);
                ptr = container.data();
            }
            status = clEnqueueWriteBuffer(queue,
                                          *buffer,
                                          CL_TRUE,
                                          0,
                                          signature.getSize(),
                                          ptr,
                                          0,
                                          NULL,
                                          NULL);
            checkError(status, "Failed to transfer input %d", argi);
        }

        void asyncRead(int argi, cl_command_queue queue, cl_event *waitEvent) override {
            cl_int status;
            void *ptr = rawData;
            if (needCopy) {
                container.resize(length);
                ptr = container.data();
            }
            status = clEnqueueReadBuffer(queue,
                                         *buffer,
                                         CL_FALSE,
                                         0,
                                         signature.getSize(),
                                         ptr,
                                         0,
                                         NULL,
                                         waitEvent);
            checkError(status, "Failed to transfer input %d", argi);
            if (needCopy) {
                memcpy(rawData, ptr, sizeof(T) * length);
            }
        }

        void asyncWrite(int argi, cl_command_queue queue, cl_event *waitEvent) override {
            cl_int status;
            void *ptr = rawData;
            if (needCopy) {
                container.resize(length);
                container.assign(rawData, rawData + length);
                ptr = container.data();
            }
            status = clEnqueueWriteBuffer(queue,
                                          *buffer,
                                          CL_FALSE,
                                          0,
                                          signature.getSize(),
                                          ptr,
                                          0,
                                          NULL,
                                          waitEvent);
            checkError(status, "Failed to transfer input %d", argi);
        }

    private:
        T *rawData = NULL;
        size_t length;
        bool needCopy = false;
        std::vector<T, aligned_allocator<T>> container;
        cl_mem *buffer = NULL;
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
