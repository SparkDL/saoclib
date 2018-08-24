package com.pzque.sparkdl.saoclib.nativeapi

object CLEnv {
  type PtrClEnv = Long

  @native def newInstance(): Long

  @native def destroyInstance(handle: Long): Unit

  @native def initOpenCL(handle: Long): Boolean

  @native def getNumDevices(handle: Long): Long

  @native def getDeviceIDList(handle: Long): Array[Long]

  @native def getDeviceID(handle: Long, index: Int): Long
}