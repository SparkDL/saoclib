package com.pzque.sparkdl.saoclib

import scala.reflect.ClassTag

sealed abstract class KernelArg(_argLimit: KernelArgLimit)
  extends KernelArgQuery {
  val argLimit: KernelArgLimit = _argLimit

  def getArgValue: AnyRef

  /**
    * Get argument mode id, (input/output/input_output...)
    *
    * @return mode id of this argument
    */
  override def getModeID: Long = argLimit.getModeID

  /**
    * Get argument type id
    *
    * @return type id of this argument
    */
  override def getTypeID: Long = argLimit.getTypeID

  /**
    * Get the element type id
    * For primitive type, it equals to 'getTypeID'
    * For array type, it returns the type id of elements
    *
    * @return element type id of this array type argument
    */
  override def getElemTypeID: Long = argLimit.getElemTypeID

  /**
    * Get total size of the type
    * For array type, it returns element size * array length
    *
    * @return the total size of this argument
    */
  override def getSize: Long = argLimit.getSize

  /**
    * Get the element size of array type
    * For primitive type, it equals to 'getSize'
    * For array type, it returns the size of one element
    *
    * @return primitive type size or the size of one element in array type
    */
  override def getElemSize: Long = argLimit.getElemSize

  /**
    * Get the length of array
    * For primitive type, it returns 0
    * For array type, it returns array length
    *
    * @return 0 or array length
    */
  override def getArrayLength: Long = argLimit.getArrayLength

  override def toString: String = argLimit.toString
}

case class ArgVal[T <: AnyVal : ClassTag : TypeMapping](arg_value: T)(mode: KernelArgMode)
  extends KernelArg(limit(implicitly[TypeMapping[T]].getNativeType(), mode)) {
  val argValue: T = arg_value

  override def getArgValue: Object = arg_value.asInstanceOf[Object]
}


class ArgArray[T <: AnyVal : ClassTag : TypeMapping](arg_value: Array[T])(mode: KernelArgMode)
  extends KernelArg(limit(c_array(implicitly[TypeMapping[T]].getNativeType(), arg_value.length), mode)) {

  def this(size: Int)(mode: KernelArgMode) = this(new Array[T](size))(mode)

  val argValue: Array[T] = arg_value

  def length: Int = this.argValue.length

  def size: Int = this.arg_value.size

  def apply(index: Int): T = this.argValue(index)

  def update(index: Int, x: T) = this.argValue.update(index, x)

  override def getArgValue: AnyRef = argValue

}


object ArgArray {

  // e.g. ArgArray(Array(1,2,3))(mode_input)
  def apply[T <: AnyVal : TypeMapping : ClassTag](argValue: Array[T])(mode: KernelArgMode)
  : ArgArray[T]
  = new ArgArray[T](argValue)(mode)

  // e.g. ArgArray(1,2,3)(mode_input)
  def apply[T <: AnyVal : TypeMapping : ClassTag](elements: T*)(mode: KernelArgMode)
  : ArgArray[T]
  = new ArgArray[T](elements.toArray[T])(mode)

  // e.g ArgArray.fill(3)(0)(mode_input)
  def fill[T <: AnyVal : TypeMapping : ClassTag](size: Int)(fillValue: T)(mode: KernelArgMode)
  : ArgArray[T]
  = new ArgArray[T](Array.fill(size)(fillValue))(mode)
}