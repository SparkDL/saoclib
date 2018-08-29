package com.pzque.sparkdl.utils

abstract class NativeBackendObject extends AutoCloseable {
  protected var nativeHandle: Long = 0
  protected var nativeOwner: Boolean = true

  def getNativeHandle: Long = nativeHandle

  def isValid: Boolean = nativeOwner && nativeHandle != 0

  override def close(): Unit = {
    this.synchronized {
      if (nativeOwner && nativeHandle != 0) {
        destroy()
        nativeHandle = 0
        nativeOwner = false
      }
    }
  }

  protected def destroy(): Unit
}
