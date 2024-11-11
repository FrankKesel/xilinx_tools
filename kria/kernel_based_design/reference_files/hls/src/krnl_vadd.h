/**
 * @file krn_vadd.h
 * @author Frank Kesel
 * @date 29 Nov 2022
 * @version 1.0
 * @brief Vector addition HLS demo
 * @details
 */


#ifndef SRC_KRNL_VADD_H_
#define SRC_KRNL_VADD_H_

// Define the data size for vector addition
#define DATA_SIZE 1024

// TRIPCOUNT identifier for HLS to be able to calculate tripcounts
const int c_size = DATA_SIZE;

void krnl_vadd(uint32_t* in1, uint32_t* in2, uint32_t* out, int size);

#endif /* SRC_KRNL_VADD_H_ */
