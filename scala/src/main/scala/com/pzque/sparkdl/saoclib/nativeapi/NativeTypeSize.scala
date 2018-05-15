package com.pzque.sparkdl.saoclib.nativeapi

object NativeTypeSize {
  @native def sizeof_c_byte: Long

  @native def sizeof_c_short: Long

  @native def sizeof_c_int: Long

  @native def sizeof_c_long: Long

  @native def sizeof_c_float: Long

  @native def sizeof_c_double: Long
}
