//
// Created by pcz on 18-5-17.
//

#ifdef _WIN32
#include <process.h>
#else

#include <unistd.h>

#endif

#include "sys/file.h"

#include "AOCLUtils/opencl.h"
#include <cstdarg>
#include <thread>
#include <iostream>
#include <sstream>
#include "utils.h"

namespace acl {

    bool is_first_running() {
        int pid_file = open("/tmp/aclmkl.pid", O_CREAT | O_RDWR, 0666);
        int rc = flock(pid_file, LOCK_EX | LOCK_NB);
        if (rc) {
            if (EWOULDBLOCK == errno) {
                return false;
            }
        } else {
            return true;
        }
    }

    void log(const std::string &format, ...) {
#ifndef NDEBUG
        auto pid = getpid();
        std::string copiedFormat = format;
        std::thread::id tid = std::this_thread::get_id();
        std::stringstream stream;

        for (int i = 0; i < copiedFormat.length(); i++) {
            auto e = copiedFormat[i];
            if (std::isblank(e) || e == '\n') {
                stream << e;
                copiedFormat.erase(i, 1);
            } else {
                break;
            }
        }

        stream << "Process(" << pid << ")-" << "Thread(" << tid << "): " << copiedFormat;

        va_list args;
        va_start(args, format);
        vprintf(stream.str().c_str(), args);
        va_end(args);
#endif
    }

    std::string getThreadString() {
        std::thread::id tid = std::this_thread::get_id();
        std::stringstream stream;
        stream << tid;
        return stream.str();
    }

    int paddedSize(int rawSize, int blockSize) {
        int padding = blockSize - rawSize % blockSize;
        if (padding == blockSize) {
            padding = 0;
        }
        return rawSize + padding;
    }

    double executeTime(const std::function<void()> &function, const char *name, int repeat) {
        double start, end;
        start = aocl_utils::getCurrentTimestamp();
        for (int i = 0; i < repeat; i++) {
            function();
        }
        end = aocl_utils::getCurrentTimestamp();
        auto time = end - start;
        log("Operation %s, repeat %d, average cost: %0.4fms\n", name, repeat, time * 1000 / repeat);
        return time;
    }

    size_t primitiveTypeSize(NativeTypeID typeID) {
        switch (typeID) {
            case NativeTypeID::c_byte:
                return sizeof(signed char);
            case NativeTypeID::c_short:
                return sizeof(short);
            case NativeTypeID::c_int:
                return sizeof(int);
            case NativeTypeID::c_long:
                return sizeof(long);
            case NativeTypeID::c_float:
                return sizeof(float);
            case NativeTypeID::c_double:
                return sizeof(double);
            default:
                assert(false && "wrong branch");
                return 0;
        }
    }

    const char *nativeTypeRepr(NativeTypeID typeID) {
        switch (typeID) {
            case NativeTypeID::c_byte:
                return "byte";
            case NativeTypeID::c_short:
                return "short";
            case NativeTypeID::c_int:
                return "int";
            case NativeTypeID::c_long:
                return "long";
            case NativeTypeID::c_float:
                return "float";
            case NativeTypeID::c_double:
                return "double";
            case NativeTypeID::c_array:
                return "array";
        }
    }

    const char *argModeRepr(KernelArgMode mode) {
        switch (mode) {
            case KernelArgMode::input:
                return "input";
            case KernelArgMode::output:
                return "output";
            case KernelArgMode::input_output:
                return "input+output";
        }
    }

    bool isInput(KernelArgMode mode) {
        switch (mode) {
            case KernelArgMode::output:
                return false;
            case KernelArgMode::input:
            case KernelArgMode::input_output:
                return true;
        }
    }

    bool isOutput(KernelArgMode mode) {
        switch (mode) {
            case KernelArgMode::input:
                return false;
            case KernelArgMode::output:
            case KernelArgMode::input_output:
                return true;
        }
    }

    bool isTrans(CBLAS_TRANSPOSE trans) {
        switch (trans) {
            case CblasTrans:
                return true;
            default:
                return false;
        }
    }

    void printMatrix(const char *name, float *a, int rows, int cols) {
        printf("\n%s:\n", name);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%.4f,", a[rows * j + i]);
            }
            printf("\n");
        }
        printf("\n");
    }

}