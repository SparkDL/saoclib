package com.pzque.sparkd.models

import com.intel.analytics.bigdl._
import com.intel.analytics.bigdl.nn._
import com.intel.analytics.bigdl.numeric.NumericFloat

object LeNet5 {
  def apply(classNum: Int): Module[Float] = {
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

  def graph(classNum: Int): Module[Float] = {
    val input = Reshape(Array(1, 28, 28)).inputs()
    val conv1 = SpatialConvolution(1, 6, 5, 5).setName("conv1_5x5").inputs(input)
    val tanh1 = Tanh().inputs(conv1)
    val pool1 = SpatialMaxPooling(2, 2, 2, 2).inputs(tanh1)
    val tanh2 = Tanh().inputs(pool1)
    val conv2 = SpatialConvolution(6, 12, 5, 5).setName("conv2_5x5").inputs(tanh2)
    val pool2 = SpatialMaxPooling(2, 2, 2, 2).inputs(conv2)
    val reshape = Reshape(Array(12 * 4 * 4)).inputs(pool2)
    val fc1 = Linear(12 * 4 * 4, 100).setName("fc1").inputs(reshape)
    val tanh3 = Tanh().inputs(fc1)
    val fc2 = Linear(100, classNum).setName("fc2").inputs(tanh3)
    val output = LogSoftMax().inputs(fc2)

    Graph(input, output)
  }
}
