package com.pzque.sparkdl.saoclib

object FImage {
  @native def newFImage(name: String): Long

  @native def getNumDevices(pImage: Long): Long

  @native def initOpenCL(pImage: Long): Boolean
}

class FImage(name: String) extends NativeBackendObject {
  _nativeHandle = FImage.newFImage(name)

  override protected def disposeInternal(): Unit = {
    disposeInternal(_nativeHandle)
  }

  def getNumDevices: Option[Long] = {
    if (isValid) {
      return Some(FImage.getNumDevices(_nativeHandle))
    }
    None
  }

  def initOpenCL(): Option[Boolean] = {
    if (isValid) {
      return Some(FImage.initOpenCL(_nativeHandle))
    }
    None
  }

  @native protected def disposeInternal(handle: Long): Unit
}