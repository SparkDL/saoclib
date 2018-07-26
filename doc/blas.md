# level1 - vector operations
## vsAdd
y = a + b
```c++
void vsAdd(int n, float *a, float *b, float *y)
```

## vsSub
y = a - b
```c++
void vsSub(int n, float *a, float *b, float *y)
```

## vsMul
y = a * b
```c++
void vsMul(int n, float *a, float *b, float *y)
```

## vsDiv
y = a / b
```c++
void vsDiv(int n, float *a, float *b, float *y)
```

## vsPowx
y = a ^ b_const
```c++
void vsPowx(int n, float *a, float b, float *y)
```

## vsLn
y = ln(a)
```c++
void vsLn(int n, float *a, float *y)
```

## vsExp
y = exp(a)
```c++
void vsExp(int n, float *a, float *y)
```

## vsSqrt
y = sqrt(a)
```c++
void vsSqrt(int n, float *a, float *y)
```

## vsTanh
y = tanh(a)
```c++
void vsTanh(int n, float *a, float *y)
```

## vsLog1p
y = log1p(a)
```c++
void vsLog1p(int n, float *a, float *y)
```

## vsAbs
y = abs(a)
```c++
void vsAbs(int n, float *a, float *y)
```

## vsdot
sx dot sy
```c++
float cblas_sdot(int n, float *sx, float incx, float *sy, float incy)
```

```
Purpose:
    SDOT forms the dot product of two vectors.
    uses unrolled loops for increments equal to one.
Parameters
[in]	N	
          N is INTEGER
         number of elements in input vector(s)
[in]	SX	
          SX is REAL array, dimension ( 1 + ( N - 1 )*abs( INCX ) )
[in]	INCX	
          INCX is INTEGER
         storage spacing between elements of SX
[in]	SY	
          SY is REAL array, dimension ( 1 + ( N - 1 )*abs( INCY ) )
[in]	INCY	
          INCY is INTEGER
         storage spacing between elements of SY
```

## vsaxpy
y = a_const * x + y

```c++
void cblas_saxpy(int n, float sa, float *sx, int incx, float *sy, int incy)
```
```
[in]	N	
          N is INTEGER
         number of elements in input vector(s)
[in]	SA	
          SA is REAL
           On entry, SA specifies the scalar alpha.
[in]	SX	
          SX is REAL array, dimension ( 1 + ( N - 1 )*abs( INCX ) )
[in]	INCX	
          INCX is INTEGER
         storage spacing between elements of SX
[in,out]	SY	
          SY is REAL array, dimension ( 1 + ( N - 1 )*abs( INCY ) )
[in]	INCY	
          INCY is INTEGER
         storage spacing between elements of SY
```

## vsscal 
x = a_const * x
```c++
cblas_sscal(int n, float sa, float* sx, incx)	
```
```
Purpose:
    SSCAL scales a vector by a constant.
    uses unrolled loops for increment equal to 1.
Parameters
[in]	N	
          N is INTEGER
         number of elements in input vector(s)
[in]	SA	
          SA is REAL
           On entry, SA specifies the scalar alpha.
[in,out]	SX	
          SX is REAL array, dimension ( 1 + ( N - 1 )*abs( INCX ) )
[in]	INCX	
          INCX is INTEGER
         storage spacing between elements of SX
```
# level2 - matrix x vector operations
## cblas_sgemv


# level3 - matrix operations

## cblas_sgemm
```c++
cblas_gem(CblasColMajor,
```
