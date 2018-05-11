import com.pzque.sparkdl.saoclib._


object App {
  def main(args: Array[String]): Unit = {
    System.loadLibrary("saoclib")

    val env = new FEnv()
    env.initOpenCL()

    val image = new FImage(env.getNativeHandler, "/home/pcz/Projects/saoclib/bin/matrix_mult")
    val device = env.getDeviceId(0).get
    val devices = Array(device)
    image.deployImage(devices)
  }
}