import com.intel.analytics.bigdl.nn._
import com.pzque.sparkdl.codegen._


object Main {
  def main(args: Array[String]): Unit = {
    val linear = Linear[Float](2, 3)
    val conv = SpatialConvolution[Float](2, 3, 4, 5)
    val model = Sequential[Float]()
      .add(linear)
      .add(conv)
    CodeGen.gen(model)
  }
}