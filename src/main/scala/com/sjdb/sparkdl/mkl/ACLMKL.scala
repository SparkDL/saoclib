package com.sjdb.sparkdl.mkl

import java.util.concurrent.atomic.AtomicIntegerArray

import com.intel.analytics.bigdl.mkl.MKL
import com.sjdb.sparkdl.utils.Utils

import scala.collection.mutable.ListBuffer


object ACLMKL extends MKLInterface {
  private var _useIntelMKL = false
  private var executionTimems: Double = 0


  def getAndRefreshTransferTimems: Double = {
    ACLMKLNative.refreshBenchmarkResultFile()
    val kernelTimems = Utils.getKernelTimems()
    val totalTimems = this.executionTimems
    this.executionTimems = 0
    totalTimems - kernelTimems
  }

  /**
    * Use Intel MKL library instead of FPGA functions
    *
    * @param use whether to use IntelMKL library or not
    */
  def useIntelMKL(use: Boolean): Unit = {
    this._useIntelMKL = use
  }

  var msg: Array[String] = Array("");
  private var accelerators: Array[Long] = ACLMKLNative.allocateAccelerators(msg)
  if (accelerators.isEmpty) {
    println(f"Failed to allocate FPGA accelerator for reason: ${msg(0)}\n\tFall back to Intel MKL Library.")
  } else {
    println("Using ACLMKL.")
  }

  private var isBusy: AtomicIntegerArray = new AtomicIntegerArray(Array.fill(accelerators.length)(0))

  private def getAccelerator: (Int, Long) = {
    for (index <- accelerators.indices) {
      if (isBusy.get(index) == 0) {
        lock(index)
        return (index, accelerators(index))
      }
    }
    (-1, 0)
  }

  private def lock(index: Int): Unit = {
    isBusy.set(index, 1)
  }

  private def unlock(index: Int): Unit = {
    if (index < 0) {
      return
    }
    isBusy.set(index, 0)
  }

  override def vsAdd(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsAdd(n, a, aOffset, b, bOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsAdd(accHandle, n, a, aOffset, b, bOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsSub(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsSub(n, a, aOffset, b, bOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsSub(accHandle, n, a, aOffset, b, bOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsMul(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsMul(n, a, aOffset, b, bOffset, y, yOffset);
    //    } else {
    //      ACLMKLNative.vsMul(accHandle, n, a, aOffset, b, bOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsDiv(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsDiv(n, a, aOffset, b, bOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsDiv(accHandle, n, a, aOffset, b, bOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsPowx(n: Int, a: Array[Float], aOffset: Int, b: Float, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsPowx(n, a, aOffset, b, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsPowx(accHandle, n, a, aOffset, b, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsLn(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsLn(n, a, aOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsLn(accHandle, n, a, aOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsExp(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsExp(n, a, aOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsExp(accHandle, n, a, aOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsSqrt(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsSqrt(n, a, aOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsSqrt(accHandle, n, a, aOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsTanh(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsTanh(n, a, aOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsTanh(accHandle, n, a, aOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsLog1p(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsLog1p(n, a, aOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsLog1p(accHandle, n, a, aOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsAbs(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsAbs(n, a, aOffset, y, yOffset)
    //    } else {
    //      ACLMKLNative.vsAbs(accHandle, n, a, aOffset, y, yOffset)
    //    }
    //    unlock(index)
  }

  override def vsgemm(transa: Char, transb: Char, m: Int, n: Int, k: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, b: Array[Float], bOffset: Int, ldb: Int, beta: Float, c: Array[Float], cOffset: Int, ldc: Int): Unit
  = {
    val start = System.nanoTime()
    val (index, accHandle) = getAccelerator
    if (this._useIntelMKL || accHandle == 0) {
      MKL.vsgemm(transa, transb, m, n, k, alpha, a, aOffset, lda, b, bOffset, ldb, beta, c, cOffset, ldc)
    } else {
      ACLMKLNative.vsgemm(accHandle, transa, transb, m, n, k, alpha, a, aOffset, lda, b, bOffset, ldb, beta, c, cOffset, ldc)
      //      var d = c.clone()
      //      MKL.vsgemm(transa, transb, m, n, k, alpha, a, aOffset, lda, b, bOffset, ldb, beta, d, cOffset, ldc)
      //      val l2e = Utils.l2error(c, d)
      //      val diffv = Utils.diff(c, d)
      //      Utils.printMatrix("diff", diffv, m, n)
      //      if (!(l2e < 1e-6)) {
      //        println(f"transa:$transa,transb:$transb,m:$m,n:$n,k:$k")
      //        println(f"alpha:$alpha")
      //        println(f"a:$a,aOffset:$aOffset,lda:$lda")
      //        println(f"b:$b,bOffset:$bOffset,ldb:$ldb")
      //        println(f"beta:$beta")
      //        println(f"c:$c,cOffset:$cOffset,ldc:$ldc")
      //        Utils.printMatrix("diff", diffv, m, n)
      //        throw new RuntimeException(f"Wrong sgemm results: $l2e")
      //      }
    }
    unlock(index)
    val end = System.nanoTime()
    executionTimems += (end - start) * 1e-6
  }

  override def vsgemv(trans: Char, m: Int, n: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, x: Array[Float], xOffest: Int, incx: Int, beta: Float, y: Array[Float], yOffest: Int, incy: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsgemv(trans, m, n, alpha, a, aOffset, lda, x, xOffest, incx, beta, y, yOffest, incy)
    //    } else {
    //      ACLMKLNative.vsgemv(accHandle, trans, m, n, alpha, a, aOffset, lda, x, xOffest, incx, beta, y, yOffest, incy)
    //    }
    //    unlock(index)
  }

  override def vsaxpy(n: Int, da: Float, dx: Array[Float], dxOffest: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsaxpy(n, da, dx, dxOffest, incx, dy, dyOffset, incy)
    //    } else {
    //      ACLMKLNative.vsaxpy(accHandle, n, da, dx, dxOffest, incx, dy, dyOffset, incy)
    //    }
    //    unlock(index)
  }

  override def vsdot(n: Int, dx: Array[Float], dxOffset: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Float
  = {
    MKL.vsdot(n, dx, dxOffset, incx, dy, dyOffset, incy)

    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    //      val result = MKL.vsdot(n, dx, dxOffset, incx, dy, dyOffset, incy)
    //      unlock(index)
    //      return result
    //    } else {
    //      unlock(index)
    //      return ACLMKLNative.vsdot(accHandle, n, dx, dxOffset, incx, dy, dyOffset, incy)
    //    }
  }

  override def vsger(m: Int, n: Int, alpha: Float, x: Array[Float], xOffset: Int, incx: Int, y: Array[Float], yOffset: Int, incy: Int, a: Array[Float], aOffset: Int, lda: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsger(m, n, alpha, x, xOffset, incx, y, yOffset, incy, a, aOffset, lda)
    //    } else {
    //      ACLMKLNative.vsger(accHandle, m, n, alpha, x, xOffset, incx, y, yOffset, incy, a, aOffset, lda)
    //    }
    //    unlock(index)
  }

  override def vsscal(n: Int, sa: Float, sx: Array[Float], offset: Int, incx: Int): Unit
  = {
    //    val (index, accHandle) = getAccelerator
    //    if (accHandle == 0) {
    MKL.vsscal(n, sa, sx, offset, incx)
    //    } else {
    //      ACLMKLNative.vsscal(accHandle, n, sa, sx, offset, incx)
    //    }
    //    unlock(index)
  }
}