#include <jni.h>
#include <ACLResourceManager.h>

#ifdef __cplusplus

extern "C" {
#endif
using namespace saoclib;
static const char *binaryPath = "/home/pcz/develop/saoclib/project/target/aclblas";
static ACLResourceManager manager(binaryPath);

/*
 * Class:     com_pzque_sparkdl_mkl_ACLMKL__
 * Method:    allocateAccelerator
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_allocateAccelerator
        (JNIEnv *env, jobject, jstring jmsg) {
    std::string msg("");
    ACLBlasAccelerator *accHandle = NULL;
    bool isOK = manager.allocateAccelerator(&accHandle, msg);
    if (!isOK) {
        jmsg = env->NewStringUTF(msg.c_str());
        accHandle = NULL;
    }
    return reinterpret_cast<jlong>(accHandle);
}


/*
 * Class:     com_pzque_sparkdl_mkl_ACLMKL__
 * Method:    releaseAccelerator
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_releaseAccelerator
        (JNIEnv *env, jobject, jlong accHandle) {
//TODO inplement this function
}

/*
 * Class:     com_pzque_sparkdl_mkl_ACLMKL__
 * Method:    vsAdd
 * Signature: (JI[FI[FI[FI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsAdd
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray b,
         jint bOffset, jfloatArray y, jint yOffset) {

    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_b = (jfloat *) env->GetPrimitiveArrayCritical(b, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsAdd(n, jni_a + aOffset, jni_b + bOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(b, jni_b, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsSub
 * Signature: (I[FI[FI[FI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsSub
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray b,
         jint bOffset, jfloatArray y, jint yOffset) {

    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_b = (jfloat *) env->GetPrimitiveArrayCritical(b, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsSub(n, jni_a + aOffset, jni_b + bOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(b, jni_b, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}

/*
  * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
  * Method:    vsMul
  * Signature: (I[FI[FI[FI)V
  */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsMul
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray b,
         jint bOffset, jfloatArray y, jint yOffset) {

    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_b = (jfloat *) env->GetPrimitiveArrayCritical(b, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsMul(n, jni_a + aOffset, jni_b + bOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(b, jni_b, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsDiv
 * Signature: (I[FI[FI[FI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsDiv
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray b, jint bOffset,
         jfloatArray y, jint yOffset) {

    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_b = (jfloat *) env->GetPrimitiveArrayCritical(b, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsDiv(n, jni_a + aOffset, jni_b + bOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(b, jni_b, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}

/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsPowx
 * Signature: (I[FIF[FI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsPowx
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloat b, jfloatArray y,
         jint yOffset) {

    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsPowx(n, jni_a + aOffset, b, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}

/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsLn
 * Signature: (I[FI[FI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsLn
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray y,
         jint yOffset) {

    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsLn(n, jni_a + aOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsExp
 * Signature: (I[FI[FI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsExp
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray y,
         jint yOffset) {

    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsExp(n, jni_a + aOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsSqrt
 * Signature: (I[FI[FI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsSqrt
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray y,
         jint yOffset) {

    jfloat *jni_a = (float *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_y = (float *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsSqrt(n, jni_a + aOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}



/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsTanh
 * Signature: (I[DI[DI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsTanh
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray y,
         jint yOffset) {

    jfloat *jni_a = (float *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_y = (float *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsTanh(n, jni_a + aOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsLog1p
 * Signature: (I[FI[FI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsLog1p
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset, jfloatArray y,
         jint yOffset) {

    jfloat *jni_a = (float *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_y = (float *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsLog1p(n, jni_a + aOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vdDiv
 * Signature: (I[DI[DI[DI)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsAbs
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray a, jint aOffset,
         jfloatArray y, jint yOffset) {

    jfloat *jni_a = (float *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_y = (float *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->vsAbs(n, jni_a + aOffset, jni_y + yOffset);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
}

/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsgemm
 * Signature: (CCIIIF[FII[FIIF[FII)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsgemm
        (JNIEnv *env, jobject obj, jlong accHandle, jchar transa, jchar transb, jint m, jint n,
         jint k, jfloat alpha, jfloatArray a, jint aOffset, jint lda, jfloatArray b,
         jint bOffset, jint ldb, jfloat beta, jfloatArray c, jint cOffset, jint ldc) {

    jfloat *jni_a = (float *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_b = (float *) env->GetPrimitiveArrayCritical(b, JNI_FALSE);
    jfloat *jni_c = (float *) env->GetPrimitiveArrayCritical(c, JNI_FALSE);

//    int jni_transa, jni_transb;
//    if (transa == 't' || transa == 'T') jni_transa = CblasTrans; else jni_transa = CblasNoTrans;
//    if (transb == 't' || transb == 'T') jni_transb = CblasTrans; else jni_transb = CblasNoTrans;
//
//    cblas_sgemm(CblasColMajor, jni_transa, jni_transb, m, n, k, alpha, jni_a + aOffset, lda,
//                jni_b + bOffset, ldb, beta, jni_c + cOffset, ldc);

    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
    env->ReleasePrimitiveArrayCritical(b, jni_b, 0);
    env->ReleasePrimitiveArrayCritical(c, jni_c, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsgemv
 * Signature: (SIIF[FII[FIIF[FII)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsgemv
        (JNIEnv *env, jobject obj, jlong accHandle, jchar trans, jint m, jint n,
         jfloat alpha, jfloatArray a, jint aOffset, jint lda, jfloatArray x,
         jint xOffset, jint incx, jfloat beta, jfloatArray y, jint yOffset, jint incy) {

    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);
    jfloat *jni_x = (jfloat *) env->GetPrimitiveArrayCritical(x, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

//    int jni_trans;
//    if (trans == 't' || trans == 'T') jni_trans = CblasTrans; else jni_trans = CblasNoTrans;
//    cblas_sgemv(CblasColMajor, jni_trans, m, n, alpha, jni_a + aOffset, lda, jni_x + xOffset, incx,
//                beta, jni_y + yOffset, incy);

    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
    env->ReleasePrimitiveArrayCritical(x, jni_x, 0);
    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsaxpy
 * Signature: (II[FII[FII)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsaxpy
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloat a, jfloatArray x, jint xOffset, jint incx,
         jfloatArray y, jint yOffset, jint incy) {
    jfloat *jni_x = (jfloat *) env->GetPrimitiveArrayCritical(x, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->cblas_saxpy(n, a, jni_x + xOffset, incx, jni_y + yOffset, incy);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(x, jni_x, 0);
}


/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsdot
 * Signature: (I[FII[FII)V
 */
JNIEXPORT float JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsdot
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloatArray x, jint xOffset, jint incx,
         jfloatArray y, jint yOffset, jint incy) {
    jfloat *jni_x = (float *) env->GetPrimitiveArrayCritical(x, JNI_FALSE);
    jfloat *jni_y = (float *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);

    float res = reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->cblas_sdot(n, jni_x + xOffset, incx, jni_y + yOffset, incy);

    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(x, jni_x, 0);
    return res;
}

/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsger
 * Signature: (I[FII[FII)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsger
        (JNIEnv *env, jobject obj, jlong accHandle, jint m, jint n, jfloat alpha, jfloatArray x, jint xOffset,
         jint incx, jfloatArray y, jint yOffset, jint incy, jfloatArray a, jint aOffset, jint lda) {
    jfloat *jni_x = (jfloat *) env->GetPrimitiveArrayCritical(x, JNI_FALSE);
    jfloat *jni_y = (jfloat *) env->GetPrimitiveArrayCritical(y, JNI_FALSE);
    jfloat *jni_a = (jfloat *) env->GetPrimitiveArrayCritical(a, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->cblas_sger(CblasColMajor, m, n, alpha, jni_x + xOffset, incx, jni_y + yOffset, incy, jni_a + aOffset,
                         lda);

    env->ReleasePrimitiveArrayCritical(a, jni_a, 0);
    env->ReleasePrimitiveArrayCritical(y, jni_y, 0);
    env->ReleasePrimitiveArrayCritical(x, jni_x, 0);
}

/*
 * Class:     com_pzque_sparkDl_mkl_ACLMKL_00024
 * Method:    vsscal
 * Signature: (II[FII)V
 */
JNIEXPORT void JNICALL Java_com_pzque_sparkdl_mkl_ACLMKL_00024_vsscal
        (JNIEnv *env, jobject obj, jlong accHandle, jint n, jfloat a, jfloatArray x, jint xOffset, jint incx) {
    jfloat *jni_x = (jfloat *) env->GetPrimitiveArrayCritical(x, JNI_FALSE);

    reinterpret_cast<ACLBlasAccelerator *>(accHandle)
            ->cblas_sscal(n, a, jni_x + xOffset, incx);

    env->ReleasePrimitiveArrayCritical(x, jni_x, 0);
}

#ifdef __cplusplus
}
#endif
