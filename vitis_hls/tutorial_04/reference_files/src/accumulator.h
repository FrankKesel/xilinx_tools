#ifndef ACCUMULATOR_H_
#define ACCUMULATOR_H_
 
#include <stdio.h>
#include <stdint.h>

typedef int16_t din_t;
typedef int32_t dout_t;
typedef int32_t dacc_t;

#define CHANNELS 8
#define SAMPLES  4
#define N CHANNELS * SAMPLES

void accumulator(dout_t d_o[N], din_t d_i[N]);

#endif
