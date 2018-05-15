//
// Created by pcz on 18-5-14.
//
#include "com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel__.h"
#include "NativeTypeID.h"
#include "../src/saoclib.hpp"

using namespace saoclib;

KernelArgLimit makeArgLimit(JNIEnv *env, jobject arg_limit_jobject) {

    // get type id
    jclass arg_limit_cls = env->GetObjectClass(arg_limit_jobject);
    jfieldID fid = env->GetFieldID(arg_limit_cls, "argType", "Lcom/pzque/sparkdl/saoclib/NativeType;");
    assert(fid != NULL && "cannot find id for field 'argType'");
    jobject arg_type_jobject = env->GetObjectField(arg_limit_jobject, fid);

    jclass arg_type_cls = env->GetObjectClass(arg_type_jobject);
    jmethodID mid = env->GetMethodID(arg_type_cls, "id", "()J");
    assert(mid != NULL && "cannot find method 'id()'");
    jlong arg_type_id_raw = env->CallLongMethod(arg_type_jobject, mid);
    auto arg_type_id = NativeTypeID(arg_type_id_raw);

    // get arg mode
    fid = env->GetFieldID(arg_limit_cls, "argModeID", "J");
    assert(fid != NULL && "cannot find id for field 'argModeID'");
    jlong arg_mode_raw = env->GetLongField(arg_limit_jobject, fid);
    auto arg_mode = KernelArgMode(arg_mode_raw);

    switch (arg_type_id) {
        case NativeTypeID::c_void:
            return KernelArgLimit::Void(arg_mode);
        case NativeTypeID::c_byte:
            return KernelArgLimit::Primitive<unsigned char>(arg_mode);
        case NativeTypeID::c_short:
            return KernelArgLimit::Primitive<short>(arg_mode);
        case NativeTypeID::c_int:
            return KernelArgLimit::Primitive<int>(arg_mode);
        case NativeTypeID::c_float:
            return KernelArgLimit::Primitive<float>(arg_mode);
        case NativeTypeID::c_double:
            return KernelArgLimit::Primitive<double>(arg_mode);
        case NativeTypeID::c_array:
            // get element size
            mid = env->GetMethodID(arg_type_cls, "elemSize", "()J");
            assert(mid != NULL && "cannot find method 'elemSize()'");
            jlong elem_size = env->CallLongMethod(arg_type_jobject, mid);
            // get array length
            fid = env->GetFieldID(arg_type_cls, "length", "J");
            assert(fid != NULL && "cannot find field 'length");
            jlong array_length = env->GetLongField(arg_type_jobject, fid);
            return KernelArgLimit::AlignedBuffer(arg_mode, elem_size, array_length);
    }
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel__
 * Method:    test_get_type_id
 * Signature: (Ljava/lang/Object;)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel_00024_test_1get_1type_1id
        (JNIEnv *env, jobject jobj, jobject obj) {
    return 0;
}
/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel__
 * Method:    newInstance
 * Signature: (I[I[IJJLjava/lang/String;[Ljava/lang/Object;)J
 */
// TODO: to complete the kernel construction
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel_00024_newInstance
        (JNIEnv *env, jobject obj,
         jint work_dim, jintArray global_work_size, jintArray local_work_size,
         jlong image_handle, jlong device_handle,
         jstring kernel_name, jobjectArray arg_limits_jarray) {
    jsize n = env->GetArrayLength(arg_limits_jarray);
    scoped_array<KernelArgLimit> arg_limits(n);
    for (jsize i = 0; i < n; i++) {
        jobject arg_limit_jobject = env->GetObjectArrayElement(arg_limits_jarray, 0);
        arg_limits[i] = makeArgLimit(env, arg_limit_jobject);
    }
    for (int i = 0; i < n; i++) {
        arg_limits[i].print();
    }
    return 0;
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel__
 * Method:    destroyInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel_00024_destroyInstance
        (JNIEnv *env, jobject obj, jlong handle) {
    delete reinterpret_cast<NDRangeKernel *>(handle);
}

/*
 * Class:     com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel__
 * Method:    call
 * Signature: (J[Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel_00024_call
        (JNIEnv *env, jobject obj, jlong kernel_handle, jobjectArray args) {

}