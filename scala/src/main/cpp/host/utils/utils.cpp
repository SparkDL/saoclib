//
// Created by pcz on 18-5-17.
//

#include "utils.h"

namespace saoclib {

    size_t primitiveTypeSize(NativeTypeID typeID) {
        switch (typeID) {
            case NativeTypeID::c_void:
                return 0;
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
            case NativeTypeID::c_void:
                return "void";
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
            case KernelArgMode::mode_input:
                return "input";
            case KernelArgMode::mode_output:
                return "output";
            case KernelArgMode::mode_input_output:
                return "input+output";
        }
    }

    bool isInput(KernelArgMode mode) {
        switch (mode) {
            case KernelArgMode::mode_output:
                return false;
            case KernelArgMode::mode_input:
            case KernelArgMode::mode_input_output:
                return true;
        }
    }

    bool isOutput(KernelArgMode mode) {
        switch (mode) {
            case KernelArgMode::mode_input:
                return false;
            case KernelArgMode::mode_output:
            case KernelArgMode::mode_input_output:
                return true;
        }
    }

}