package com.pzque.sparkdl.saoclib

abstract class NativeBackendObject extends AutoCloseable {

  protected var _nativeHandle: Long = 0
  protected var _nativeOwner: Boolean = true

  override def close(): Unit = {
    this.synchronized {
      if (_nativeOwner && _nativeHandle != 0) {
        disposeInternal()
        _nativeHandle = 0
        _nativeOwner = false
      }
    }
  }

  protected def disposeInternal(): Unit

  def isValid: Boolean = _nativeOwner && _nativeHandle != 0
}

