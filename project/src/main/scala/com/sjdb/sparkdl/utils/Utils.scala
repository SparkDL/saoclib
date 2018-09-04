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
}
