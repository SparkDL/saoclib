package com.intel.analytics.bigdl.nn

;

import com.intel.analytics.bigdl.nn.abstractnn.{AbstractModule, Activity, TensorModule}
import com.intel.analytics.bigdl.tensor.Tensor
import com.intel.analytics.bigdl.tensor.TensorNumericMath.TensorNumeric
import com.intel.analytics.bigdl.utils.RandomGenerator._
import com.intel.analytics.bigdl.utils.Table
import com.intel.analytics.bigdl.optim.Regularizer

import scala.reflect.ClassTag

@SerialVersionUID(5237686508074490666L)
class RnnCellDS[T: ClassTag](
                              inputSize: Int = 4,
                              hiddenSize: Int = 3,
                              activation: TensorModule[T],
                              uRegularizer: Regularizer[T] = null)
                            (implicit ev: TensorNumeric[T])
  extends RnnCell[T](inputSize, hiddenSize, activation, uRegularizer = uRegularizer) {

  // preTopology should be null for DeepSpeech model
  preTopology = null

  override def toString(): String = {
    val str = "nn.RnnCellDS"
    str
  }
}

object RnnCellDS {
  def apply[@specialized(Float, Double) T: ClassTag](
                                                      inputSize: Int = 4,
                                                      hiddenSize: Int = 3,
                                                      activation: TensorModule[T],
                                                      uRegularizer: Regularizer[T] = null)
                                                    (implicit ev: TensorNumeric[T]): RnnCellDS[T] = {
    new RnnCellDS[T](inputSize, hiddenSize, activation, uRegularizer)
  }
}