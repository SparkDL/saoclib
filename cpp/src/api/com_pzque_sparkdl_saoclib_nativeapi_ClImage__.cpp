//
// Created by pcz on 18-5-11.
//

#include "com_pzque_sparkdl_saoclib_nativeapi_ClImage__.h"
#include "../src/CLImage.h"

using namespace saoclib;
/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClImage__
 * Method:    newInstance
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClImage_00024_newInstance
        (JNIEnv *env, jobject obj, jlong p_env, jstring jname) {
    auto *cl_env = reinterpret_cast<CLEnv *>(p_env);
    const char *name_chars = env->GetStringUTFChars(jname, NULL);
    std::string name(name_chars);
    env->ReleaseStringUTFChars(jname, name_chars);
    return reinterpret_cast<jlong>(new ClImage(cl_env, name));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClImage__
 * Method:    destroyInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClImage_00024_destroyInstance
        (JNIEnv *env, jobject obj, jlong p_image) {
    delete reinterpret_cast<ClImage *>(p_image);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_ClImage__
 * Method:    deployImage
 * Signature: (J[J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_ClImage_00024_deployImage
        (JNIEnv *env, jobject obj, jlong p_image, jlongArray jdevices) {
    ClImage *image = reinterpret_cast<ClImage *>(p_image);
    auto num_devices = env->GetArrayLength(jdevices);
    jlong *devices = env->GetLongArrayElements(jdevices, JNI_FALSE);
    bool result = image->deployImage(reinterpret_cast<cl_device_id const *>(devices), num_devices);
    env->ReleaseLongArrayElements(jdevices, devices, 0);
    if (result) {
        return JNI_TRUE;
    }
    return JNI_FALSE;
}