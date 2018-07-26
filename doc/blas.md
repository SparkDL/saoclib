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
y = a ^ b(const)
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
y = abs(a)
```c++
void cblas_sdot(int n, float *sx, float incx, float *sy, float incy)
```

- N
  N is INTEGER
  number of elements in input vector(s)
- SX
  SX is REAL array, dimension ( 1 + ( N - 1 )*abs( INCX ) )
- INCX	
  INCX is INTEGER
  storage spacing between elements of SX
- SY
  SY is REAL array, dimension ( 1 + ( N - 1 )*abs( INCY ) )
- INCY
  INCY is INTEGER
  storage spacing between elements of SY

## cblas_saxpy

```c++
void cblas_saxpy(int n, float *a, float *y)
```
## cblas_sdot

## cblas_sscal

# level2 - matrix x vector operations
## cblas_sgemv


# level3 - matrix operations

## cblas_sgemm
```c++
cblas_gem(CblasColMajor,
```