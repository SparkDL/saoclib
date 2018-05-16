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
 * Method:    newInstance
 * Signature: (I[I[IJJLjava/lang/String;[Ljava/lang/Object;)J
 */
// TODO: to complete the kernel construction
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel_00024_newInstance
        (JNIEnv *env,
         jobject obj,
         jint work_dim,
         jlongArray global_work_size_list_jarray,
         jlongArray local_work_size_list_jarray,
         jlong image_handle,
         jlong device_handle,
         jstring kernel_name_jstring,
         jobjectArray arg_limits_jarray) {
    assert(image_handle != 0
           && "image_handle cannot be a null pointer");
    assert(device_handle != 0
           && "device_handle cannot be a null pointer");

    /* fetch global work size and local work size*/
    scoped_array<jlong> global_work_size_list;
    scoped_array<jlong> local_work_size_list;

    // global work size list cannot be null
    assert(global_work_size_list_jarray != NULL
           && "global work size list cannot be null");
    jsize global_work_size_list_length = env->GetArrayLength(global_work_size_list_jarray);
    // global work size list must be of 'work_dim' size
    assert(work_dim == global_work_size_list_length
           && "the array of global work size must be of 'work_dim' size");
    // copy elements from jarray to the scoped_array
    global_work_size_list.reset(work_dim);
    env->GetLongArrayRegion(global_work_size_list_jarray, 0, work_dim, global_work_size_list.get());

    // local work size list could be null
    if (local_work_size_list_jarray != NULL) {
        jsize local_work_size_length = env->GetArrayLength(local_work_size_list_jarray);
        assert(work_dim == local_work_size_length
               && "if local work size is not null, it must be of 'work_dim' size");
        // copy elements from jarray to the scoped_array
        local_work_size_list.reset(work_dim);
        env->GetLongArrayRegion(local_work_size_list_jarray, 0, work_dim, local_work_size_list.get());
    }

    /* fetch arg limits*/
    assert(arg_limits_jarray != NULL
           && "arg limits cannot be null");
    jsize num_args = env->GetArrayLength(arg_limits_jarray);
    scoped_array<KernelArgLimit> arg_limits(num_args);
    for (jsize i = 0; i < num_args; i++) {
        jobject arg_limit_jobject = env->GetObjectArrayElement(arg_limits_jarray, 0);
        arg_limits[i] = makeArgLimit(env, arg_limit_jobject);
    }

    /* fetch kernel name */
    const char *kernel_name_cstr = env->GetStringUTFChars(kernel_name_jstring, NULL);
    std::string kernel_name(kernel_name_cstr);
    env->ReleaseStringUTFChars(kernel_name_jstring, kernel_name_cstr);

    /* construct object */
    auto image = reinterpret_cast<ClImage *>(image_handle);
    auto device = reinterpret_cast<cl_device_id>(device_handle);
    return reinterpret_cast<jlong>(new NDRangeKernel(work_dim,
                                                     global_work_size_list.get(),
                                                     local_work_size_list.get(),
                                                     image,
                                                     device,
                                                     kernel_name,
                                                     arg_limits.get(),
                                                     num_args));
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