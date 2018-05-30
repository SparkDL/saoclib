package com.pzque.sparkdl.saoclib

/**
  * this trait is used to map Scala value type to native C type
  * @tparam T
  */
trait NativeTypeMapping[T <: AnyVal] {
  def getNativeType(isArray: Boolean = false, arrayLength: Long = 0): NativeType
}

object NativeTypeMapping {

  implicit object ByteMapping extends NativeTypeMapping[Byte] {
    override def getNativeType(isArray: Boolean = false, arrayLength: Long = 0): NativeType = c_byte
  }

  implicit object ShortMapping extends NativeTypeMapping[Short] {
    override def getNativeType(isArray: Boolean = false, arrayLength: Long = 0): NativeType = c_short
  }

  implicit object IntMapping extends NativeTypeMapping[Int] {
    override def getNativeType(isArray: Boolean = false, arrayLength: Long = 0): NativeType = c_int
  }

  implicit object LongMapping extends NativeTypeMapping[Long] {
    override def getNativeType(isArray: Boolean = false, arrayLength: Long = 0): NativeType = c_long
  }

  implicit object FloatMapping extends NativeTypeMapping[Float] {
    override def getNativeType(isArray: Boolean = false, arrayLength: Long = 0): NativeType = c_float
  }

  implicit object DoubleMapping extends NativeTypeMapping[Double] {
    override def getNativeType(isArray: Boolean = false, arrayLength: Long = 0): NativeType = c_double
  }

}