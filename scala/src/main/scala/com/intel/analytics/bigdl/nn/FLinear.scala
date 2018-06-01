package com.intel.analytics.bigdl.nn

import com.intel.analytics.bigdl.tensor.{Tensor, TensorKernel, TensorMath}
import com.intel.analytics.bigdl.tensor.TensorNumericMath.TensorNumeric
import com.pzque.sparkdl.saoclib._

import scala.reflect.ClassTag

class FLinear[T <: AnyVal : ClassTag]
(inputSize: Int,
 outputSize: Int,
 withBias: Boolean = true
)(implicit ev: TensorNumeric[T],tm:NativeTypeMapping[T]) extends Linear[T](inputSize, outputSize, withBias) {


  override def updateOutput(input: Tensor[T]): Tensor[T] = {
    require(input.dim() == 1 || input.dim() == 2,
      "Linear: " + ErrorInfo.constrainInputAsVectorOrBatch +
        s"input dim ${input.dim()}")

    if (input.dim() == 1) {
      output.resize(Array(outputSize))
      if (withBias) output.copy(bias) else output.zero()
      output.addmv(ev.fromType[Int](1), weight, input)
    }
    else if (input.dim() == 2) {
      val nFrame = input.size(1)
      val nElement = output.nElement
      val t = Array(nFrame, weight.size(1))
      output.resize(t)
      if (output.nElement() != nElement) {
        output.zero()
      }

      if (addBuffer.nElement() != nFrame) {
        addBuffer.resize(Array(nFrame)).fill(ev.one)
      }

      FMath.matrixMult(input, weight.t(), output)
      println(s"gg:\n$output")
      if (withBias) output.addr(ev.one, addBuffer, bias)
    }
    output
  }

}
