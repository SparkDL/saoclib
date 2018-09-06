//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_UTILS_H
#define SAOCLIB_CPP_UTILS_H

#include <cstddef>
#include <cassert>
#include <functional>
#include "NativeTypeID.h"
#include "KernelArgMode.h"
#include "aclmkl.h"

namespace acl {
    void log(const std::string &format, ...);

    std::string getThreadString();

    int paddedSize(int rawSize, int blockSize);

    double executeTime(const std::function<void()> &function, const char *name, int repeat = 1);

    size_t primitiveTypeSize(NativeTypeID typeID);

    const char *nativeTypeRepr(NativeTypeID typeID);

    const char *argModeRepr(KernelArgMode mode);

    bool isInput(KernelArgMode mode);

    bool isOutput(KernelArgMode mode);

    bool isTrans(CBLAS_TRANSPOSE trans);

    void printMatrix(const char *name, float *a, int rows, int cols);


    template<typename T>
    void matrixCopy(T *src, int src_rows, int src_cols, int src_real_rows,
                    T *dest, int dest_rows, int dest_cols, int dest_real_rows) {
        if (dest_rows == src_rows && dest_cols == src_cols) {
            memcpy(dest, src, src_rows * src_cols * sizeof(T));
        } else {
            memset(dest, 0, dest_rows * dest_cols * sizeof(T));
            int copy_cols = src_cols < dest_cols ? src_cols : dest_cols;
            int copy_rows = src_rows < dest_rows ? src_rows : dest_rows;
            for (int i = 0; i < copy_cols; i++) {
                memcpy(dest, src, copy_rows * sizeof(T));
                src += src_real_rows;
                dest += dest_real_rows;
            }
        }
    }

    template<typename T>
    void matrixTransposeCopy(T *src, int src_rows, int src_cols, int src_real_rows,
                             T *dest, int dest_rows, int dest_cols, int dest_real_rows) {
        memset(dest, 0, dest_rows * dest_cols * sizeof(T));
        int copy_cols = src_cols < dest_cols ? src_cols : dest_cols;
        int copy_rows = src_rows < dest_rows ? src_rows : dest_rows;
        for (int row = 0; row < copy_rows; row++) {
            for (int col = 0; col < copy_cols; col++) {
                dest[dest_real_rows * row + col] = src[src_real_rows * col + row];
            }
        }
    }
}
#endif //SAOCLIB_CPP_UTILS_H
