package com.intel.analytics.bigdl.nn

import com.intel.analytics.bigdl.tensor.TensorNumericMath.TensorNumeric

import scala.reflect.ClassTag

class FLinear[T: ClassTag]
(inputSize: Int,
 outputSize: Int,
 withBias: Boolean = true
)(implicit ev: TensorNumeric[T]) extends Linear[T](inputSize, outputSize, withBias) {


}
