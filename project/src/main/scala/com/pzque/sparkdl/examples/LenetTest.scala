package com.pzque.sparkdl.examples

import com.intel.analytics.bigdl.Module
import com.intel.analytics.bigdl.nn._
import com.intel.analytics.bigdl.numeric.NumericFloat
import com.intel.analytics.bigdl.tensor.Tensor
import com.intel.analytics.bigdl.utils.Engine
import org.apache.spark.SparkContext

object LenetTest {
  def main(args: Array[String]): Unit = {
    System.loadLibrary("saoclib")
    // Scala code example
    val conf = Engine.createSparkConf().setAppName("LenetTest").setMaster("local[4]")
    val sc = new SparkContext(conf)
    Engine.init
    val cm = cpu(5)
    val fm = fpga(5)
    fm.loadModelWeights(cm, matchAll = true)
    for (i <- 1 to 100) {
      val input = Tensor(28, 28).randn()
      val expected = cm.forward(input)
      val actual = fm.forward(input)
      require(expected.equals(actual))
      println(s"expected:\n${expected}")
      println(s"actual:\n${actual}")
    }
  }

  def cpu(classNum: Int): Module[Float] = {
    val model = Sequential()
    model.add(Reshape(Array(1, 28, 28)))
      .add(SpatialConvolution(1, 6, 5, 5).setName("conv1_5x5"))
      .add(Tanh())
      .add(SpatialMaxPooling(2, 2, 2, 2))
      .add(Tanh())
      .add(SpatialConvolution(6, 12, 5, 5).setName("conv2_5x5"))
      .add(SpatialMaxPooling(2, 2, 2, 2))
      .add(Reshape(Array(12 * 4 * 4)))
      .add(Linear(12 * 4 * 4, 100).setName("fc1"))
      .add(Tanh())
      .add(Linear(100, classNum).setName("fc2"))
      .add(LogSoftMax())
  }

  def fpga(classNum: Int): Module[Float] = {
    val model = Sequential()
    model.add(Reshape(Array(1, 28, 28)))
      .add(SpatialConvolution(1, 6, 5, 5).setName("conv1_5x5"))
      .add(Tanh())
      .add(SpatialMaxPooling(2, 2, 2, 2))
      .add(Tanh())
      .add(SpatialConvolution(6, 12, 5, 5).setName("conv2_5x5"))
      .add(SpatialMaxPooling(2, 2, 2, 2))
      .add(Reshape(Array(12 * 4 * 4)))
      .add(FpgaLinear(12 * 4 * 4, 100).setName("fc1"))
      .add(Tanh())
      .add(FpgaLinear(100, classNum).setName("fc2"))
      .add(LogSoftMax())
  }
}
