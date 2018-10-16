package com.sjdb.sparkdl.mkl

import com.intel.analytics.bigdl.mkl.MKL
import java.io.{File, FileOutputStream}
import java.io.File.createTempFile
import java.nio.channels.Channels.newChannel
import java.util.concurrent.atomic.{AtomicBoolean, AtomicIntegerArray}


object ACLMKLNative {
  try {
    val aoclmklFileName: String = "libaclmkl.so"
    val tmpFile = extract(aoclmklFileName)
    // System.load(tmpFile.getAbsolutePath)
    System.load("/home/pcz/develop/saoclib/src/main/cpp/cmake-build-debug/libaclmkl.so")
  } catch {
    case e: Exception => {
      e.printStackTrace()
      // TODO: Add an argument for user, continuing to run even if MKL load failed.
      throw new RuntimeException("Failed to load MKL")
    }
  }

  // Extract so file from jar to a temp path
  private def extract(path: String): File = {
    try {
      val url = this.getClass.getResource("/" + path)
      if (url == null) {
        throw new Error("Can't find dynamic lib file in jar, path = " + path)
      }
      val in = this.getClass.getResourceAsStream("/" + path)
      val file = createTempFile("dlNativeLoader", path)
      val src = newChannel(in)
      val dest = new FileOutputStream(file).getChannel
      dest.transferFrom(src, 0, Long.MaxValue)
      dest.close()
      src.close()
      return file
    } catch {
      case e: Throwable =>
        throw new Error("Can't extract dynamic lib file to /tmp dir.\n" + e)
    }
  }

  @native def allocateAccelerators(msg: Array[String]): Array[Long]

  @native def releaseAccelerator(accHandle: Long): Boolean

  @native def vsAdd(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsSub(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsMul(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsDiv(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsPowx(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, b: Float, y: Array[Float], yOffset: Int): Unit

  @native def vsLn(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsExp(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsSqrt(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsTanh(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsLog1p(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsAbs(accHandle: Long, n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsgemm(accHandle: Long, transa: Char, transb: Char, m: Int, n: Int, k: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, b: Array[Float], bOffset: Int, ldb: Int, beta: Float, c: Array[Float], cOffset: Int, ldc: Int): Unit

  @native def vsgemv(accHandle: Long, trans: Char, m: Int, n: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, x: Array[Float], xOffest: Int, incx: Int, beta: Float, y: Array[Float], yOffest: Int, incy: Int): Unit

  @native def vsaxpy(accHandle: Long, n: Int, da: Float, dx: Array[Float], dxOffest: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Unit

  @native def vsdot(accHandle: Long, n: Int, dx: Array[Float], dxOffset: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Float

  @native def vsger(accHandle: Long, m: Int, n: Int, alpha: Float, x: Array[Float], xOffset: Int, incx: Int, y: Array[Float], yOffset: Int, incy: Int, a: Array[Float], aOffset: Int, lda: Int): Unit

  @native def vsscal(accHandle: Long, n: Int, sa: Float, sx: Array[Float], offset: Int, incx: Int): Unit
}