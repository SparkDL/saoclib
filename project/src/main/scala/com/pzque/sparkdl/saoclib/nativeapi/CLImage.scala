package com.pzque.sparkdl.saoclib.nativeapi

object CLImage {
  type PtrClImage = Long

  @native def newInstance(handle: Long, imagePath: String): Long

  @native def destroyInstance(handle: Long): Unit

  @native def deployImage(handle: Long, devices: Array[Long]): Boolean
}