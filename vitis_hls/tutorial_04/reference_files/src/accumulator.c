#include "accumulator.h"

void accumulator(dout_t d_o[CHANNELS], din_t d_i[N]) {

	int i, j;
	
	static dout_t accu=0;

    L0: for (i=0; i<CHANNELS; i++){
        L1: for(j=0; j<SAMPLES; j++){
            accu += d_i[j*CHANNELS + i];
        }
        d_o[i] = accu;
    }
}
