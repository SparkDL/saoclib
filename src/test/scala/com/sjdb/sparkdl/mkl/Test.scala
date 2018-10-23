package com.sjdb.sparkdl.mkl

import com.intel.analytics.bigdl.mkl.MKL
import com.sjdb.sparkdl.utils.Utils

import scala.util.Random

object Test {
  def main(args: Array[String]): Unit = {
    val m = 4096
    val n = 4096
    val k = 4096
    val alpha = 1f
    val beta = 1f
    val a = Array.fill(m * k)(Random.nextFloat)
    val b = Array.fill(k * n)(Random.nextFloat)
    var c = Array.fill(m * n)(Random.nextFloat)

    var time:Double = 1.0

    ACLMKL.useIntelMKL(false)
    ACLMKL.vsgemm('n', 'n', m, n, k, alpha,
      a, 0, m,
      b, 0, k,
      beta, c, 0, m)
    println(f"Transfer time: ${ACLMKL.getAndRefreshTransferTimems} ms")


    time = Utils.executeTime(
      () => ACLMKL.vsgemm('n', 'n', m, n, k, alpha,
        a, 0, m,
        b, 0, k,
        beta, c, 0, m),
      "aclmkl-sgemm",
      10)

  }
}
