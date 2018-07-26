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
## vsger
```c++
cblas_vsger(int m, int n, float alpha, float* x, int incx, float* y, int incy, float *a, int lda)
```
```
Purpose:
 SGER   performs the rank 1 operation

    A := alpha*x*y**T + A,

 where alpha is a scalar, x is an m element vector, y is an n element
 vector and A is an m by n matrix.
Parameters
[in]	M	
          M is INTEGER
           On entry, M specifies the number of rows of the matrix A.
           M must be at least zero.
[in]	N	
          N is INTEGER
           On entry, N specifies the number of columns of the matrix A.
           N must be at least zero.
[in]	ALPHA	
          ALPHA is REAL
           On entry, ALPHA specifies the scalar alpha.
[in]	X	
          X is REAL array, dimension at least
           ( 1 + ( m - 1 )*abs( INCX ) ).
           Before entry, the incremented array X must contain the m
           element vector x.
[in]	INCX	
          INCX is INTEGER
           On entry, INCX specifies the increment for the elements of
           X. INCX must not be zero.
[in]	Y	
          Y is REAL array, dimension at least
           ( 1 + ( n - 1 )*abs( INCY ) ).
           Before entry, the incremented array Y must contain the n
           element vector y.
[in]	INCY	
          INCY is INTEGER
           On entry, INCY specifies the increment for the elements of
           Y. INCY must not be zero.
[in,out]	A	
          A is REAL array, dimension ( LDA, N )
           Before entry, the leading m by n part of the array A must
           contain the matrix of coefficients. On exit, A is
           overwritten by the updated matrix.
[in]	LDA	
          LDA is INTEGER
           On entry, LDA specifies the first dimension of A as declared
           in the calling (sub) program. LDA must be at least
           max( 1, m ).
```
## vsgemv


# level3 - matrix operations

## cblas_sgemm
```c++
cblas_gem(CblasColMajor,
```
