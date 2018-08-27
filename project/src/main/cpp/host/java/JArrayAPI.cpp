//
// Created by pcz on 18-5-18.
//
#include "JArrayAPI.h"

KernelArg *readJArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode, NativeTypeID typeID) {
    switch (typeID) {
        case NativeTypeID::c_byte:
            return readJByteArray(env, obj, arg_mode);
        case NativeTypeID::c_short:
            return readJShortArray(env, obj, arg_mode);
        case NativeTypeID::c_int:
            return readJIntArray(env, obj, arg_mode);
        case NativeTypeID::c_long:
            return readJLongArray(env, obj, arg_mode);
        case NativeTypeID::c_float:
            return readJFloatArray(env, obj, arg_mode);
        case NativeTypeID::c_double:
            return readJDoubleArray(env, obj, arg_mode);
        default: {
            assert(false && "wrong branch");
            return NULL;
        }
    }
}

KernelArg *readJByteArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode) {
    auto data_jarray = reinterpret_cast<jbyteArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    auto *data_container = new scoped_aligned_ptr<jbyte>(array_length);
    if (isInput(arg_mode)) {
        env->GetByteArrayRegion(data_jarray, 0, array_length, data_container->get());
    }
    return new ByteArrayArg(data_container->get(), array_length, arg_mode);
}

KernelArg *readJShortArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode) {
    auto data_jarray = reinterpret_cast<jshortArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    auto *data_container = new scoped_aligned_ptr<jshort>(array_length);
    if (isInput(arg_mode)) {
        env->GetShortArrayRegion(data_jarray, 0, array_length, data_container->get());
    }
    return new ShortArrayArg(data_container->get(), array_length, arg_mode);
}

KernelArg *readJIntArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode) {
    auto data_jarray = reinterpret_cast<jintArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    auto *data_container = new scoped_aligned_ptr<jint>(array_length);
    if (isInput(arg_mode)) {
        env->GetIntArrayRegion(data_jarray, 0, array_length, data_container->get());
    }
    return new IntArrayArg(data_container->get(), array_length, arg_mode);
}

KernelArg *readJLongArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode) {
    auto data_jarray = reinterpret_cast<jlongArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    auto *data_container = new scoped_aligned_ptr<jlong>(array_length);
    if (isInput(arg_mode)) {
        env->GetLongArrayRegion(data_jarray, 0, array_length, data_container->get());
    }
    return new LongArrayArg(data_container->get(), array_length, arg_mode);
}

// TODO note the data_container delete time
KernelArg *readJFloatArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode) {
    auto data_jarray = reinterpret_cast<jfloatArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    auto *data_container = new scoped_aligned_ptr<jfloat>(array_length);
    if (isInput(arg_mode)) {
        env->GetFloatArrayRegion(data_jarray, 0, array_length, data_container->get());
    }
    return new FloatArrayArg(data_container->get(), array_length, arg_mode);
}

KernelArg *readJDoubleArray(JNIEnv *env, jobject obj, KernelArgMode arg_mode) {
    auto data_jarray = reinterpret_cast<jdoubleArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    auto *data_container = new scoped_aligned_ptr<jdouble>(array_length);
    if (isInput(arg_mode)) {
        env->GetDoubleArrayRegion(data_jarray, 0, array_length, data_container->get());
    }
    return new DoubleArrayArg(data_container->get(), array_length, arg_mode);
}

void writeJArray(JNIEnv *env, jobject obj, KernelArg *arg, NativeTypeID typeID) {
    switch (typeID) {
        case NativeTypeID::c_byte:
            return writeJByteArray(env, obj, arg);
        case NativeTypeID::c_short:
            return writeJShortArray(env, obj, arg);
        case NativeTypeID::c_int:
            return writeJIntArray(env, obj, arg);
        case NativeTypeID::c_long:
            return writeJLongArray(env, obj, arg);
        case NativeTypeID::c_float:
            return writeJFloatArray(env, obj, arg);
        case NativeTypeID::c_double:
            return writeJDoubleArray(env, obj, arg);
        default: {
            assert(false && "wrong branch");
        }
    }
}

void writeJByteArray(JNIEnv *env, jobject obj, KernelArg *arg) {
    auto data_jarray = reinterpret_cast<jbyteArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    env->SetByteArrayRegion(data_jarray, 0, array_length, (jbyte *) arg->getWriteableDataPtr());
}

void writeJShortArray(JNIEnv *env, jobject obj, KernelArg *arg) {
    auto data_jarray = reinterpret_cast<jshortArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    env->SetShortArrayRegion(data_jarray, 0, array_length, (jshort *) arg->getWriteableDataPtr());
}

void writeJIntArray(JNIEnv *env, jobject obj, KernelArg *arg) {
    auto data_jarray = reinterpret_cast<jintArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    env->SetIntArrayRegion(data_jarray, 0, array_length, (jint *) arg->getWriteableDataPtr());
}

void writeJLongArray(JNIEnv *env, jobject obj, KernelArg *arg) {
    auto data_jarray = reinterpret_cast<jlongArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    env->SetLongArrayRegion(data_jarray, 0, array_length, (jlong *) arg->getWriteableDataPtr());
}

void writeJFloatArray(JNIEnv *env, jobject obj, KernelArg *arg) {
    auto data_jarray = reinterpret_cast<jfloatArray>(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    env->SetFloatArrayRegion(data_jarray, 0, array_length, (jfloat *) arg->getWriteableDataPtr());
}

void writeJDoubleArray(JNIEnv *env, jobject obj, KernelArg *arg) {
    auto data_jarray = reinterpret_cast<jdoubleArray >(obj);
    jsize array_length = env->GetArrayLength(data_jarray);
    env->SetDoubleArrayRegion(data_jarray, 0, array_length, (jdouble *) arg->getWriteableDataPtr());
}


