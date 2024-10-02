#include "accumulator.h"
 
int main () {

	// Input array:
	// Channels:    C0  C1  C2  C3  C4  C5  C6  C7
	// Sample:		0	0	0	0	0	0	0	0
	//				1	1	1	1	1	1	1	1
	//				2	2	2	2	2	2	2	2
	//				3	3	3	3	3	3	3	3
	din_t d_i[N];
					
	//Output array (organization same as input array) 
	dout_t d_o[N];
	
	int i, error_flag=0, retval=0;
	int in, ref;
	dout_t output_gold[N];
	FILE        *fp;

	fp=fopen("result.golden.dat","r");

	//Fill input array and golden output
	for (i=0; i<N; i++) {
	    fscanf(fp,"%d %d\n", &in, &ref);
	    d_i[i] = (din_t)in;
	    output_gold[i] = (dout_t)ref;
	}

	// Execute the DUT
	accumulator(d_o, d_i);

	//Compare results
	for (i=0; i<N; i++) {
	    if(output_gold[i] != d_o[i]){
	        error_flag = 1;
	        printf("Error: sample=%d, input=%d, expected output=%d, "\
	  			 "DUT output=%d \n",\
	               i, d_i[i], output_gold[i], d_o[i]);
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
