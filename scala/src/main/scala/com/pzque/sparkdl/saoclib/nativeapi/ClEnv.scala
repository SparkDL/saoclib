package com.pzque.sparkdl.saoclib.nativeapi

object ClEnv {
  type PtrClEnv = Long

  @native def newInstance(): Long

  @native def destroyInstance(handle: Long): Unit

  @native def initOpenCL(handle: Long): Boolean

  @native def getNumDevices(handle: Long): Long

  @native def getDeviceIdList(handle: Long): Array[Long]

  @native def getDeviceId(handle: Long, index: Int): Long
}