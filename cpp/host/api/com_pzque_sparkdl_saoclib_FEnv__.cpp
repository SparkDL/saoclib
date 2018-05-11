//
// Created by pcz on 18-5-10.
//

#include "com_pzque_sparkdl_saoclib_FEnv__.h"
#include "../src/saoclib.hpp"

using namespace saoclib;

/*
 * Class:     com_pzque_sparkdl_saoclib_FEnv__
 * Method:    newInstance
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_FEnv_00024_newInstance
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new FEnv());
}

/*
 * Class:     com_pzque_sparkdl_saoclib_FEnv__
 * Method:    destroyInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_FEnv_00024_destroyInstance
        (JNIEnv *env, jobject obj, jlong ptr) {
    delete reinterpret_cast<FEnv *>(ptr);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_FEnv__
 * Method:    initOpenCL
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pzque_sparkdl_saoclib_FEnv_00024_initOpenCL
        (JNIEnv *env, jobject obj, jlong ptr) {
    auto *fenv = reinterpret_cast<FEnv *>(ptr);
    bool result = fenv->init_opencl();
    if (result) {
        return JNI_TRUE;
    }
    return JNI_FALSE;
}

/*
 * Class:     com_pzque_sparkdl_saoclib_FEnv__
 * Method:    getNumDevices
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_FEnv_00024_getNumDevices
        (JNIEnv *env, jobject obj, jlong ptr) {
    auto *fenv = reinterpret_cast<FEnv *>(ptr);
    return fenv->getNumDevices();
}

/*
 * Class:     com_pzque_sparkdl_saoclib_FEnv__
 * Method:    getDeviceIdList
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_pzque_sparkdl_saoclib_FEnv_00024_getDeviceIdList
        (JNIEnv *env, jobject obj, jlong ptr) {
    auto *fenv = reinterpret_cast<FEnv *>(ptr);
    unsigned num_devices = fenv->getNumDevices();
    const long *devices = (long *) fenv->getDevices().get();

    auto array = env->NewLongArray(num_devices);
    env->SetLongArrayRegion(array, 0, num_devices, devices);
    return array;
}

/*
 * Class:     com_pzque_sparkdl_saoclib_FEnv__
 * Method:    getDeviceId
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_FEnv_00024_getDeviceId
        (JNIEnv *env, jobject obj, jlong ptr, jint index) {
    auto *fenv = reinterpret_cast<FEnv *>(ptr);
    return reinterpret_cast<jlong>(fenv->getDeviceId(index));
}

void cleanup() {}
