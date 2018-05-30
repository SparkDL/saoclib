package com.pzque.sparkdl.saoclib

/**
  * this trait is used to map Scala value type to native C type
  *
  * @tparam T
  */
trait NativeTypeMapping[T <: AnyVal] {
  val nativeType: NativeType
}

object NativeTypeMapping {

  implicit object ByteMapping extends NativeTypeMapping[Byte] {
    override val nativeType: NativeType = c_byte
  }

  implicit object ShortMapping extends NativeTypeMapping[Short] {
    override val nativeType: NativeType = c_short
  }

  implicit object IntMapping extends NativeTypeMapping[Int] {
    override val nativeType: NativeType = c_int
  }

  implicit object LongMapping extends NativeTypeMapping[Long] {
    override val nativeType: NativeType = c_long
  }

  implicit object FloatMapping extends NativeTypeMapping[Float] {
    override val nativeType: NativeType = c_float
  }

  implicit object DoubleMapping extends NativeTypeMapping[Double] {
    override val nativeType: NativeType = c_double
  }

}