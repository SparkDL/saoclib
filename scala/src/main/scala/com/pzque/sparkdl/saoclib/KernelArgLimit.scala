package com.pzque.sparkdl.saoclib

case class KernelArgLimit(argType: NativeType, argMode: KernelArgMode)
  extends KernelArgQuery {
  def verify(arg: KernelArg): Boolean = {
    val ret = this == arg.argLimit
    println(s"expected: $this, actual: $arg")
    ret
  }

  override def toString: String = s"<$argType,$argMode>"


  /**
    * Get argument mode id, (input/output/input_output...)
    *
    * @return mode id of this argument
    */
  override def getModeID: Long = argMode.id

  /**
    * Get argument type id
    *
    * @return type id of this argument
    */
  override def getTypeID: Long = argType.id

  /**
    * Get the element type id
    * For primitive type, it equals to 'getTypeID'
    * For array type, it returns the type id of elements
    *
    * @return element type id of this array type argument
    */
  override def getElemTypeID: Long = {
    argType match {
      case array_type: c_array => array_type.elemType.id
      case primitive_type => primitive_type.id
    }
  }

  /**
    * Get total size of the type
    * For array type, it returns element size * array length
    *
    * @return the total size of this argument
    */
  override def getSize: Long = argType.size

  /**
    * Get the element size of array type
    * For primitive type, it equals to 'getSize'
    * For array type, it returns the size of one element
    *
    * @return primitive type size or the size of one element in array type
    */
  override def getElemSize: Long = {
    argType match {
      case array_type: c_array => array_type.elemType.size
      case primitive_type => primitive_type.size
    }
  }

  /**
    * Get the length of array
    * For primitive type, it returns 0
    * For array type, it returns array length
    *
    * @return 0 or array length
    */
  override def getArrayLength: Long = {
    argType match {
      case array_type: c_array => array_type.length
      case _ => 0
    }
  }
}

object limit {
  def apply(arg_type: NativeType, arg_mode: KernelArgMode): KernelArgLimit = {
    KernelArgLimit(arg_type, arg_mode)
  }
}