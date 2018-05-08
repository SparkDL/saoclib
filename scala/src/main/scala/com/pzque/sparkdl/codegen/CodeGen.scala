package com.pzque.sparkdl.codegen

import com.intel.analytics.bigdl.nn._
import com.intel.analytics.bigdl.nn.abstractnn.{AbstractModule, Activity}

import scala.reflect.ClassTag

object CodeGen {
  def gen[A <: Activity : ClassTag, B <: Activity : ClassTag, T: ClassTag](module: AbstractModule[A, B, T]): Unit = {
    module match {
      case m: Linear[T] => {
        LinearGen.gen(m)
      }
      case m: SpatialConvolution[T] => {
        SpatialConvolutionGen.gen(m)
      }
      case m: Sequential[T] => {
        SequentialGen.gen(m)
      }
      case m => {
        throw new Exception(s"Unsupported layer: ${m}")
      }
    }
  }
}