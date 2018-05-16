package com.pzque.sparkdl.saoclib

trait TypeMapping[T <: AnyVal] {
  def getNativeType(is_array: Boolean = false, array_length: Long = 0): NativeType
}

object TypeMapping {

  implicit object ByteMapping extends TypeMapping[Byte] {
    override def getNativeType(is_array: Boolean = false, array_length: Long = 0): NativeType = c_byte
  }


  implicit object ShortMapping extends TypeMapping[Short] {
    override def getNativeType(is_array: Boolean = false, array_length: Long = 0): NativeType = c_short
  }

  implicit object IntMapping extends TypeMapping[Int] {
    override def getNativeType(is_array: Boolean = false, array_length: Long = 0): NativeType = c_int
  }

  implicit object LongMapping extends TypeMapping[Long] {
    override def getNativeType(is_array: Boolean = false, array_length: Long = 0): NativeType = c_long
  }

  implicit object FloatMapping extends TypeMapping[Float] {
    override def getNativeType(is_array: Boolean = false, array_length: Long = 0): NativeType = c_float
  }

  implicit object DoubleMapping extends TypeMapping[Double] {
    override def getNativeType(is_array: Boolean = false, array_length: Long = 0): NativeType = c_double
  }

}