package com.pzque.sparkdl.saoclib

abstract class KernelArg(arg_limit: KernelArgLimit, value: Object) {
  val argLimit: KernelArgLimit = arg_limit

  override def toString: String = argLimit.toString
}

case class ArgInt(value: Integer, mode: KernelArgMode)
  extends KernelArg(limit(c_int, mode), value) {
}

case class ArgFloat(value: java.lang.Float, mode: KernelArgMode)
  extends KernelArg(limit(c_int, mode), value) {

}

case class ArgDouble(value: java.lang.Double, mode: KernelArgMode)
  extends KernelArg(limit(c_int, mode), value) {

}

case class ArgIntArray(value: Array[Int], length: Long, mode: KernelArgMode)
  extends KernelArg(limit(c_array(c_int, length), mode), value) {

}

case class ArgFloatArray(value: Array[Float], length: Long, mode: KernelArgMode)
  extends KernelArg(limit(c_array(c_float, length), mode), value) {

}

case class ArgDoubleArray(value: Array[Double], length: Long, mode: KernelArgMode)
  extends KernelArg(limit(c_array(c_double, length), mode), value) {

}