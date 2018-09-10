package com.sjdb.sparkdl.mkl

import com.intel.analytics.bigdl.mkl.MKL

trait MKLInterface {
  val MKL_WAIT_POLICY_PASSIVE: Int = MKL.MKL_WAIT_POLICY_PASSIVE
  val MKL_WAIT_POLICY_ACTIVE: Int = MKL.MKL_WAIT_POLICY_ACTIVE

  def getMklNumThreads: Int = MKL.getMklNumThreads

  def getMklBlockTime: Int = MKL.getMklBlockTime

  def getMklDisableFastMM: Boolean = MKL.getMklDisableFastMM

  def getMklWaitPolicy: Int = MKL.getMklWaitPolicy

  def isMKLLoaded: Boolean = MKL.isMKLLoaded

  def setNumThreads(numThreads: Int): Unit = MKL.setNumThreads(numThreads)

  def disableFastMM(): Unit = MKL.disableFastMM()

  def setBlockTime(msec: Int): Unit = MKL.setBlockTime(msec)

  def waitPolicy(mode: Int): Unit = MKL.waitPolicy(mode)

  def vsAdd(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsAdd(n, a, aOffset, b, bOffset, y, yOffset)

  def vdAdd(n: Int, a: Array[Double], aOffset: Int, b: Array[Double], bOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdAdd(n, a, aOffset, b, bOffset, y, yOffset)

  def vsSub(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsSub(n, a, aOffset, b, bOffset, y, yOffset)

  def vdSub(n: Int, a: Array[Double], aOffset: Int, b: Array[Double], bOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdSub(n, a, aOffset, b, bOffset, y, yOffset)

  def vsMul(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsMul(n, a, aOffset, b, bOffset, y, yOffset)

  def vdMul(n: Int, a: Array[Double], aOffset: Int, b: Array[Double], bOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdMul(n, a, aOffset, b, bOffset, y, yOffset)

  def vsDiv(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsDiv(n, a, aOffset, b, bOffset, y, yOffset)

  def vdDiv(n: Int, a: Array[Double], aOffset: Int, b: Array[Double], bOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdDiv(n, a, aOffset, b, bOffset, y, yOffset)

  def vsPowx(n: Int, a: Array[Float], aOffset: Int, b: Float, y: Array[Float], yOffset: Int): Unit
  = MKL.vsPowx(n, a, aOffset, b, y, yOffset)

  def vdPowx(n: Int, a: Array[Double], aOffset: Int, b: Double, y: Array[Double], yOffset: Int): Unit
  = MKL.vdPowx(n, a, aOffset, b, y, yOffset)

  def vsLn(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsLn(n, a, aOffset, y, yOffset)

  def vdLn(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdLn(n, a, aOffset, y, yOffset)

  def vsExp(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsExp(n, a, aOffset, y, yOffset)

  def vdExp(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdExp(n, a, aOffset, y, yOffset)

  def vsSqrt(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsSqrt(n, a, aOffset, y, yOffset)

  def vdSqrt(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdSqrt(n, a, aOffset, y, yOffset)

  def vdTanh(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdTanh(n, a, aOffset, y, yOffset)

  def vsTanh(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsTanh(n, a, aOffset, y, yOffset)

  def vsLog1p(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsLog1p(n, a, aOffset, y, yOffset)

  def vdLog1p(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdLog1p(n, a, aOffset, y, yOffset)

  def vsAbs(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = MKL.vsAbs(n, a, aOffset, y, yOffset)

  def vdAbs(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit
  = MKL.vdAbs(n, a, aOffset, y, yOffset)

  def vsgemm(transa: Char, transb: Char, m: Int, n: Int, k: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, b: Array[Float], bOffset: Int, ldb: Int, beta: Float, c: Array[Float], cOffset: Int, ldc: Int): Unit
  = MKL.vsgemm(transa, transb, m, n, k, alpha, a, aOffset, lda, b, bOffset, ldb, beta, c, cOffset, ldc)

  def vdgemm(transa: Char, transb: Char, m: Int, n: Int, k: Int, alpha: Double, a: Array[Double], aOffset: Int, lda: Int, b: Array[Double], bOffset: Int, ldb: Int, beta: Double, c: Array[Double], cOffset: Int, ldc: Int): Unit
  = MKL.vdgemm(transa, transb, m, n, k, alpha, a, aOffset, lda, b, bOffset, ldb, beta, c, cOffset, ldc)

  def vsgemv(trans: Char, m: Int, n: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, x: Array[Float], xOffest: Int, incx: Int, beta: Float, y: Array[Float], yOffest: Int, incy: Int): Unit
  = MKL.vsgemv(trans, m, n, alpha, a, aOffset, lda, x, xOffest, incx, beta, y, yOffest, incy)

  def vdgemv(trans: Char, m: Int, n: Int, alpha: Double, a: Array[Double], aOffset: Int, lda: Int, x: Array[Double], xOffest: Int, incx: Int, beta: Double, y: Array[Double], yOffest: Int, incy: Int): Unit
  = MKL.vdgemv(trans, m, n, alpha, a, aOffset, lda, x, xOffest, incx, beta, y, yOffest, incy)

  def vsaxpy(n: Int, da: Float, dx: Array[Float], dxOffest: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Unit
  = MKL.vsaxpy(n, da, dx, dxOffest, incx, dy, dyOffset, incy)

  def vdaxpy(n: Int, da: Double, dx: Array[Double], dxOffest: Int, incx: Int, dy: Array[Double], dyOffset: Int, incy: Int): Unit
  = MKL.vdaxpy(n, da, dx, dxOffest, incx, dy, dyOffset, incy)

  def vsdot(n: Int, dx: Array[Float], dxOffset: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Float
  = MKL.vsdot(n, dx, dxOffset, incx, dy, dyOffset, incy)

  def vddot(n: Int, dx: Array[Double], dxOffset: Int, incx: Int, dy: Array[Double], dyOffset: Int, incy: Int): Double
  = MKL.vddot(n, dx, dxOffset, incx, dy, dyOffset, incy)

  def vsger(m: Int, n: Int, alpha: Float, x: Array[Float], xOffset: Int, incx: Int, y: Array[Float], yOffset: Int, incy: Int, a: Array[Float], aOffset: Int, lda: Int): Unit
  = MKL.vsger(m, n, alpha, x, xOffset, incx, y, yOffset, incy, a, aOffset, lda)

  def vdger(m: Int, n: Int, alpha: Double, x: Array[Double], xOffset: Int, incx: Int, y: Array[Double], yOffset: Int, incy: Int, a: Array[Double], aOffset: Int, lda: Int): Unit
  = MKL.vdger(m, n, alpha, x, xOffset, incx, y, yOffset, incy, a, aOffset, lda)

  def vsscal(n: Int, sa: Float, sx: Array[Float], offset: Int, incx: Int): Unit
  = MKL.vsscal(n, sa, sx, offset, incx)

  def vdscal(n: Int, sa: Double, sx: Array[Double], offset: Int, incx: Int): Unit
  = MKL.vdscal(n, sa, sx, offset, incx)

  /**
    * Get the worker pool size of current JVM thread. Note different JVM thread has separated MKL worker pool.
    *
    * @return
    */
  def getNumThreads: Int = MKL.getNumThreads
}