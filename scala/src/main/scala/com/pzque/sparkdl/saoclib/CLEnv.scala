package com.pzque.sparkdl.saoclib

import nativeapi.CLEnv


class CLEnv extends NativeBackendObject {
  _nativeHandle = CLEnv.newInstance()

  override protected def destroy(): Unit = {
    CLEnv.destroyInstance(_nativeHandle)
  }

  def initOpenCL(): Option[Boolean] = {
    if (isValid) {
      return Some(CLEnv.initOpenCL(_nativeHandle))
    }
    None
  }

  def getNumDevices: Option[Long] = {
    if (isValid) {
      return Some(CLEnv.getNumDevices(_nativeHandle))
    }
    None
  }

  def getDeviceIDList: Option[Array[Long]] = {
    if (isValid) {
      return Some(CLEnv.getDeviceIDList(_nativeHandle))
    }
    None
  }

  def getDeviceID(index: Int): Option[Long] = {
    if (isValid) {
      return Some(CLEnv.getDeviceID(_nativeHandle, index))
    }
    None
  }

}
