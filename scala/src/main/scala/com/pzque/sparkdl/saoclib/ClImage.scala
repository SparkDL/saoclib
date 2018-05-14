package com.pzque.sparkdl.saoclib

object ClImage {
  @native protected def newInstance(handle: Long, imagePath: String): Long

  @native protected def destroyInstance(handle: Long): Unit

  @native protected def deployImage(handle: Long, devices: Array[Long]): Boolean
}

class ClImage(pFEnv: Long, imagePath: String) extends NativeBackendObject {
  _nativeHandle = ClImage.newInstance(pFEnv, imagePath)

  override protected def destroy(): Unit = {
    ClImage.destroyInstance(_nativeHandle)
  }

  def deployImage(devices: Array[Long]): Option[Boolean] = {
    if (isValid) {
      Some(ClImage.deployImage(_nativeHandle, devices))
    }
    None
  }
}