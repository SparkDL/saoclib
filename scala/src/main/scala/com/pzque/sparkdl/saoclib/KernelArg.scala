package com.pzque.sparkdl.saoclib

import scala.reflect.ClassTag


abstract class KernelArg[T: ClassTag](arg_limit: KernelArgLimit, value: T) {
  val argLimit: KernelArgLimit = arg_limit

  override def toString: String = argLimit.toString
}

case class ArgInt(value: Int, mode: KernelArgMode)
  extends KernelArg[Int](limit(c_int, mode), value) {
}

case class ArgFloat(value: Float, mode: KernelArgMode)
  extends KernelArg[Float](limit(c_int, mode), value) {

}

case class ArgDouble(value: Double, mode: KernelArgMode)
  extends KernelArg[Double](limit(c_int, mode), value) {

}

case class ArgIntArray(value: Array[Int], length: Long, mode: KernelArgMode)
  extends KernelArg[Array[Int]](limit(c_array(c_int, length), mode), value) {

}

case class ArgFloatArray(value: Array[Float], length: Long, mode: KernelArgMode)
  extends KernelArg[Array[Float]](limit(c_array(c_float, length), mode), value) {

}

case class ArgDoubleArray(value: Array[Double], length: Long, mode: KernelArgMode)
  extends KernelArg[Array[Double]](limit(c_array(c_double, length), mode), value) {

}