package com.pzque.sparkdl.saoclib

object FImage {
  @native protected def newFImage(name: String): Long

  @native protected def getNumDevices(pImage: Long): Long

  @native protected def initOpenCL(pImage: Long): Boolean

  @native protected def disposeInternal(handle: Long): Unit
}

class FImage(name: String) extends NativeBackendObject {
  _nativeHandle = FImage.newFImage(name)

  override protected def destroy(): Unit = {
    FImage.disposeInternal(_nativeHandle)
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
}