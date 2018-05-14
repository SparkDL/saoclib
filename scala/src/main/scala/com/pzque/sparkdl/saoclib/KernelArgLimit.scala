package com.pzque.sparkdl.saoclib

import scala.reflect.ClassTag

case class KernelArgLimit(arg_type: NativeType, arg_mode: KernelArgMode) {
  def verify[T: ClassTag](arg: KernelArg[T]): Boolean = {
    val ret = this == arg.argLimit
    println(s"expected: $this, actual: $arg")
    ret
  }

  override def toString: String = s"<$arg_type,$arg_mode>"

  // @native def rep(limit: KernelArgLimit): Unit
}

object limit {
  def apply(arg_type: NativeType, arg_mode: KernelArgMode): KernelArgLimit = {
    KernelArgLimit(arg_type, arg_mode)
  }
}