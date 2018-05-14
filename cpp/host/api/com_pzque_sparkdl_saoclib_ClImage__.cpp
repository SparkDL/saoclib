//
// Created by pcz on 18-5-11.
//

#include "com_pzque_sparkdl_saoclib_ClImage__.h"
#include "../src/ClImage.hpp"

using namespace saoclib;
/*
 * Class:     com_pzque_sparkdl_saoclib_ClImage__
 * Method:    newInstance
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_ClImage_00024_newInstance
        (JNIEnv *env, jobject obj, jlong p_env, jstring jname) {
    auto *cl_env = reinterpret_cast<ClEnv *>(p_env);
    const char *name_chars = env->GetStringUTFChars(jname, NULL);
    std::string name(name_chars);
    env->ReleaseStringUTFChars(jname, name_chars);
    return reinterpret_cast<jlong>(new ClImage(cl_env, name));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_ClImage__
 * Method:    destroyInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_ClImage_00024_destroyInstance
        (JNIEnv *env, jobject obj, jlong p_image) {
    delete reinterpret_cast<ClImage *>(p_image);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_ClImage__
 * Method:    deployImage
 * Signature: (J[J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pzque_sparkdl_saoclib_ClImage_00024_deployImage
        (JNIEnv *env, jobject obj, jlong p_image, jlongArray jdevices) {
    ClImage *image = reinterpret_cast<ClImage *>(p_image);
    auto num_devices = env->GetArrayLength(jdevices);
    scoped_array<cl_device_id> devices(num_devices);
    env->GetLongArrayRegion(jdevices, 0, num_devices, (jlong *) devices.get());

    bool result = image->deploy_image(devices.get(), num_devices);
    if (result) {
        return JNI_TRUE;
    }
    return JNI_FALSE;
}