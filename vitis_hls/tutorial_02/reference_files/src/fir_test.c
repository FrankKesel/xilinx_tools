#include <stdio.h>
#include <math.h>
#include "fir.h"

#define SAMPLES 25

int main () {
  FILE         *fp;

  int signal, output;
  int taps[N] = {0,-10,-9,23,56,63,56,23,-9,-10,0};

  int i, i_gold, output_gold, error_flag;

  signal = 0;
  error_flag = 0;

  fp=fopen("out.gold.dat","r");

  //Apply input signal to FIR filter and store output
  for (i=0; i<=SAMPLES; i++) {
    //Read the input signal from golden device file
    fscanf(fp,"%d %d %d\n",&i_gold,&signal,&output_gold);

	// Call the DUT function
    fir(&output,taps,signal);

    //Compare the output
	if(output_gold != output){
      error_flag = 1;
      printf("Error: sample=%d, input=%d, expected output=%d, "\
			 "DUT output=%d \n",\
             i, signal, output_gold, output);
	}
  }

  fclose(fp);

  if(error_flag == 1){
    printf("**********************************************\n");
    printf("FAIL: Output does not match the golden output!\n");
    printf("**********************************************\n");
    return 1;
  } else {
    printf("*******************************************\n");
    printf("PASS: The output matches the golden output!\n");
    printf("*******************************************\n");
    return 0;
  }
}
