package com.pzque.sparkdl.saoclib

import com.pzque.sparkdl.saoclib.nativeapi.KernelArgModeID

trait KernelArgMode {
  def id: Long
}

object mode_input extends KernelArgMode {
  val _id: Long = KernelArgModeID.id_mode_input

  override def id: Long = _id

  override def toString: String = "input"
}

object mode_output extends KernelArgMode {
  val _id: Long = KernelArgModeID.id_mode_output

  override def id: Long = _id

  override def toString: String = "output"
}

object mode_input_output extends KernelArgMode {
  val _id: Long = KernelArgModeID.id_mode_input_output

  override def id: Long = _id

  override def toString: String = "input_output"
}

