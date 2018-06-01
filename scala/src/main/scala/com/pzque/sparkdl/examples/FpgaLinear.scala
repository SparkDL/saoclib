package com.pzque.sparkdl.examples

import com.intel.analytics.bigdl.nn.{FLinear, Linear}
import com.intel.analytics.bigdl.tensor.Tensor
import com.intel.analytics.bigdl.utils.T

object FpgaLinear {
  def main(args: Array[String]): Unit = {
    System.loadLibrary("saoclib")
    val weight = Tensor[Float](T(
      T(1f, 2f, 3f),
      T(1f, 2f, 3f),
      T(1f, 2f, 3f),
      T(1f, 2f, 3f)
    ))
    val bias = Tensor[Float](T(0.1f, 0.1f, 0.1f, 0.1f))
    val linear = Linear[Float](3, 4, true)
    val flinear = new FLinear[Float](3, 4, true)

    linear.setWeightsBias(Array(weight, bias))
    flinear.setWeightsBias(Array(weight, bias))
    // x * w.T + b
    val x = Tensor[Float](T(
      T(1f, 2f, 3f),
      T(1f, 2f, 3f)
    ))
    val expected = linear.updateOutput(x)
    val actual = flinear.updateOutput(x)

    println(s"w:\n${weight}")
    println(s"b:\n${bias}")
    println(s"x:\n${x}")
    println(s"expected:\n${expected}\nactual:\n${actual}")
  }
}
