package com.intel.analytics.bigdl.tensor

import scala.reflect.ClassTag
import com.intel.analytics.bigdl.tensor.TensorNumericMath.TensorNumeric
import com.pzque.sparkdl.saoclib._

class TensorKernel[T <: AnyVal : ClassTag : TypeMapping]
(_inputNum: Int, _kernel: Kernel, _outputSize: Array[Int])
(implicit ev: TensorNumeric[T]) {
  val inputNum: Int = _inputNum
  val outputSize: Array[Int] = _outputSize
  val kernel: Kernel = _kernel
  val inputs: Array[Tensor[T]] = new Array[Tensor[T]](inputNum)

  def setInput(i: Int, tensor: Tensor[T]): Unit = {
    inputs(i) = tensor
  }

  def getOutput(): Tensor[T] = {
    val argNum = inputNum + 1
    val args = new Array[ArgArray[T]](argNum)
    for ((t, i) <- inputs.view.zipWithIndex) {
      args(i) = ArgArray(TensorKernel.getStorageArray(t))(modeInput)
    }
    args(argNum - 1) = ArgArray.fill(outputSize.product)(ev.fromType[Int](0))(modeOutput)
    kernel.call(args)
    return TensorKernel.toDenseTensor(args.last.argValue, outputSize)
  }
}

object TensorKernel {
  def getStorageArray[T <: AnyVal : ClassTag : TypeMapping]
  (tensor: Tensor[T])
  (implicit ev: TensorNumeric[T]): Array[T] = {
    return tensor.storage().array()
  }

  def toDenseTensor[T <: AnyVal : ClassTag : TypeMapping]
  (array: Array[T], size: Array[Int])
  (implicit ev: TensorNumeric[T]): Tensor[T] = {
    val expectedElemNum = size.product
    val givenElemNum = array.length
    if (givenElemNum != expectedElemNum) {
      throw new NegativeArraySizeException(s"cannot convert $givenElemNum elements to a ${size.mkString("*")} tensor")
    }
    return new DenseTensor[T](
      storage = new ArrayStorage[T](array),
      storageOffset = 1,
      size = size,
      stride = size2Stride(size))
  }

  def size2Stride(sizes: Array[Int]): Array[Int] = {
    val strides = new Array[Int](sizes.length)
    var jump = 1
    var i = strides.length - 1
    while (i >= 0) {
      strides(i) = jump
      jump = jump * sizes(i)
      i -= 1
    }
    strides
  }
}