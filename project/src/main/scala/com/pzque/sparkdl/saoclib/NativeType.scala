package com.pzque.sparkdl.saoclib

import nativeapi._

trait NativeType {
  type ScalaType

  def size: Long

  def id: Long

  override def toString: String
}

object c_void extends NativeType {
  val _size: Long = 0
  val _id: Long = NativeTypeID.id_c_void

  override type ScalaType = Unit

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "void"
}

object c_byte extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_byte
  val _id: Long = NativeTypeID.id_c_byte

  override type ScalaType = Byte

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "byte"
}

object c_short extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_short
  val _id: Long = NativeTypeID.id_c_short

  override type ScalaType = Short

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "short"
}

object c_int extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_int
  val _id: Long = NativeTypeID.id_c_int

  override type ScalaType = Int

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "int"
}

object c_long extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_long
  val _id: Long = NativeTypeID.id_c_long

  override type ScalaType = Long

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "long"
}


object c_float extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_float
  val _id: Long = NativeTypeID.id_c_float


  override type ScalaType = Float

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "float"
}

object c_double extends NativeType {
  val _size: Long = NativeTypeSize.sizeof_c_double
  val _id: Long = NativeTypeID.id_c_double

  override type ScalaType = Double

  override def size: Long = _size

  override def id: Long = _id

  override def toString: String = "double"
}


case class c_array(elemType: NativeType, length: Long) extends NativeType {
  val _id: Long = NativeTypeID.id_c_array

  def elemSize: Long = this.elemType.size

  def elemTypeID: Long = this.elemType.id

  override type ScalaType = Array[_ <: AnyVal]

  override def size: Long = this.elemType.size * this.length

  override def id: Long = _id

  override def toString: String = s"$elemType[$length]"
}


