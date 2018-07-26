# BLAS Functions
## level1 - vector operations
### vsAdd
y = a + b
```c++
void vsAdd(int n, float *a, float *b, float *y)
```

### vsSub
y = a - b
```c++
void vsSub(int n, float *a, float *b, float *y)
```

### vsMul
y = a * b
```c++
void vsMul(int n, float *a, float *b, float *y)
```

### vsDiv
y = a / b
```c++
void vsDiv(int n, float *a, float *b, float *y)
```

### vsPowx
y = a ^ b_const
```c++
void vsPowx(int n, float *a, float b, float *y)
```

### vsLn
y = ln(a)
```c++
void vsLn(int n, float *a, float *y)
```

### vsExp
y = exp(a)
```c++
void vsExp(int n, float *a, float *y)
```

### vsSqrt
y = sqrt(a)
```c++
void vsSqrt(int n, float *a, float *y)
```

### vsTanh
y = tanh(a)
```c++
void vsTanh(int n, float *a, float *y)
```

### vsLog1p
y = log1p(a)
```c++
void vsLog1p(int n, float *a, float *y)
```

### vsAbs
y = abs(a)
```c++
void vsAbs(int n, float *a, float *y)
```

### vsdot
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

### vsaxpy
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

### vsscal 
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
## level2 - matrix x vector operations

### vsger
`A := alpha*x*y**T + A`

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
### vsgemv
```c++
cblas_gemv(enum trans, int m, int n, float alpha, float *a, int lda, 
    float* x, int incx, float beta, float* y, int incy);
```

```
Purpose:
 SGEMV  performs one of the matrix-vector operations

    y := alpha*A*x + beta*y,   or   y := alpha*A**T*x + beta*y,

 where alpha and beta are scalars, x and y are vectors and A is an
 m by n matrix.
Parameters
[in]	TRANS	
          TRANS is CHARACTER*1
           On entry, TRANS specifies the operation to be performed as
           follows:

              TRANS = 'N' or 'n'   y := alpha*A*x + beta*y.

              TRANS = 'T' or 't'   y := alpha*A**T*x + beta*y.

              TRANS = 'C' or 'c'   y := alpha*A**T*x + beta*y.
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
[in]	A	
          A is REAL array, dimension ( LDA, N )
           Before entry, the leading m by n part of the array A must
           contain the matrix of coefficients.
[in]	LDA	
          LDA is INTEGER
           On entry, LDA specifies the first dimension of A as declared
           in the calling (sub) program. LDA must be at least
           max( 1, m ).
[in]	X	
          X is REAL array, dimension at least
           ( 1 + ( n - 1 )*abs( INCX ) ) when TRANS = 'N' or 'n'
           and at least
           ( 1 + ( m - 1 )*abs( INCX ) ) otherwise.
           Before entry, the incremented array X must contain the
           vector x.
[in]	INCX	
          INCX is INTEGER
           On entry, INCX specifies the increment for the elements of
           X. INCX must not be zero.
[in]	BETA	
          BETA is REAL
           On entry, BETA specifies the scalar beta. When BETA is
           supplied as zero then Y need not be set on input.
[in,out]	Y	
          Y is REAL array, dimension at least
           ( 1 + ( m - 1 )*abs( INCY ) ) when TRANS = 'N' or 'n'
           and at least
           ( 1 + ( n - 1 )*abs( INCY ) ) otherwise.
           Before entry with BETA non-zero, the incremented array Y
           must contain the vector y. On exit, Y is overwritten by the
           updated vector y.
[in]	INCY	
          INCY is INTEGER
           On entry, INCY specifies the increment for the elements of
           Y. INCY must not be zero.

```

# level3 - matrix operations

## vsgemm
`C := alpha*op( A )*op( B ) + beta*C`

```c++
cblas_gem(enum transa, enum transb, int m, int n, int k, int alpha, 
    float * a, int lda, float *b, int ldb,
    float beta, float *c, int ldc)
```
```
Purpose:
 SGEMM  performs one of the matrix-matrix operations

    C := alpha*op( A )*op( B ) + beta*C,

 where  op( X ) is one of

    op( X ) = X   or   op( X ) = X**T,

 alpha and beta are scalars, and A, B and C are matrices, with op( A )
 an m by k matrix,  op( B )  a  k by n matrix and  C an m by n matrix.
Parameters
[in]	TRANSA	
          TRANSA is CHARACTER*1
           On entry, TRANSA specifies the form of op( A ) to be used in
           the matrix multiplication as follows:

              TRANSA = 'N' or 'n',  op( A ) = A.

              TRANSA = 'T' or 't',  op( A ) = A**T.

              TRANSA = 'C' or 'c',  op( A ) = A**T.
[in]	TRANSB	
          TRANSB is CHARACTER*1
           On entry, TRANSB specifies the form of op( B ) to be used in
           the matrix multiplication as follows:

              TRANSB = 'N' or 'n',  op( B ) = B.

              TRANSB = 'T' or 't',  op( B ) = B**T.

              TRANSB = 'C' or 'c',  op( B ) = B**T.
[in]	M	
          M is INTEGER
           On entry,  M  specifies  the number  of rows  of the  matrix
           op( A )  and of the  matrix  C.  M  must  be at least  zero.
[in]	N	
          N is INTEGER
           On entry,  N  specifies the number  of columns of the matrix
           op( B ) and the number of columns of the matrix C. N must be
           at least zero.
[in]	K	
          K is INTEGER
           On entry,  K  specifies  the number of columns of the matrix
           op( A ) and the number of rows of the matrix op( B ). K must
           be at least  zero.
[in]	ALPHA	
          ALPHA is REAL
           On entry, ALPHA specifies the scalar alpha.
[in]	A	
          A is REAL array, dimension ( LDA, ka ), where ka is
           k  when  TRANSA = 'N' or 'n',  and is  m  otherwise.
           Before entry with  TRANSA = 'N' or 'n',  the leading  m by k
           part of the array  A  must contain the matrix  A,  otherwise
           the leading  k by m  part of the array  A  must contain  the
           matrix A.
[in]	LDA	
          LDA is INTEGER
           On entry, LDA specifies the first dimension of A as declared
           in the calling (sub) program. When  TRANSA = 'N' or 'n' then
           LDA must be at least  max( 1, m ), otherwise  LDA must be at
           least  max( 1, k ).
[in]	B	
          B is REAL array, dimension ( LDB, kb ), where kb is
           n  when  TRANSB = 'N' or 'n',  and is  k  otherwise.
           Before entry with  TRANSB = 'N' or 'n',  the leading  k by n
           part of the array  B  must contain the matrix  B,  otherwise
           the leading  n by k  part of the array  B  must contain  the
           matrix B.
[in]	LDB	
          LDB is INTEGER
           On entry, LDB specifies the first dimension of B as declared
           in the calling (sub) program. When  TRANSB = 'N' or 'n' then
           LDB must be at least  max( 1, k ), otherwise  LDB must be at
           least  max( 1, n ).
[in]	BETA	
          BETA is REAL
           On entry,  BETA  specifies the scalar  beta.  When  BETA  is
           supplied as zero then C need not be set on input.
[in,out]	C	
          C is REAL array, dimension ( LDC, N )
           Before entry, the leading  m by n  part of the array  C must
           contain the matrix  C,  except when  beta  is zero, in which
           case C need not be set on entry.
           On exit, the array  C  is overwritten by the  m by n  matrix
           ( alpha*op( A )*op( B ) + beta*C ).
[in]	LDC	
          LDC is INTEGER
           On entry, LDC specifies the first dimension of C as declared
           in  the  calling  (sub)  program.   LDC  must  be  at  least
           max( 1, m ).
```
