package com.pzque.sparkdl.mkl

object MKLLoader {
  val MKL: ThreadLocal[MKLInterface] = new ThreadLocal[MKLInterface] {
    var mkl: MKLInterface = _

    override def initialValue: MKLInterface = {
      val instance = ACLMKL.getInstance()
      instance match {
        case Left(msg) => {
          println(f"****** Using Intel MKL library in thread-${Thread.currentThread().getId}: cannot load ACLMKL: $msg ******\n")
          mkl = IntelMKL
          IntelMKL
        }
        case Right(aclmkl) => {
          println(f"****** Using ACLMKL library in thread-${Thread.currentThread().getId} ******")
          mkl = aclmkl
          aclmkl
        }
      }
    }

    override def finalize(): Unit = {
      println("==========================finalizing")
      if (mkl != null) {
        mkl.cleanup
      }
    }
  }
}
