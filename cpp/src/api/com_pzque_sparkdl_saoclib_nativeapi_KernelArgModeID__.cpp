#include "com_pzque_sparkdl_saoclib_nativeapi_KernelArgModeID__.h"
#include "../src/KernelArgLimit.h"
using namespace saoclib;

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_KernelArgModeID__
 * Method:    id_mode_input
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_KernelArgModeID_00024_id_1mode_1input
  (JNIEnv *, jobject){
    return static_cast<jlong>(KernelArgMode::mode_input);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_KernelArgModeID__
 * Method:    id_mode_output
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_KernelArgModeID_00024_id_1mode_1output
        (JNIEnv *, jobject){
    return static_cast<jlong>(KernelArgMode::mode_output);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_KernelArgModeID__
 * Method:    id_mode_input_output
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_KernelArgModeID_00024_id_1mode_1input_1output
        (JNIEnv *, jobject){
    return static_cast<jlong>(KernelArgMode::mode_input_output);
}
