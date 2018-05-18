//
// Created by pcz on 18-5-10.
//

#include "com_pzque_sparkdl_saoclib_nativeapi_ClEnv__.h"
#include "../src/saoclib.h"

using namespace saoclib;

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClEnv__
 * Method:    newInstance
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClEnv_00024_newInstance
        (JNIEnv *env, jobject obj) {
    return reinterpret_cast<jlong>(new ClEnv());
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClEnv__
 * Method:    destroyInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClEnv_00024_destroyInstance
        (JNIEnv *env, jobject obj, jlong ptr) {
    delete reinterpret_cast<ClEnv *>(ptr);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClEnv__
 * Method:    initOpenCL
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClEnv_00024_initOpenCL
        (JNIEnv *env, jobject obj, jlong ptr) {
    auto *cl_env = reinterpret_cast<ClEnv *>(ptr);
    bool result = cl_env->initOpenCL();
    if (result) {
        return JNI_TRUE;
    }
    return JNI_FALSE;
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClEnv__
 * Method:    getNumDevices
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClEnv_00024_getNumDevices
        (JNIEnv *env, jobject obj, jlong ptr) {
    auto *cl_env = reinterpret_cast<ClEnv *>(ptr);
    return cl_env->getNumDevices();
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClEnv__
 * Method:    getDeviceIdList
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClEnv_00024_getDeviceIdList
        (JNIEnv *env, jobject obj, jlong ptr) {
    auto *clEnv = reinterpret_cast<ClEnv *>(ptr);
    unsigned num_devices = clEnv->getNumDevices();
    const long *devices = (long *) clEnv->getDevices().get();

    auto array = env->NewLongArray(num_devices);
    env->SetLongArrayRegion(array, 0, num_devices, devices);
    return array;
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClEnv__
 * Method:    getDeviceId
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClEnv_00024_getDeviceId
        (JNIEnv *env, jobject obj, jlong ptr, jint index) {
    auto *clEnv = reinterpret_cast<ClEnv *>(ptr);
    return reinterpret_cast<jlong>(clEnv->getDeviceID(index));
}

void cleanup() {}
