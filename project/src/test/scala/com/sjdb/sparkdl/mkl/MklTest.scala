package com.sjdb.sparkdl.mkl

import com.intel.analytics.bigdl.mkl.{MKL => iMKL}

object MklTest {
  def main(args: Array[String]): Unit = {
    val a = Array(1f, 2f, 3f, 4f, 5f, 6f, 7f, 8f)
    val b = Array(1f, 1f, 10f, 10f, 100f, 100f, 1000f, 1000f, 10000f, 10000f)
    var c = Array.fill(100)(0f)
    var d = Array.fill(100)(0f)
    ACLMKL.vsger(8, 10, 1f, a, 0, 1, b, 0, 1, c, 0, 10)
    iMKL.vsger(8, 10, 1f, a, 0, 1, b, 0, 1, d, 0, 10)
    println(c.mkString(","))
    println(d.mkString(","))
  }
}
