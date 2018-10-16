package com.sjdb.sparkdl.utils

object Utils {
  def executeTime(func: () => Unit, name: String, repeat: Int) = {
    val start = System.currentTimeMillis()
    for (i <- 0 until repeat) {
      func.apply()
    }
    val end = System.currentTimeMillis()
    println(f"Execute $name $repeat times, average cost: ${(end - start) / repeat.toFloat} ms.")
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
}
