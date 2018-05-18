//
// Created by pcz on 18-5-17.
//

#ifndef SAOCLIB_CPP_NATIVETYPEID_H
#define SAOCLIB_CPP_NATIVETYPEID_H
/*!
 * This enumeration is used to be identify native type in scala.
 * Feel free to modify their values.
 * @note If you want to add new IDs, remember to add new methods to scala calss com.pzque.sacolib.natvieapi.NativeTypeID.
 */
enum class NativeTypeID : long {
    c_void = 0,
    c_byte = 1,
    c_short = 2,
    c_int = 3,
    c_long = 4,
    c_float = 5,
    c_double = 6,
    c_array = 7
};

typedef signed char byte;

template<class T>
struct NativeTypeIDMapping;

template<>
struct NativeTypeIDMapping<void> {
    static const NativeTypeID typeID = NativeTypeID::c_void;
};

template<>
struct NativeTypeIDMapping<byte> {
    static const NativeTypeID typeID = NativeTypeID::c_byte;
};

template<>
struct NativeTypeIDMapping<short> {
    static const NativeTypeID typeID = NativeTypeID::c_short;
};

template<>
struct NativeTypeIDMapping<int> {
    static const NativeTypeID typeID = NativeTypeID::c_int;
};

template<>
struct NativeTypeIDMapping<long> {
    static const NativeTypeID typeID = NativeTypeID::c_long;
};

template<>
struct NativeTypeIDMapping<float> {
    static const NativeTypeID typeID = NativeTypeID::c_float;
};

template<>
struct NativeTypeIDMapping<double> {
    static const NativeTypeID typeID = NativeTypeID::c_double;
};
#endif //SAOCLIB_CPP_NATIVETYPEID_H
