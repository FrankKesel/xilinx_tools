#include "accumulator.h"

// The input data d_i and output data d_o are arrays with an organisation
// in channels x samples. So the array index i can be transformed in the 
// corresponding channel index by i%CHANNELS (see testbench).
// The input data is accumulated for each channel.
// The output data is the accumulated channel value for each iteration 
// in the loop.

void accumulator(dout_t d_o[N], din_t d_i[N]) {
	int i, chanId	;
	
	// Local storage for channels
	static dacc_t acc[CHANNELS] = {0,0,0,0,0,0,0,0};

	// Accumulate data for each channel
	L1: for (i=0; i<N; i++) {
		chanId = i%CHANNELS; //Calculate channel index
		acc[chanId] = acc[chanId] + d_i[i]; //Accumulate data
		d_o[i] = acc[chanId];
	}
}
