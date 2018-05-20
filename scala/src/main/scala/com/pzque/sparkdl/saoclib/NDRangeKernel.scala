package com.pzque.sparkdl.saoclib

import nativeapi.NDRangeKernel


class NDRangeKernel(work_dim: Int,
                    global_work_size_list: Array[Long],
                    local_work_size_list: Array[Long],
                    cl_image: CLImage,
                    device_handle: Long,
                    kernel_name: String,
                    arg_limits: Array[KernelArgLimit])
  extends NativeBackendObject {
  _nativeHandle = NDRangeKernel.newInstance(
    work_dim, global_work_size_list,
    local_work_size_list, cl_image.getNativeHandle,
    device_handle, kernel_name, arg_limits)

  val workDim: Int = work_dim
  val globalWorkSize: Array[Long] = global_work_size_list
  val localWorkSize: Array[Long] = local_work_size_list
  val clImage: CLImage = cl_image
  val deviceHandle: Long = device_handle
  val kernelName: String = kernel_name
  val argLimits: Array[KernelArgLimit] = arg_limits

  def call(args: KernelArg*): Unit = {
    NDRangeKernel.call(_nativeHandle, args.toArray)
  }

  override protected def destroy(): Unit = NDRangeKernel.destroyInstance(_nativeHandle)
}