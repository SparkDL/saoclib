package com.pzque.sparkdl.saoclib

trait KernelArgQuery {
  /**
    * Get argument mode id, (input/output/input_output...)
    *
    * @return mode id of this argument
    */
  def getModeID: Long

  /**
    * Get argument type id
    *
    * @return type id of this argument
    */
  def getTypeID: Long


  /**
    * Get the element type id
    * For primitive type, it equals to 'getTypeID'
    * For array type, it returns the type id of elements
    *
    * @return element type id of this array type argument
    */
  def getElemTypeID: Long

  /**
    * Get total size of the type
    * For array type, it returns element size * array length
    *
    * @return the total size of this argument
    */
  def getSize: Long

  /**
    * Get the element size of array type
    * For primitive type, it equals to 'getSize'
    * For array type, it returns the size of one element
    *
    * @return primitive type size or the size of one element in array type
    */
  def getElemSize: Long


  /**
    * Get the length of array
    * For primitive type, it returns 0
    * For array type, it returns array length
    *
    * @return 0 or array length
    */
  def getArrayLength: Long
}
