import com.pzque.sparkdl.saoclib._

object App {
  def main(args: Array[String]): Unit = {
    System.loadLibrary("saoclib")
    testCL()
  }

  def testCL() = {
    System.loadLibrary("saoclib")

    // init opencl essentials
    val env = new ClEnv()
    env.initOpenCL()
    // construct an opencl image from file path
    val image = new ClImage(env.getNativeHandle,
      "/home/pcz/Projects/saoclib/bin/vector_add")

    // get available devices and deploy an image to specific devices
    val device_handle_list = env.getDeviceIdList.get
    device_handle_list.foreach(
      x => {
        println(s"device: $x")
      })
    val device_handle = device_handle_list(0)

    image.deployImage(device_handle_list)

    // construct a kernel
    val N = 100
    val limits: Array[KernelArgLimit] = Array(
      limit(c_array(c_float, N), mode_input),
      limit(c_array(c_float, N), mode_input),
      limit(c_array(c_float, N), mode_output)
    )

    val kernel = new NDRangeKernel(
      work_dim = 1,
      global_work_size_list = Array(N),
      local_work_size_list = null,
      cl_image = image,
      device_handle = device_handle,
      kernel_name = "vector_add",
      arg_limits = limits
    )
    // call kernel with arguments
    val a: ArgArray[Float] = ArgArray.fill(N)(1f)(mode_input)
    val b: ArgArray[Float] = ArgArray.fill(N)(2f)(mode_input)
    val c: ArgArray[Float] = ArgArray.fill(N)(0f)(mode_output)

    kernel.call(a, b, c)

    c.argValue.foreach(x => print(s"$x,"))
  }
}