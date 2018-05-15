package com.pzque.sparkdl.saoclib

abstract class NativeBackendObject extends AutoCloseable {
  protected var _nativeHandle: Long = 0
  protected var _nativeOwner: Boolean = true

  def getNativeHandle: Long = _nativeHandle

  def isValid: Boolean = _nativeOwner && _nativeHandle != 0

  override def close(): Unit = {
    this.synchronized {
      if (_nativeOwner && _nativeHandle != 0) {
        destroy()
        _nativeHandle = 0
        _nativeOwner = false
      }
    }
  }

  protected def destroy(): Unit
}
