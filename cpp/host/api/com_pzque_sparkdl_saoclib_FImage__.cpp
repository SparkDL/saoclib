//
// Created by pcz on 18-5-11.
//

#include "com_pzque_sparkdl_saoclib_FImage__.h"
#include "../src/FImage.hpp"

using namespace saoclib;
/*
 * Class:     com_pzque_sparkdl_saoclib_FImage__
 * Method:    newInstance
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_FImage_00024_newInstance
        (JNIEnv *env, jobject obj, jlong penv, jstring jname) {
    auto *fenv = reinterpret_cast<FEnv *>(penv);
    const char *name_chars = env->GetStringUTFChars(jname, NULL);
    std::string name(name_chars);
    env->ReleaseStringUTFChars(jname, name_chars);
    return reinterpret_cast<jlong>(new FImage(fenv, name));
}

/*
 * Class:     com_pzque_sparkdl_saoclib_FImage__
 * Method:    destroyInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_FImage_00024_destroyInstance
        (JNIEnv *env, jobject obj, jlong pfimage) {
    delete reinterpret_cast<FImage *>(pfimage);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_FImage__
 * Method:    deployImage
 * Signature: (J[J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pzque_sparkdl_saoclib_FImage_00024_deployImage
        (JNIEnv *env, jobject obj, jlong pfimage, jlongArray jdevices) {
    FImage *fimage = reinterpret_cast<FImage *>(pfimage);
    auto num_devices = env->GetArrayLength(jdevices);
    scoped_array<cl_device_id> devices(num_devices);
    env->GetLongArrayRegion(jdevices, 0, num_devices, (jlong *) devices.get());
    bool result = fimage->deploy_image(devices.get(), num_devices);
    if (result) {
        return JNI_TRUE;
    }
    return JNI_FALSE;
}