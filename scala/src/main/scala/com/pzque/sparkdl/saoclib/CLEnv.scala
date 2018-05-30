package com.pzque.sparkdl.saoclib

import nativeapi.CLEnv


class CLEnv extends NativeBackendObject {
  nativeHandle = CLEnv.newInstance()

  override protected def destroy(): Unit = {
    CLEnv.destroyInstance(nativeHandle)
  }

  def initOpenCL(): Option[Boolean] = {
    if (isValid) {
      return Some(CLEnv.initOpenCL(nativeHandle))
    }
    None
  }

  def getNumDevices: Option[Long] = {
    if (isValid) {
      return Some(CLEnv.getNumDevices(nativeHandle))
    }
    None
  }

  def getDeviceIDList: Option[Array[Long]] = {
    if (isValid) {
      return Some(CLEnv.getDeviceIDList(nativeHandle))
    }
    None
  }

  def getDeviceID(index: Int): Option[Long] = {
    if (isValid) {
      return Some(CLEnv.getDeviceID(nativeHandle, index))
    }
    None
  }

}
