package com.intel.analytics.bigdl.nn

import com.intel.analytics.bigdl.tensor.{Tensor, TensorKernel}
import com.intel.analytics.bigdl.tensor.TensorNumericMath.TensorNumeric
import com.pzque.sparkdl.saoclib._

import scala.reflect.ClassTag

object FMath {
  System.loadLibrary("saoclib")
  // init opencl essentials
  val env = new CLEnv()
  env.initOpenCL()
  // get available devices
  val device_id_list = env.getDeviceIDList.get
  print("devices: ")
  println(device_id_list.mkString("[", ",", "]"))
  val device_id = device_id_list(0)

  // deploy the image to specific devices
  // construct an opencl image from file path
  val matrixMultImage = new CLImage(env, "/home/pcz/Projects/saoclib/bin/matrix_mult")
  matrixMultImage.deployImage(device_id_list)


  def matrixMult[T <: AnyVal : ClassTag]
  (m1: Tensor[T], m2: Tensor[T], output: Tensor[T])
  (implicit ev: TensorNumeric[T], tm: NativeTypeMapping[T]) = {
    require(m1.dim() == 2 && m2.dim() == 2,
      s"matrices expected, got ${m1.dim()}, ${m2.dim()} tensors")
    require(m1.size(2) == m2.size(1),
      s"size mismatch, m1:${m1.size().mkString("x")} m2:${m2.size().mkString("x")}")

    // construct a kernel
    val BLOCK_SIZE: Int = 1
    val aH: Int = m1.size(1)
    val aW: Int = m1.size(2)
    val bH: Int = m2.size(1)
    val bW: Int = m2.size(2)
    val cH: Int = aH
    val cW: Int = bW
    val aSize = aH * aW
    val bSize = bH * bW
    val cSize = cH * cW

    val limits: Array[KernelArgLimit] = Array(
      limit(c_int, mode_input),
      limit(c_int, mode_input),
      limit(c_array(tm.nativeType, aSize), mode_input),
      limit(c_array(tm.nativeType, bSize), mode_input),
      limit(c_array(tm.nativeType, cSize), mode_output)
    )
    val kernel = new NDRangeKernel(
      _workDim = 2,
      _globalWorkSizeList = Array(cW, cH),
      _localWorkSizeList = Array(BLOCK_SIZE, BLOCK_SIZE),
      _clImage = matrixMultImage,
      _deviceID = device_id,
      _kernelName = "matrixMult",
      _argLimits = limits
    )

    val a = TensorKernel.tensor2array(m1)
    val b = TensorKernel.tensor2array(m2)
    val c = TensorKernel.tensor2array(output)
    // prepare kernel arguments
    val aWArg = ArgVal[Int](aW)(mode_input)
    val bWArg = ArgVal[Int](bW)(mode_input)
    val aArg: ArgArray[T] = ArgArray(a)(mode_input)
    val bArg: ArgArray[T] = ArgArray(b)(mode_input)
    val cArg: ArgArray[T] = ArgArray(c)(mode_output)

    // call kernel
    var start, end: Long = 0
    start = System.currentTimeMillis()
    kernel.call(aWArg, bWArg, aArg, bArg, cArg)
    end = System.currentTimeMillis()
    println(s"Kernel cost: ${end - start}ms\n")
  }
}
