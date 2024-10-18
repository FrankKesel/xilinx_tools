#include "accumulator.h"
 
int main () {

	// Input array
	din_t d_i[N];					
	//Output array
	dout_t d_o[CHANNELS];
    //Reference array
    dout_t ref[CHANNELS];

    dout_t accu = 0; 
	int i, j, error_flag=0, retval=0;
	int in;
    int k;

	//Fill input array 
	for (i=0; i<N; i++) {
	    d_i[i] = i; 
	}
    //Generate reference data
    for (i=0; i<CHANNELS; i++){
        for(j=0; j<SAMPLES; j++){
            accu += d_i[j*CHANNELS + i];
        }
        ref[i] = accu;
    }

	// Execute the DUT
	accumulator(d_o, d_i);

	//Compare results
	for (i=0; i<CHANNELS; i++) {
	    if (ref[i] != d_o[i] )
            error_flag = 1;
	}

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
