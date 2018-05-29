package com.pzque.sparkdl.saoclib

trait Kernel {
  def call(args: KernelArg*): Unit = call(args.toArray)

  def call(args: Array[_ <: KernelArg]): Unit
}
