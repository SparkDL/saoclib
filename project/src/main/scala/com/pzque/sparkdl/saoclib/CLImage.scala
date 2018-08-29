package com.pzque.sparkdl.saoclib

import com.pzque.sparkdl.utils.NativeBackendObject
import nativeapi.CLImage

class CLImage(_clEnv: CLEnv, _imagePath: String) extends NativeBackendObject {
  val clEnv: CLEnv = _clEnv
  val imagePath: String = _imagePath

  nativeHandle = CLImage.newInstance(_clEnv.getNativeHandle, _imagePath)

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