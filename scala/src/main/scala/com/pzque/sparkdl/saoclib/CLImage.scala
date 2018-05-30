package com.pzque.sparkdl.saoclib

import nativeapi.CLImage

class CLImage(clEnv: CLEnv, imagePath: String) extends NativeBackendObject {
  nativeHandle = CLImage.newInstance(clEnv.getNativeHandle, imagePath)

  override protected def destroy(): Unit = {
    CLImage.destroyInstance(nativeHandle)
  }

  def deployImage(devices: Array[Long]): Option[Boolean] = {
    if (isValid) {
      Some(CLImage.deployImage(nativeHandle, devices))
    }
    None
  }
}