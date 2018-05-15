package com.pzque.sparkdl.saoclib

import nativeapi.ClEnv


class ClEnv extends NativeBackendObject {
  _nativeHandle = ClEnv.newInstance()

  override protected def destroy(): Unit = {
    ClEnv.destroyInstance(_nativeHandle)
  }

  def initOpenCL(): Option[Boolean] = {
    if (isValid) {
      return Some(ClEnv.initOpenCL(_nativeHandle))
    }
    None
  }

  def getNumDevices: Option[Long] = {
    if (isValid) {
      return Some(ClEnv.getNumDevices(_nativeHandle))
    }
    None
  }

  def getDeviceIdList: Option[Array[Long]] = {
    if (isValid) {
      return Some(ClEnv.getDeviceIdList(_nativeHandle))
    }
    None
  }

  def getDeviceId(index: Int): Option[Long] = {
    if (isValid) {
      return Some(ClEnv.getDeviceId(_nativeHandle, index))
    }
    None
  }

}
