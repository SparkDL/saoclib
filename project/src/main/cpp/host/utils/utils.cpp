//
// Created by pcz on 18-5-17.
//

#ifdef _WIN32
#include <process.h>
#else

#include <unistd.h>

#endif

#include <AOCLUtils/opencl.h>
#include <cstdarg>
#include <thread>
#include <iostream>
#include <sstream>
#include "utils.h"

namespace saoclib {

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

    void executeTime(const std::function<void()> &function, const char *name) {
        double start, end;
        start = aocl_utils::getCurrentTimestamp();
        function();
        end = aocl_utils::getCurrentTimestamp();
        log("Operation %s cost: %0.3fms\n", name, (end - start) * 1000);
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


}