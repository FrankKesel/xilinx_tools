#ifndef MATMUL_H_
#define MATMUL_H_

#include <stdio.h>
#include <stdint.h>

//Rows and columns of matrix a and b: In order to calculate the inner product
//properly the number of rows of a must match the number of columns of b
//and the number of columns of a must match the number of rows of b
#define A_ROWS 3
#define A_COLS 3
#define B_ROWS A_COLS
#define B_COLS A_ROWS

typedef int8_t din_t;
typedef int16_t dout_t;

void matmul(din_t a[A_ROWS][A_COLS], din_t b[B_ROWS][B_COLS],
      dout_t res[A_ROWS][B_COLS]);

#endif 

