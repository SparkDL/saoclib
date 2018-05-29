package com.pzque.sparkdl.saoclib

import com.pzque.sparkdl.saoclib.nativeapi.NDRangeKernel


class NDRangeKernel
(_workDim: Int,
 _globalWorkSizeList: Array[Long],
 _localWorkSizeList: Array[Long],
 _clImage: CLImage,
 _deviceID: Long,
 _kernelName: String,
 _argLimits: Array[KernelArgLimit]
) extends NativeBackendObject with Kernel {
  _nativeHandle = NDRangeKernel.newInstance(
    _workDim, _globalWorkSizeList,
    _localWorkSizeList, _clImage.getNativeHandle,
    _deviceID, _kernelName, _argLimits)

  val workDim: Int = _workDim
  val globalWorkSize: Array[Long] = _globalWorkSizeList
  val localWorkSize: Array[Long] = _localWorkSizeList
  val clImage: CLImage = _clImage
  val deviceID: Long = _deviceID
  val kernelName: String = _kernelName
  val argLimits: Array[KernelArgLimit] = _argLimits

  override def call(args: Array[_ <: KernelArg]): Unit = {
    NDRangeKernel.call(_nativeHandle, args)
  }

  override protected def destroy(): Unit = NDRangeKernel.destroyInstance(_nativeHandle)
}