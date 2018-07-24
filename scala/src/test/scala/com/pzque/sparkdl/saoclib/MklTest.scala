package com.pzque.sparkdl.saoclib

import com.intel.analytics.bigdl.mkl.MKL

object MklTest {
  def main(args: Array[String]): Unit = {
    val a = Array(1f, 1f, 1f, 1f, 1f, 1f)
    val b = Array(2f, 2f, 2f, 2f, 1f, 1f)
    val res = MKL.vsdot(a.length, a, 0, 2, b, 0, 2)
    println(res)
  }
}
