package com.sjdb.sparkdl.mkl

import com.intel.analytics.bigdl.mkl.{MKL => iMKL}

import scala.util.Random

object MKLTest {
  def main(args: Array[String]): Unit = {
    val transa: Char = 't'
    val transb: Char = 'n'
    val m = 4096
    val n = 4096
    val k = 1152
    val lda = m
    val ldb = k
    val ldc = m
    val alpha = 1f
    val beta = 0f
    val aOffset = 0
    val bOffset = 0
    val cOffset = 0

    val a = Array.fill(m * k)(Random.nextFloat())
    val b = Array.fill(k * n)(Random.nextFloat())
    var c1 = Array.fill(m * n)(0f)
    var c2 = Array.fill(m * n)(0f)

    iMKL.vsgemm(transa, transb,
      m, n, k,
      alpha,
      a, aOffset, lda,
      a, bOffset, ldb,
      beta,
      c1, cOffset, ldc
    )

    ACLMKL.vsgemm(transa, transb,
      m, n, k,
      alpha,
      a, aOffset, lda,
      a, bOffset, ldb,
      beta,
      c2, cOffset, ldc
    )

    println(s"l2error:${l2error(c2, c1)}")
  }

  def div(a: Array[Float], b: Array[Float]) = {
    require(a.length == b.length)
    a.zip(b).map(x => x._1 / x._2)
  }

  def diff(a: Array[Float], b: Array[Float]) = {
    require(a.length == b.length)
    a.zip(b).map(x => x._1 - x._2)
  }

  def square(a: Array[Float]) = {
    a.map(x => x * x)
  }

  def l2error(a: Array[Float], b: Array[Float]) = {
    require(a.length == b.length)
    val d = square(diff(a, b))
    val o = square(b)
    Math.sqrt(div(d, o).sum / a.length)
  }

  def printMatrix(name: String, a: Array[Float], rows: Int, cols: Int): Unit = {
    println(f"${name}:")
    for (i <- 0 until rows) {
      for (j <- 0 until cols) {
        print(a(rows * j + i))
        print(",")
      }
      println()
    }
    println()
  }
}
