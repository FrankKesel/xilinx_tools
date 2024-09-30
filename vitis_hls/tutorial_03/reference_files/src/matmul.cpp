#include "matmul.h"

// This function calculates the inner product of matrix a with matrix b,
// so each row of matrix a must be multiplied with the elements of each
// column in b and then the results of all multiplications must be summed
// (inner product of the row vector of a and the column vector of b)

void matmul(din_t a[A_ROWS][A_COLS], din_t b[B_ROWS][B_COLS],
      dout_t res[A_ROWS][B_COLS]){

	dout_t temp = 0;

	// Iterate over the rows of the a matrix
	Row: for(int i = 0; i < A_ROWS; i++) {
		// Iterate over the columns of the b matrix
		Col: for(int j = 0; j < B_COLS; j++) {
			temp = 0;
			// Inner product of row vector a and column vector b
			Product: for(int k = 0; k < A_COLS; k++) {
				temp += a[i][k] * b[k][j];
			}
			res[i][j] = temp;
		}
	}
}

