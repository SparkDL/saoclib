//
// Created by pcz on 18-5-18.
//

#ifndef SAOCLIB_CPP_JARRAYAPI_H
#define SAOCLIB_CPP_JARRAYAPI_H

#include <jni.h>
#include "AOCLUtils/aocl_utils.h"
#include "KernelArg.h"

using namespace saoclib;

KernelArg *readJArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode, NativeTypeID typeID);

KernelArg *readJByteArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode);

KernelArg *readJShortArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode);

KernelArg *readJIntArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode);

KernelArg *readJLongArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode);

KernelArg *readJFloatArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode);

KernelArg *readJDoubleArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode);


void writeJArray(JNIEnv *env, jobject obj, KernelArg *arg, NativeTypeID typeID);

void writeJByteArray(JNIEnv *env, jobject obj, KernelArg *arg);

void writeJShortArray(JNIEnv *env, jobject obj, KernelArg *arg);

void writeJIntArray(JNIEnv *env, jobject obj, KernelArg *arg);

void writeJLongArray(JNIEnv *env, jobject obj, KernelArg *arg);

void writeJFloatArray(JNIEnv *env, jobject obj, KernelArg *arg);

void writeJDoubleArray(JNIEnv *env, jobject obj, KernelArg *arg);


#endif //SAOCLIB_CPP_JARRAYAPI_H
