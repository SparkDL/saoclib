package com.pzque.sparkdl.saoclib

import nativeapi.NDRangeKernel


class NDRangeKernel(work_dim: Int,
                    global_work_size: Array[Int],
                    local_work_size: Array[Int],
                    cl_image: ClImage,
                    device_handle: Long,
                    kernel_name: String,
                    arg_limits: Array[KernelArgLimit]) extends NativeBackendObject {
  _nativeHandle = NDRangeKernel.newInstance(
    work_dim, global_work_size,
    local_work_size, cl_image.getNativeHandle,
    device, kernel_name, arg_limits)

  val workDim: Int = work_dim
  val globalWorkSize: Array[Int] = global_work_size
  val localWorkSize: Array[Int] = local_work_size
  val clImage: ClImage = cl_image
  val device: Long = device_handle
  val kernelName: String = kernel_name
  val argLimits: Array[KernelArgLimit] = arg_limits

  def call(args: Array[KernelArg]): Unit = NDRangeKernel.call(_nativeHandle, argLimits)

  override protected def destroy(): Unit = NDRangeKernel.destroyInstance(_nativeHandle)


}