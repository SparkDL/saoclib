//
// Created by pcz on 18-5-14.
//
#include "com_pzque_sparkdl_saoclib_KernelArgLimit__.h"
#include "../src/saoclib.hpp"

using namespace saoclib;
/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    destroyInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_destroyInstance
        (JNIEnv *env, jobject obj, jlong handle) {
    delete reinterpret_cast<KernelArgLimit *>(handle);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    void_output
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_void_1output
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::VoidOutput()));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_int_input
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1int_1input
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::PrimitiveInput<int>()));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_int_output
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1int_1output
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::PrimitiveOutput<int>()));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_float_input
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1float_1input
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::PrimitiveInput<float>()));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_float_output
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1float_1output
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::PrimitiveOutput<float>()));
}
/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_double_input
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1double_1input
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::PrimitiveInput<double>()));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_double_output
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1double_1output
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::PrimitiveOutput<double>()));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_int_array_input
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1int_1array_1input
        (JNIEnv *env, jobject obj, jlong length) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::AlignedBufferInput<int>(length)));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_int_array_output
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1int_1array_1output
        (JNIEnv *env, jobject obj, jlong length) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::AlignedBufferOutput<int>(length)));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_float_array_input
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1float_1array_1input
        (JNIEnv *env, jobject obj, jlong length) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::AlignedBufferInput<float>(length)));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_float_array_output
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1float_1array_1output
        (JNIEnv *env, jobject obj, jlong length) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::AlignedBufferOutput<float>(length)));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_double_array_input
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1double_1array_1input
        (JNIEnv *env, jobject obj, jlong length) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::AlignedBufferInput<double>(length)));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_KernelArgLimit__
 * Method:    c_double_array_output
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_KernelArgLimit_00024_c_1double_1array_1output
        (JNIEnv *env, jobject obj, jlong length) {
    return reinterpret_cast<jlong>(new KernelArgLimit(KernelArgLimit::AlignedBufferOutput<double>(length)));
}
