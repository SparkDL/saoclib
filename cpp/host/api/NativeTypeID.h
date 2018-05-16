//
// Created by pcz on 18-5-15.
//

#ifndef SAOCLIB_CPP_NATIVETYPEID_H
#define SAOCLIB_CPP_NATIVETYPEID_H

/*!
 * This enumeration is used to be identify native type in scala.
 * Feel free to modify their values.
 * @note If you want to add new IDs, remember to add new methods to scala calss com.pzque.sacolib.natvieapi.NativeTypeID.
 */
enum class NativeTypeID : jlong {
    c_void = 0,
    c_byte = 1,
    c_short = 2,
    c_int = 3,
    c_long = 4,
    c_float = 5,
    c_double = 6,
    c_array = 7
};

#endif //SAOCLIB_CPP_NATIVETYPEID_H
