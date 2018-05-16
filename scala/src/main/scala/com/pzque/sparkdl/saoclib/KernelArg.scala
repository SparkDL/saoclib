package com.pzque.sparkdl.saoclib

import scala.reflect.ClassTag

sealed abstract class KernelArg(arg_limit: KernelArgLimit) {
  val argLimit: KernelArgLimit = arg_limit

  def getArgValue: Object

  override def toString: String = argLimit.toString
}

case class ArgVal[T <: AnyVal: TypeMapping](arg_value: T)(mode: KernelArgMode)
  extends KernelArg(limit(implicitly[TypeMapping[T]].getNativeType(), mode)) {
  val argValue: T = arg_value

  override def getArgValue: Object = arg_value.asInstanceOf[Object]
}


class ArgArray[T <: AnyVal : TypeMapping](arg_value: Array[T])(mode: KernelArgMode)
  extends KernelArg(limit(c_array(implicitly[TypeMapping[T]].getNativeType(), arg_value.length), mode)) {
  val argValue: Array[T] = arg_value

  def length: Int = this.argValue.length

  def size: Int = this.arg_value.size

  def apply(index: Int): T = this.argValue(index)

  def update(index: Int, x: T) = this.arg_value.update(index, x)

  override def getArgValue: AnyRef = arg_value
}


object ArgArray {

  // e.g. ArgArray(Array(1,2,3))(mode_input)
  def apply[T <: AnyVal : TypeMapping : ClassTag](args: Array[T])(mode: KernelArgMode)

  = new ArgArray[T](args)(mode)

  // e.g. ArgArray(1,2,3)(mode_input)
  def apply[T <: AnyVal : TypeMapping : ClassTag](values: T*)(mode: KernelArgMode)
  : ArgArray[T]
  = new ArgArray[T](values.toArray[T])(mode)

  // e.g ArgArray.fill(3)(0)(mode_input)
  def fill[T <: AnyVal : TypeMapping : ClassTag](size: Int)(fill_value: T)(mode: KernelArgMode)
  : ArgArray[T]
  = new ArgArray[T](Array.fill(size)(fill_value))(mode)
}