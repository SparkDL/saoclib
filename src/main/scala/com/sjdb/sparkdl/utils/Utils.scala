package com.sjdb.sparkdl.utils

import scala.io.Source

object Utils {
  def gflops(n: Int, timeus: Double): Double = {
    val n3: Double = n.toDouble * n * n
    n3 / (timeus * 1e-6) * 2.0 * 1e-9
  }

  def executeTime(func: () => Unit, name: String, repeat: Int) = {
    val start = System.nanoTime()
    for (i <- 0 until repeat) {
      func.apply()
    }
    val end = System.nanoTime()
    val time: Double = (end - start) * 1e-3 / repeat.toDouble
    println(f"Execute $name $repeat times, average cost: ${time} us.")
    time
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

  def l2error(a: Array[Float], b: Array[Float]): Double = {
    require(a.length == b.length)
    val d = square(diff(a, b))
    val o = square(b)
    val dr = div(d, o).map(x => {
      if (x == Float.NaN) {
        return 0
      }
      x
    })

    Math.sqrt(dr.sum / a.length)
  }

  def printMatrix(name: String, a: Array[Float], rows: Int, cols: Int) = {
    printf("\n%s:\n", name);
    for (i <- 0 until rows) {
      for (j <- 0 until cols) {
        printf("%.4f,", a(rows * j + i));
      }
      printf("\n");
    }
    printf("\n");
  }

  def getKernelTimems(): Double = {
    var sum = 0.0
    Source.fromFile("/tmp/time.txt").getLines.foreach({
      x => {
        val array = x.split(",")
        val kernelTime = array(0).toDouble
        val totalTime = array(1).toDouble
        sum += kernelTime
      }
    })
    sum * 1e-3
  }
}
