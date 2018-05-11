package com.pzque.sparkdl.saoclib

object FImage {
  @native protected def newInstance(pFImage: Long, imagePath: String): Long

  @native protected def destroyInstance(pFImage: Long): Unit

  @native protected def deployImage(pFImage: Long, devices: Array[Long]): Boolean
}

class FImage(pFEnv: Long, imagePath: String) extends NativeBackendObject {
  _nativeHandle = FImage.newInstance(pFEnv, imagePath)

  override protected def destroy(): Unit = {
    FImage.destroyInstance(_nativeHandle)
  }

  def deployImage(devices: Array[Long]): Option[Boolean] = {
    if (isValid) {
      Some(FImage.deployImage(_nativeHandle, devices))
    }
    None
  }
}