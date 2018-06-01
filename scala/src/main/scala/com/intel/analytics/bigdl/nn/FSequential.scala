package com.intel.analytics.bigdl.nn

import scala.reflect.ClassTag
import com.intel.analytics.bigdl.nn.abstractnn.{Activity, TensorModule}
import com.intel.analytics.bigdl.tensor.Tensor
import com.intel.analytics.bigdl.tensor.TensorNumericMath.TensorNumeric
import com.pzque.sparkdl.saoclib.Kernel

class FSequential[T: ClassTag]
(implicit ev: TensorNumeric[T]) extends Sequential[T] {
  val errorString = s"FSequential doesn't support this operation now"

  //  override def updateOutput(input: Activity): Activity = {
  //    if(input.isTable){
  //      throw new UnsupportedOperationException("table input is not supported yet")
  //    }
  //    input.toTensor[T].storage().array()
  //
  //  }

}
