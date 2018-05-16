import com.pzque.sparkdl.saoclib._
import com.pzque.sparkdl.saoclib.nativeapi.NDRangeKernel

object App {
  def main(args: Array[String]): Unit = {
    System.loadLibrary("saoclib")
    testCL();
  }

  def test(): Unit = {
    System.loadLibrary("saoclib")
    val limits: Array[KernelArgLimit] = Array(
      limit(c_array(c_int, 10), mode_input),
      limit(c_array(c_int, 10), mode_input),
      limit(c_array(c_int, 10), mode_output)
    )

    val a = Array(1, 2, 3, 4, 5)
    val b = Array(1, 2, 3, 4, 5)
    val c = Array(1)
    val values = Array(
      ArgIntArray(a, 5, mode_input),
      ArgIntArray(b, 5, mode_input),
      ArgIntArray(c, 5, mode_output)
    )

    limits.zip(values).foreach(
      x => {
        val limit = x._1
        val value = x._2
        limit.verify(value)
      }
    )
  }


  def testCL() = {
    System.loadLibrary("saoclib")

    val env = new ClEnv()
    env.initOpenCL()
    val image = new ClImage(env.getNativeHandle,
      "/home/pcz/Projects/saoclib/bin/vector_add")

    val device_handle = env.getDeviceId(0).get
    val device_handle_list = Array(device_handle)
    image.deployImage(device_handle_list)

    val N = 100

    val limits: Array[KernelArgLimit] = Array(
      limit(c_array(c_int, N), mode_input),
      limit(c_array(c_int, N), mode_input),
      limit(c_array(c_int, N), mode_output)
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
  }


}