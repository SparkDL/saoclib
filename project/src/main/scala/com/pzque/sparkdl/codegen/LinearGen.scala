package com.pzque.sparkdl.codegen

import com.intel.analytics.bigdl.nn.Linear
import scala.io.Source
import scala.reflect.ClassTag

object LinearGen {
  val kernelPath = "linear.cl"

  def gen[T: ClassTag](module: Linear[T]): Unit = {
    val kernelURI = getClass.getResource(kernelPath).toURI
    Source.fromFile(kernelURI).getLines().foreach {
      println(_)
    }
    //val lines = scala.io.Source.fromInputStream( stream ).getLines
  }
}