package com.pzque.sparkdl.saoclib

abstract class KernelArgMode

object mode_input extends KernelArgMode {
  override def toString: String = "input"
}

object mode_output extends KernelArgMode {
  override def toString: String = "output"
}

object mode_input_output extends KernelArgMode {
  override def toString: String = "input_output"
}
