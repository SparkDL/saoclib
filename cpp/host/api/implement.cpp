//
// Created by pcz on 18-5-10.
//

#include "com_pzque_sparkdl_saoclib_FImage__.h"
#include "../src/FImage.hpp"

using namespace saoclib;

JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_FImage_00024_newFImage
        (JNIEnv *env, jobject obj, jstring name_jstr) {
    const char *name_cstr = env->GetStringUTFChars(name_jstr, NULL);
    std::string name = std::string(name_cstr);
    env->ReleaseStringUTFChars(name_jstr, name_cstr);
    return reinterpret_cast<jlong>(new saoclib::FImage(name));
}


JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_FImage_00024_getNumDevices
        (JNIEnv *env, jobject obj, jlong pImage) {
    auto cpImage = reinterpret_cast<FImage *>(pImage);
    return cpImage->getNumDevices();
}


JNIEXPORT jboolean JNICALL Java_com_pzque_sparkdl_saoclib_FImage_00024_initOpenCL
        (JNIEnv *env, jobject obj, jlong pImage) {
    auto cpImage = reinterpret_cast<FImage *>(pImage);
    bool result = cpImage->init_opencl();
    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_FImage_00024_disposeInternal
        (JNIEnv *env, jobject obj, jlong ptr) {
    delete (saoclib::FImage *) ptr;
}

void cleanup() {}
