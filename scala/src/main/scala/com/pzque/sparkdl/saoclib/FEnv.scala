package com.pzque.sparkdl.saoclib

object FEnv {
  @native protected def newInstance(): Long

  @native protected def destroyInstance(handle: Long): Unit

  @native def initOpenCL(handle: Long): Boolean

  @native def getNumDevices(handle: Long): Long

  @native def getDeviceIdList(handle: Long): Array[Long]

  @native def getDeviceId(handle: Long, index: Int): Long
}

class FEnv extends NativeBackendObject {
  _nativeHandle = FEnv.newInstance()

  override protected def destroy(): Unit = {
    FEnv.destroyInstance(_nativeHandle)
  }

  def initOpenCL(): Option[Boolean] = {
    if (isValid) {
      return Some(FEnv.initOpenCL(_nativeHandle))
    }
    None
  }

  def getNumDevices: Option[Long] = {
    if (isValid) {
      return Some(FEnv.getNumDevices(_nativeHandle))
    }
    None
  }

  def getDeviceIdList: Option[Array[Long]] = {
    if (isValid) {
      return Some(FEnv.getDeviceIdList(_nativeHandle))
    }
    None
  }

  def getDeviceId(index: Int): Option[Long] = {
    if (isValid) {
      return Some(FEnv.getDeviceId(_nativeHandle, index))
    }
    None
  }

}
