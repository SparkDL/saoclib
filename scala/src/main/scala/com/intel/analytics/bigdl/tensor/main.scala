package com.intel.analytics.bigdl.tensor

import com.intel.analytics.bigdl.utils.T

object main {

  def main(args: Array[String]): Unit = {
    val a = Array(1f, 2f, 3f, 4f, 5f, 6f, 7f, 8f, 9f, 10f, 11f, 12f)
    val t = TensorKernel.toDenseTensor(a, Array(2, 2, 3))
    println(t)
  }

  def as[T](array: Array[T]): String = {
    array.mkString("[", ",", "]")
  }

  def test(): Unit = {
    val a = Array(1f, 2f, 3f, 4f, 5f, 6f, 7f, 8f, 9f)
    val storage = new ArrayStorage[Float](a)
    val ten = new DenseTensor[Float](
      storage = storage,
      storageOffset = 1,
      size = Array[Int](3, 3),
      stride = Array[Int](3, 1))

    println(ten)

    var x = Tensor[Float](
      T(
        T(T(1f, 2f, 3f),
          T(4f, 5f, 6f)
        ),
        T(T(7f, 8f, 9f),
          T(10f, 11f, 12f)
        )))


    println(s"storageOffset:${x.storageOffset()}")
    println(s"size:${as(x.size())}")
    println(s"stride:${as(x.stride())}")

    println(x)
    println(as(x.storage().array()))
  }
}
