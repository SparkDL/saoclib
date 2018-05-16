package com.pzque.sparkdl.saoclib.nativeapi

object NDRangeKernel {
  @native def newInstance(work_dim: Int,
                          global_work_size_list: Array[Long],
                          local_work_size_list: Array[Long],
                          image_handle: ClImage.PtrClImage,
                          device_handle: Long,
                          kernel_name: String,
                          arg_limits: Array[_ <: Object]): Long

  @native def destroyInstance(handle: Long): Unit

  @native def call(handle: Long, args: Array[_ <: Object])
}