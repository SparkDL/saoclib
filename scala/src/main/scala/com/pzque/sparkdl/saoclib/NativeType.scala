package com.pzque.sparkdl.saoclib

object NativeSize {
  @native def sizeof_c_int: Long

  @native def sizeof_c_unsigned: Long

  @native def sizeof_c_float: Long

  @native def sizeof_c_double: Long
}

trait NativeType {
  def size: Long
}

object c_void extends NativeType {
  val _size: Long = 0

  override def size: Long = _size

  override def toString: String = "void"
}

object c_int extends NativeType {
  val _size: Long = NativeSize.sizeof_c_int

  override def size: Long = _size

  override def toString: String = "int"
}

object c_unsigned extends NativeType {
  val _size: Long = NativeSize.sizeof_c_unsigned

  override def size: Long = _size

  override def toString: String = "unsigned"
}

object c_float extends NativeType {
  val _size: Long = NativeSize.sizeof_c_float

  override def size: Long = _size

  override def toString: String = "float"
}

object c_double extends NativeType {
  val _size: Long = NativeSize.sizeof_c_double

  override def size: Long = _size

  override def toString: String = "double"
}

case class c_array(elemType: NativeType, length: Long) extends NativeType {
  override def size: Long = this.elemType.size * this.length

  override def toString: String = s"array[$elemType]"
}


