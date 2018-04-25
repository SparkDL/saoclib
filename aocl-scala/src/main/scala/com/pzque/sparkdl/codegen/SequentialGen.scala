package com.pzque.sparkdl.codegen

import com.intel.analytics.bigdl.nn.Sequential

import scala.reflect.ClassTag

object SequentialGen {
  def gen[T: ClassTag](module: Sequential[T]) = {
    module.modules foreach (
      m => {
        CodeGen.gen(m)
      })
  }
}
