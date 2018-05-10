import com.pzque.sparkdl.saoclib.FImage


object App {
  def main(args: Array[String]): Unit = {
    System.loadLibrary("saoclib")

    val image = new FImage("multi_matrix")
    image.initOpenCL()
    println(image.getNumDevices.get)
  }
}