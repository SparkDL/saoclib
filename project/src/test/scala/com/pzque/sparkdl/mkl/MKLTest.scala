package com.pzque.sparkdl.mkl

import com.pzque.sparkdl.mkl.ACLMKL
import com.intel.analytics.bigdl.mkl.{MKL => iMKL}

import scala.util.Random

object MKLTest {
  def main(args: Array[String]): Unit = {
    var m = 5000
    var n = 5000
    var alpha = 1f
    var incx = 1
    var incy = 1
    var lda = m
    m += 1
    n += 1
    lda = m
    val x = Array.fill(m * incx)(Random.nextFloat)
    val y = Array.fill(n * incy)(Random.nextFloat)
    var a1 = Array.fill(lda * n)(0f)
    var a2 = Array.fill(lda * n)(0f)

    val mkl = MKLLoader.MKL.get()
    for (i <- 0 to 5) {

      var start: Double = 0
      var end: Double = 0
      start = System.currentTimeMillis()
      mkl.vsger(m, n, alpha, x, 0, incx, y, 0, incy, a1, 0, lda)
      end = System.currentTimeMillis()
      println(f"aclblas cost: ${end - start}ms")

      start = System.currentTimeMillis()
      iMKL.vsger(m, n, alpha, x, 0, incx, y, 0, incy, a2, 0, lda)
      end = System.currentTimeMillis()
      println(f"mkl cost: ${end - start}ms");

      //println(a1.sameElements(a2))
      if (!a1.sameElements(a2)) {
        //println(a1.mkString(","))
        //println(a2.mkString(","))
      }
    }
  }
}
