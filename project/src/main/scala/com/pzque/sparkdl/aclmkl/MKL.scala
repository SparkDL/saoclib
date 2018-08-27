package com.pzque.sparkdl.aclmkl

import java.io.{File, FileOutputStream}
import java.io.File.createTempFile
import java.nio.channels.Channels.newChannel

object MKL {

  private var isLoaded: Boolean = false

  def isMKLLoaded: Boolean = isLoaded

  try {
    val aoclmklFileName: String = "libaclmkl.so"
//    val tmpFile = extract(aoclmklFileName)
//    System.load(tmpFile.getAbsolutePath)
    System.load("/home/pcz/develop/saoclib/project/target/classes/libaclmkl.so")
    init()
    isLoaded = true;
  } catch {
    case e: Exception => {
      isLoaded = false
      e.printStackTrace()
      // TODO: Add an argument for user, continuing to run even if MKL load failed.
      throw new RuntimeException("Failed to load MKL")
    }
  }

  // Extract so file from jar to a temp path
  private def extract(path: String): File = {
    try {
      val url = MKL.getClass.getResource("/" + path)
      if (url == null) {
        throw new Error("Can't find dynamic lib file in jar, path = " + path)
      }
      val in = MKL.getClass.getResourceAsStream("/" + path)
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

  @native def init(): Unit

  @native def vsAdd(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdAdd(n: Int, a: Array[Double], aOffset: Int, b: Array[Double], bOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsSub(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdSub(n: Int, a: Array[Double], aOffset: Int, b: Array[Double], bOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsMul(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdMul(n: Int, a: Array[Double], aOffset: Int, b: Array[Double], bOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsDiv(n: Int, a: Array[Float], aOffset: Int, b: Array[Float], bOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdDiv(n: Int, a: Array[Double], aOffset: Int, b: Array[Double], bOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsPowx(n: Int, a: Array[Float], aOffset: Int, b: Float, y: Array[Float], yOffset: Int): Unit

  @native def vdPowx(n: Int, a: Array[Double], aOffset: Int, b: Double, y: Array[Double], yOffset: Int): Unit

  @native def vsLn(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdLn(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsExp(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdExp(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsSqrt(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdSqrt(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vdTanh(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsTanh(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vsLog1p(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdLog1p(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsAbs(n: Int, a: Array[Float], aOffset: Int, y: Array[Float], yOffset: Int): Unit

  @native def vdAbs(n: Int, a: Array[Double], aOffset: Int, y: Array[Double], yOffset: Int): Unit

  @native def vsgemm(transa: Char, transb: Char, m: Int, n: Int, k: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, b: Array[Float], bOffset: Int, ldb: Int, beta: Float, c: Array[Float], cOffset: Int, ldc: Int): Unit

  @native def vdgemm(transa: Char, transb: Char, m: Int, n: Int, k: Int, alpha: Double, a: Array[Double], aOffset: Int, lda: Int, b: Array[Double], bOffset: Int, ldb: Int, beta: Double, c: Array[Double], cOffset: Int, ldc: Int): Unit

  @native def vsgemv(trans: Char, m: Int, n: Int, alpha: Float, a: Array[Float], aOffset: Int, lda: Int, x: Array[Float], xOffest: Int, incx: Int, beta: Float, y: Array[Float], yOffest: Int, incy: Int): Unit

  @native def vdgemv(trans: Char, m: Int, n: Int, alpha: Double, a: Array[Double], aOffset: Int, lda: Int, x: Array[Double], xOffest: Int, incx: Int, beta: Double, y: Array[Double], yOffest: Int, incy: Int): Unit

  @native def vsaxpy(n: Int, da: Float, dx: Array[Float], dxOffest: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Unit

  @native def vdaxpy(n: Int, da: Double, dx: Array[Double], dxOffest: Int, incx: Int, dy: Array[Double], dyOffset: Int, incy: Int): Unit

  @native def vsdot(n: Int, dx: Array[Float], dxOffset: Int, incx: Int, dy: Array[Float], dyOffset: Int, incy: Int): Float

  @native def vddot(n: Int, dx: Array[Double], dxOffset: Int, incx: Int, dy: Array[Double], dyOffset: Int, incy: Int): Double

  @native def vsger(m: Int, n: Int, alpha: Float, x: Array[Float], xOffset: Int, incx: Int, y: Array[Float], yOffset: Int, incy: Int, a: Array[Float], aOffset: Int, lda: Int): Unit

  @native def vdger(m: Int, n: Int, alpha: Double, x: Array[Double], xOffset: Int, incx: Int, y: Array[Double], yOffset: Int, incy: Int, a: Array[Double], aOffset: Int, lda: Int): Unit

  @native def vsscal(n: Int, sa: Float, sx: Array[Float], offset: Int, incx: Int): Unit

  @native def vdscal(n: Int, sa: Double, sx: Array[Double], offset: Int, incx: Int): Unit

}
