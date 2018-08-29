package com.pzque.sparkdl.mkl

import java.io.{File, FileOutputStream}
import java.io.File.createTempFile
import java.nio.channels.Channels.newChannel
import java.util.concurrent.atomic.AtomicBoolean

class ACLMKL private(accHandleV: Long) extends MKLInterface {
  private var accHandle: Long = accHandleV


  override def finalize(): Unit = {
    println("finalizing=================")
    ACLMKL.releaseAccelerator(accHandle)
  }

  override def cleanup: Unit = ACLMKL.releaseAccelerator(accHandle)

  override def isMKLLoaded: Boolean = true

  override def vsAdd(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsAdd(accHandle, n, a, aOffset, b, bOffset, y, yOffset)

  override def vsSub(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsSub(accHandle, n, a, aOffset, b, bOffset, y, yOffset)

  override def vsMul(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsMul(accHandle, n, a, aOffset, b, bOffset, y, yOffset)

  override def vsDiv(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsDiv(accHandle, n, a, aOffset, b, bOffset, y, yOffset)

  override def vsPowx(n: Int, a: Array[Float], aOffset: Int, b: Float, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsPowx(accHandle, n, a, aOffset, b, y, yOffset)

  override def vsLn(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsLn(accHandle, n, a, aOffset, y, yOffset)

  override def vsExp(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsExp(accHandle, n, a, aOffset, y, yOffset)

  override def vsSqrt(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsSqrt(accHandle, n, a, aOffset, y, yOffset)

  override def vsTanh(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsTanh(accHandle, n, a, aOffset, y, yOffset)

  override def vsLog1p(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsLog1p(accHandle, n, a, aOffset, y, yOffset)

  override def vsAbs(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit
  = ACLMKL.vsAbs(accHandle, n, a, aOffset, y, yOffset)

  override def vsgemm(transa: Char, transb: Char, m: Int, n: Int, k: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, b: Array[Float], bOffset: Int, ldb: Int, beta: Float, c: Array[Float], cOffset: Int, ldc: Int): Unit
  = ACLMKL.vsgemm(accHandle, transa, transb, m, n, k, alpha, a, aOffset, lda, b, bOffset, ldb, beta, c, cOffset, ldc)

  override def vsgemv(trans: Char, m: Int, n: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, x: Array[Float], xOffest: Int, incx: Int, beta: Float, y: Array[Float], yOffest: Int, incy: Int): Unit
  = ACLMKL.vsgemv(accHandle, trans, m, n, alpha, a, aOffset, lda, x, xOffest, incx, beta, y, yOffest, incy)

  override def vsaxpy(n: Int, da: Float, dx: Array[Float], dxOffest: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Unit
  = ACLMKL.vsaxpy(accHandle, n, da, dx, dxOffest, incx, dy, dyOffset, incy)

  override def vsdot(n: Int, dx: Array[Float], dxOffset: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Float
  = ACLMKL.vsdot(accHandle, n, dx, dxOffset, incx, dy, dyOffset, incy)

  override def vsger(m: Int, n: Int, alpha: Float, x: Array[Float], xOffset: Int, incx: Int, y: Array[Float], yOffset: Int, incy: Int, a: Array[Float], aOffset: Int, lda: Int): Unit
  = ACLMKL.vsger(accHandle, m, n, alpha, x, xOffset, incx, y, yOffset, incy, a, aOffset, lda)

  override def vsscal(n: Int, sa: Float, sx: Array[Float], offset: Int, incx: Int): Unit
  = ACLMKL.vsscal(accHandle, n, sa, sx, offset, incx)
}

object ACLMKL {
  try {
    val aoclmklFileName: String = "libaclmkl.so"
    //    val tmpFile = extract(aoclmklFileName)
    //    System.load(tmpFile.getAbsolutePath)
    System.load("/home/pcz/develop/saoclib/project/target/classes/libaclmkl.so")
  } catch {
    case e: Exception => {
      e.printStackTrace()
      // TODO: Add an argument for user, continuing to run even if MKL load failed.
      throw new RuntimeException("Failed to load MKL")
    }
  }

  def getInstance(): Either[String, ACLMKL] = {
    var msg: String = ""
    val accHandle = allocateAccelerator(msg)
    if (accHandle == 0) {
      return Left(msg)
    }
    Right(new ACLMKL(accHandle))
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

  @native def allocateAccelerator(msg: String): Long

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