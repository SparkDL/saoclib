package com.pzque.sparkdl.saoclib

import nativeapi.ClImage

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