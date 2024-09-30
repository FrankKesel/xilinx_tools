#include "matmul.h"


int main(){
	
	//The input matrices
	din_t a[3][3] ={{0, 0, 1},
						{0, 1, 0},
						{1, 0 , 0}};
	din_t b[3][3] ={{1, 1, 1},
						{0, 1, 1},
						{0, 0, 1}};
	//The result matrix and the expected result matrix
   dout_t result[3][3], reference[3][3];
   int err_cnt = 0;

   //Generate the expected result by calculating the inner product
   //of a X b
   for(int i = 0; i < A_ROWS; i++) {
      for(int j = 0; j < B_COLS; j++) {
         reference[i][j] = 0;
         // Inner product of a row of A and col of B
         for(int k = 0; k < A_COLS; k++) {
            reference[i][j] += a[i][k] * b[k][j];
         }
      }
   }

   // Run the DUT
   matmul(a, b, result);

   //Compare DUT result against expected result
   for (int i = 0; i < A_ROWS; i++) {
      for (int j = 0; j < B_COLS; j++) {
         // Check HW result against SW
         if (result[i][j] != reference[i][j]) {
            err_cnt++;
         }
      }
   }

   if (err_cnt != 0){
		printf("Test failed  !!!\n");
		err_cnt = 1;
   }
   else{
     	printf("Test passed !\n");
   }

   return err_cnt;
}

