package com.pzque.sparkdl.saoclib

import nativeapi.CLImage

class CLImage(clEnv: CLEnv, imagePath: String) extends NativeBackendObject {
  _nativeHandle = CLImage.newInstance(clEnv.getNativeHandle, imagePath)

  override protected def destroy(): Unit = {
    CLImage.destroyInstance(_nativeHandle)
  }

  def deployImage(devices: Array[Long]): Option[Boolean] = {
    if (isValid) {
      Some(CLImage.deployImage(_nativeHandle, devices))
    }
    None
  }
}