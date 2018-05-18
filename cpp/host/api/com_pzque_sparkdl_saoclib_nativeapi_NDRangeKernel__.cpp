//
// Created by pcz on 18-5-14.
//
#include "com_pzque_sparkdl_saoclib_nativeapi_NDRangeKernel__.h"
#include "../src/saoclib.h"

using namespace saoclib;

static KernelArgLimit makeArgLimit(JNIEnv *env, jobject arg_limit_jobject);

static KernelArg *makeArg(JNIEnv *env, jobject arg_jobject);

static KernelArg *makeArgFromJArray(JNIEnv *env,
                                    jobject value_jobject,
                                    KernelArgMode arg_mode,
                                    NativeTypeID elem_type_id);

static jobject fetchArgLimit(JNIEnv *env, jobject arg_jobject);

static jobject fetchArgType(JNIEnv *env, jobject arg_limit_jobject);

static KernelArgMode fetchArgMode(JNIEnv *env, jobject arg_limit_jobject);

static NativeTypeID fetchTypeID(JNIEnv *env, jobject arg_type_jobject);


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
    jlong *global_work_size_list = NULL, *local_work_size_list = NULL;
    // global work size list cannot be null
    assert(global_work_size_list_jarray != NULL
           && "global work size list cannot be null");
    jsize global_work_size_list_length = env->GetArrayLength(global_work_size_list_jarray);
    // global work size list must be of 'work_dim' size
    assert(work_dim == global_work_size_list_length
           && "the array of global work size must be of 'work_dim' size");
    // get the raw array pointer, remember to call ReleaseLongArrayElements method
    global_work_size_list = env->GetLongArrayElements(global_work_size_list_jarray, JNI_FALSE);

    // local work size list could be null
    if (local_work_size_list_jarray != NULL) {
        jsize local_work_size_length = env->GetArrayLength(local_work_size_list_jarray);
        assert(work_dim == local_work_size_length
               && "if local work size is not null, it must be of 'work_dim' size");
        // // get the raw array pointer, remember to call ReleaseLongArrayElements method
        local_work_size_list = env->GetLongArrayElements(local_work_size_list_jarray, JNI_FALSE);
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
    auto ret = reinterpret_cast<jlong>(new NDRangeKernel(work_dim,
                                                         global_work_size_list,
                                                         local_work_size_list,
                                                         image,
                                                         device,
                                                         kernel_name,
                                                         arg_limits.get(),
                                                         num_args));
    if (global_work_size_list) {
        env->ReleaseLongArrayElements(global_work_size_list_jarray, global_work_size_list, 0);
    }
    if (local_work_size_list) {
        env->ReleaseLongArrayElements(local_work_size_list_jarray, local_work_size_list, 0);
    }
    return ret;
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
        (JNIEnv *env, jobject obj, jlong kernel_handle, jobjectArray args_jarray) {
    /* fetch arguments type */
    assert(kernel_handle != 0
           && "kernel_handle cannot be null");
    assert(args_jarray != NULL
           && "args array cannot be null");

    jsize num_args = env->GetArrayLength(args_jarray);
    assert(num_args > 0
           && "args array cannot be empty");
    scoped_array<KernelArg *> args(num_args);
    scoped_array<int> output_index_list(num_args);
    scoped_array<jobject> arg_object;
    for (unsigned i = 0; i < num_args; i++) {
        auto arg_jobject = env->GetObjectArrayElement(args_jarray, i);
        args[i] = makeArg(env, arg_jobject);
    }
}


static KernelArgLimit makeArgLimit(JNIEnv *env, jobject arg_limit_jobject) {
    // get type id
    KernelArgMode arg_mode = fetchArgMode(env, arg_limit_jobject);

    jobject arg_type_jobject = fetchArgType(env, arg_limit_jobject);
    NativeTypeID arg_type_id = fetchTypeID(env, arg_type_jobject);

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
        case NativeTypeID::c_array: {
            // get element size
            jclass arg_type_cls = env->GetObjectClass(arg_type_jobject);
            jmethodID mid = env->GetMethodID(arg_type_cls, "elemSize", "()J");
            assert(mid != NULL && "cannot find method 'elemSize()'");
            jlong elem_size = env->CallLongMethod(arg_type_jobject, mid);
            // get array length
            jfieldID fid = env->GetFieldID(arg_type_cls, "length", "J");
            assert(fid != NULL && "cannot find field 'length");
            jlong array_length = env->GetLongField(arg_type_jobject, fid);
            return KernelArgLimit::AlignedBuffer(arg_mode, elem_size, array_length);
        }
    }
}

static KernelArg *makeArg(JNIEnv *env, jobject arg_jobject) {
    /* get argLimit field */
    jobject arg_limit_jobject = fetchArgLimit(env, arg_jobject);

    /* get type id */
    jobject arg_type_jobject = fetchArgType(env, arg_limit_jobject);
    NativeTypeID arg_type_id = fetchTypeID(env, arg_type_jobject);

    /* get arg mode (input/output/... */
    KernelArgMode arg_mode = fetchArgMode(env, arg_limit_jobject);

    /* get arg value */
    jclass arg_cls = env->GetObjectClass(arg_jobject);
    jmethodID mid = env->GetMethodID(arg_cls, "getArgValue", "()Ljava/lang/Object;");
    jobject value_jobject = env->CallObjectMethod(arg_jobject, mid);

    KernelArg *arg = NULL;
    switch (arg_type_id) {
        case NativeTypeID::c_void: {
            arg = new Void();
            break;
        }
        case NativeTypeID::c_byte: {
            jclass cls = env->FindClass("java/lang/Byte");
            jmethodID getVal = env->GetMethodID(cls, "byteValue", "()B");
            signed char byte_value = env->CallByteMethod(value_jobject, getVal);
            arg = new ArgByte(arg_mode, byte_value);
            break;
        }
        case NativeTypeID::c_short: {
            jclass cls = env->FindClass("java/lang/Short");
            jmethodID getVal = env->GetMethodID(cls, "shortValue", "()S");
            short short_value = env->CallShortMethod(value_jobject, getVal);
            arg = new ArgShort(arg_mode, short_value);
            break;
        }
        case NativeTypeID::c_int: {
            jclass cls = env->FindClass("java/lang/Integer");
            jmethodID getVal = env->GetMethodID(cls, "intValue", "()I");
            int int_value = env->CallIntMethod(value_jobject, getVal);
            arg = new ArgInt(arg_mode, int_value);
            break;
        }
        case NativeTypeID::c_long: {
            jclass cls = env->FindClass("java/lang/Long");
            jmethodID getVal = env->GetMethodID(cls, "longValue", "()J");
            long long_value = env->CallLongMethod(value_jobject, getVal);
            arg = new ArgLong(arg_mode, long_value);
            break;
        }
        case NativeTypeID::c_float: {
            jclass cls = env->FindClass("java/lang/Float");
            jmethodID getVal = env->GetMethodID(cls, "floatValue", "()F");
            float float_value = env->CallFloatMethod(value_jobject, getVal);
            arg = new ArgFloat(arg_mode, float_value);
            break;
        }
        case NativeTypeID::c_double: {
            jclass cls = env->FindClass("java/lang/Double");
            jmethodID getVal = env->GetMethodID(cls, "doubleValue", "()D");
            double double_value = env->CallDoubleMethod(value_jobject, getVal);
            arg = new ArgDouble(arg_mode, double_value);
            break;
        }
        case NativeTypeID::c_array: {
            jmethodID getElemTypeID = env->GetMethodID(arg_cls, "getElemTypeID", "()J");
            assert(getElemTypeID != NULL
                   && "cannot find method 'getElemTypeID'");
            jlong elem_type_id_raw = env->CallLongMethod(arg_jobject, getElemTypeID);
            arg = makeArgFromJArray(env, value_jobject, arg_mode, NativeTypeID(elem_type_id_raw));
            break;
        }

    }

    return arg;
}

static KernelArg *makeArgFromJArray(JNIEnv *env,
                                    jobject value_jobject,
                                    KernelArgMode arg_mode,
                                    NativeTypeID elem_type_id) {
    switch (elem_type_id) {
        case NativeTypeID::c_byte: {
            jbyteArray value_jarray = reinterpret_cast<jbyteArray>(value_jobject);
            jsize array_length = env->GetArrayLength(value_jarray);
            auto *data_container = new scoped_aligned_ptr<jbyte>(array_length);
            env->GetByteArrayRegion(value_jarray, 0, array_length, data_container->get());
            return new ArgBufferByte(arg_mode, data_container, array_length);
            break;
        }
        case NativeTypeID::c_short: {
            jshortArray value_jarray = reinterpret_cast<jshortArray >(value_jobject);
            jsize array_length = env->GetArrayLength(value_jarray);
            auto *data_container = new scoped_aligned_ptr<jshort>(array_length);
            env->GetShortArrayRegion(value_jarray, 0, array_length, data_container->get());
            return new ArgBufferShort(arg_mode, data_container, array_length);
            break;
        }
        case NativeTypeID::c_int: {
            jintArray value_jarray = reinterpret_cast<jintArray>(value_jobject);
            jsize array_length = env->GetArrayLength(value_jarray);
            auto *data_container = new scoped_aligned_ptr<jint>(array_length);
            env->GetIntArrayRegion(value_jarray, 0, array_length, data_container->get());
            return new ArgBufferInt(arg_mode, data_container, array_length);
            break;
        }
        case NativeTypeID::c_long: {
            jlongArray value_jarray = reinterpret_cast<jlongArray>(value_jobject);
            jsize array_length = env->GetArrayLength(value_jarray);
            auto *data_container = new scoped_aligned_ptr<jlong>(array_length);
            env->GetLongArrayRegion(value_jarray, 0, array_length, data_container->get());
            return new ArgBufferLong(arg_mode, data_container, array_length);
            break;
        }
        case NativeTypeID::c_float: {
            jfloatArray value_jarray = reinterpret_cast<jfloatArray>(value_jobject);
            jsize array_length = env->GetArrayLength(value_jarray);
            auto *data_container = new scoped_aligned_ptr<jfloat>(array_length);
            env->GetFloatArrayRegion(value_jarray, 0, array_length, data_container->get());
            return new ArgBufferFloat(arg_mode, data_container, array_length);
            break;
        }
        case NativeTypeID::c_double: {
            jdoubleArray value_jarray = reinterpret_cast<jdoubleArray>(value_jobject);
            jsize array_length = env->GetArrayLength(value_jarray);
            auto *data_container = new scoped_aligned_ptr<jdouble>(array_length);
            env->GetDoubleArrayRegion(value_jarray, 0, array_length, data_container->get());
            return new ArgBufferDouble(arg_mode, data_container, array_length);
            break;
        }
        case NativeTypeID::c_array: {
            assert(false && "Array of array is not supported now.");
            break;
        }
    }

}

static jobject fetchArgLimit(JNIEnv *env, jobject arg_jobject) {
    /* get argLimit field */
    jclass arg_cls = env->GetObjectClass(arg_jobject);
    jfieldID fid = env->GetFieldID(arg_cls, "argLimit", "Lcom/pzque/sparkdl/saoclib/KernelArgLimit;");
    assert(fid != NULL && "cannot find field 'argLimit'");
    return env->GetObjectField(arg_jobject, fid);
}

static jobject fetchArgType(JNIEnv *env, jobject arg_limit_jobject) {
    jclass arg_limit_cls = env->GetObjectClass(arg_limit_jobject);
    jfieldID fid = env->GetFieldID(arg_limit_cls, "argType", "Lcom/pzque/sparkdl/saoclib/NativeType;");
    assert(fid != NULL && "cannot find id for field 'argType'");
    return env->GetObjectField(arg_limit_jobject, fid);
}

static KernelArgMode fetchArgMode(JNIEnv *env, jobject arg_limit_jobject) {
    // get arg mode
    jclass arg_limit_cls = env->GetObjectClass(arg_limit_jobject);
    jmethodID mid = env->GetMethodID(arg_limit_cls, "getModeID", "()J");
    assert(mid != NULL && "cannot find method 'getModeID'");
    jlong arg_mode_raw = env->CallLongMethod(arg_limit_jobject, mid);
    return KernelArgMode(arg_mode_raw);
}

static NativeTypeID fetchTypeID(JNIEnv *env, jobject arg_type_jobject) {
    jclass arg_type_cls = env->GetObjectClass(arg_type_jobject);
    jmethodID mid = env->GetMethodID(arg_type_cls, "id", "()J");
    assert(mid != NULL && "cannot find method 'id()'");
    jlong arg_type_id_raw = env->CallLongMethod(arg_type_jobject, mid);
    return NativeTypeID(arg_type_id_raw);
}