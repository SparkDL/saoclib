package com.pzque.sparkdl.saoclib

import nativeapi._

trait NativeType {
  def size: Long

  def id: Long
}

object c_void extends NativeType {
  val _size: Long = 0
  val _id: Long = NativeTypeID.id_c_void

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "void"
}

object c_byte extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_byte
  val _id: Long = NativeTypeID.id_c_byte

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "byte"
}

object c_short extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_short
  val _id: Long = NativeTypeID.id_c_short

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "short"
}

object c_int extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_int
  val _id: Long = NativeTypeID.id_c_int

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "int"
}

object c_long extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_long
  val _id: Long = NativeTypeID.id_c_long

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "long"
}


object c_float extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_float
  val _id: Long = NativeTypeID.id_c_float

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "float"
}

object c_double extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_double
  val _id: Long = NativeTypeID.id_c_double

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "double"
}


case class c_array(elemType: NativeType, length: Long) extends NativeType {
  val _id: Long = NativeTypeID.id_c_array

  def elemSize: Long = this.elemType.size

  override def size: Long = this.elemType.size * this.length

  override def id: Long = _id

  override def toString: String = s"array[$elemType]"
}


