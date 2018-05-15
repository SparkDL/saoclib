package com.pzque.sparkdl.saoclib

case class KernelArgLimit(argType: NativeType, argModeID: Long) {
  def verify(arg: KernelArg): Boolean = {
    val ret = this == arg.argLimit
    println(s"expected: $this, actual: $arg")
    ret
  }

  override def toString: String = s"<$argType,$argModeID>"
}

object limit {
  def apply(arg_type: NativeType, arg_mode: KernelArgMode): KernelArgLimit = {
    KernelArgLimit(arg_type, arg_mode.id)
  }
}