package com.pzque.sparkdl.saoclib.nativeapi

object NDRangeKernel {
  @native def newInstance(work_dim: Int,
                          global_work_size: Array[Int],
                          local_work_size: Array[Int],
                          image_handle: ClImage.PtrClImage,
                          device_handle: Long,
                          kernel_name: String,
                          arg_limits: Array[_ <: Object]): Long

  @native def destroyInstance(handle: Long): Unit

  @native def call(handle: Long, args: Array[_ <: Object])

  @native def test_get_type_id(obj: Object): Long
}