//
// Created by pcz on 18-5-11.
//

#include "com_pzque_sparkdl_saoclib_nativeapi_CLImage__.h"
#include "CLBinary.h"

using namespace saoclib;
/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_CLImage__
 * Method:    newInstance
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_CLImage_00024_newInstance
        (JNIEnv *env, jobject obj, jlong p_env, jstring jname) {
    auto *cl_env = reinterpret_cast<CLEnv *>(p_env);
    const char *name_chars = env->GetStringUTFChars(jname, NULL);
    std::string name(name_chars);
    env->ReleaseStringUTFChars(jname, name_chars);
    return reinterpret_cast<jlong>(new CLBinary(cl_env, name));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_CLImage__
 * Method:    destroyInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_CLImage_00024_destroyInstance
        (JNIEnv *env, jobject obj, jlong p_image) {
    delete reinterpret_cast<CLBinary *>(p_image);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_CLImage__
 * Method:    deployImage
 * Signature: (J[J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_CLImage_00024_deployImage
        (JNIEnv *env, jobject obj, jlong p_image, jlongArray jdevices) {
    CLBinary *image = reinterpret_cast<CLBinary *>(p_image);
    auto num_devices = env->GetArrayLength(jdevices);
    jlong *devices = env->GetLongArrayElements(jdevices, JNI_FALSE);
    bool result = image->deploy(reinterpret_cast<cl_device_id const *>(devices), num_devices);
    env->ReleaseLongArrayElements(jdevices, devices, 0);
    if (result) {
        return JNI_TRUE;
    }
    return JNI_FALSE;
}