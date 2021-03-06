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

        virtual ~KernelArg() = default;

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

        virtual size_t bufferSize()=0;

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

        ~Primitive() override = default;

        void setArg(cl_kernel kernel, int argi) override {
            cl_int status;
            size_t size = signature.getSize();
            status = clSetKernelArg(kernel, argi, size, (void *) &data);
            checkError(status, "Failed to set argument %d", argi);
        }

        size_t bufferSize() override {
            return sizeof(T);
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

        ~Array() override = default;

        size_t bufferSize() override {
            return sizeof(T) * length;
        }

        void setArg(cl_kernel kernel, int argi) override {
            cl_int status;
            status = clSetKernelArg(kernel, argi, sizeof(cl_mem), buffer);
            std::stringstream stream;
            stream << "Failed to set argument " << argi;
            checkError(status, stream.str().c_str());
        }

        void bindBuffer(cl_mem *buffer) override {
            this->buffer = buffer;
        }

        void syncRead(int argi, cl_command_queue queue) override {
            cl_int status;
            void *ptr = rawData;
            if (this->isNeedCopy()) {
                this->resizeContainer();
                ptr = container.data();
            }
            status = clEnqueueReadBuffer(queue,
                                         *buffer,
                                         CL_TRUE,
                                         0,
                                         this->bufferSize(),
                                         ptr,
                                         0,
                                         NULL,
                                         NULL);
            checkError(status, "Failed to transfer input %d", argi);
            if (this->isNeedCopy()) {
                this->readContainer();
            }
        }

        void syncWrite(int argi, cl_command_queue queue) override {
            cl_int status;
            void *ptr = rawData;
            if (this->isNeedCopy()) {
                this->resizeContainer();
                this->writeContainer();
                ptr = container.data();
            }
            status = clEnqueueWriteBuffer(queue,
                                          *buffer,
                                          CL_TRUE,
                                          0,
                                          this->bufferSize(),
                                          ptr,
                                          0,
                                          NULL,
                                          NULL);
            checkError(status, "Failed to transfer input %d", argi);
        }

        void asyncRead(int argi, cl_command_queue queue, cl_event *waitEvent) override {
            cl_int status;
            void *ptr = rawData;
            if (this->isNeedCopy()) {
                this->resizeContainer();
                ptr = container.data();
            }
            status = clEnqueueReadBuffer(queue,
                                         *buffer,
                                         CL_FALSE,
                                         0,
                                         this->bufferSize(),
                                         ptr,
                                         0,
                                         NULL,
                                         waitEvent);
            checkError(status, "Failed to transfer input %d", argi);
            if (this->isNeedCopy()) {
                this->readContainer();
            }
        }

        void asyncWrite(int argi, cl_command_queue queue, cl_event *waitEvent) override {
            cl_int status;
            void *ptr = rawData;
            if (this->isNeedCopy()) {
                this->resizeContainer();
                this->writeContainer();
                ptr = container.data();
            }
            status = clEnqueueWriteBuffer(queue,
                                          *buffer,
                                          CL_FALSE,
                                          0,
                                          this->bufferSize(),
                                          ptr,
                                          0,
                                          NULL,
                                          waitEvent);
            checkError(status, "Failed to transfer input %d", argi);
        }

        virtual bool isNeedCopy() {
            return needCopy;
        }

        virtual void resizeContainer() {
            container.resize(length);
        }

        virtual void writeContainer() {
            container.assign(rawData, rawData + length);
        }

        virtual void readContainer() {
            memcpy(rawData, container.data(), sizeof(T) * length);
        }

    protected:
        T *rawData = NULL;
        size_t length;
        bool needCopy = false;
        std::vector<T, aligned_allocator<T>> container;
        cl_mem *buffer = NULL;
    };

    template<class T, int BLOCK = 64>
    class Matrix : public Array<T> {
    public:
        Matrix(T *rawData,
               int realRows,
               int rows,
               int cols,
               bool transpose,
               KernelArgMode mode)
                : Array<T>(rawData, rows * cols, mode),
                  transpose(transpose),
                  dataRealRows(realRows),
                  dataRows(rows),
                  dataCols(cols) {
            paddedRows = paddedSize(dataRows, BLOCK);
            paddedCols = paddedSize(dataCols, BLOCK);
            needCopy = !((long) rawData % 1024 == 0 && paddedRows == dataRows && paddedCols == dataCols);
        }

        virtual ~Matrix() = default;

        int getPaddedRows() {
            return paddedRows;
        }

        int getPaddedCols() {
            return paddedCols;
        }

        size_t bufferSize() {
            return sizeof(T) * paddedRows * paddedCols;
        }

        bool isNeedCopy() override {
            return needCopy;
        }

        void resizeContainer() override {
            this->container.resize(paddedRows * paddedCols);
        }

        void writeContainer() override {
            log("Writing container\n");
            if (transpose) {
                /// @note M.T is a dataRows*dataCols matrix, thus M is a dataCols*dataRows matrix
                matrixTransposeCopy<T>(this->rawData, dataCols, dataRows, dataRealRows,
                                       this->container.data(), paddedRows, paddedCols, paddedRows);
            } else {
                matrixCopy<T>(this->rawData, dataRows, dataCols, dataRealRows,
                              this->container.data(), paddedRows, paddedCols, paddedRows);
            }
        }

        void readContainer() override {
            if (transpose) {
                matrixTransposeCopy<T>(this->container.data(), paddedRows, paddedCols, paddedRows,
                                       this->rawData, dataRows, dataCols, dataRealRows);
            } else {
                matrixCopy<T>(this->container.data(), paddedRows, paddedCols, paddedRows,
                              this->rawData, dataRows, dataCols, dataRealRows);
            }
        }

    private:
        bool needCopy;
        bool transpose;
        int dataRealRows;
        int dataRows;
        int dataCols;
        int paddedRows;
        int paddedCols;
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
