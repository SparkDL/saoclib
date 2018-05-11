package com.pzque.sparkdl.saoclib


object NDRangeKernel {
  @native protected def newInstance(): Long

  @native protected def destroyInstance(handle: Long): Unit
}

class NDRangeKernel extends NativeBackendObject {
  _nativeHandle = NDRangeKernel.newInstance()

  override protected def destroy(): Unit = NDRangeKernel.destroyInstance(_nativeHandle)
}