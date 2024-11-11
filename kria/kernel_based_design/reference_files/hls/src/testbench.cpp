/**
 * @file testbench.cpp
 * @author Frank Kesel
 * @date 29 Nov 2022
 * @version 1.0
 * @brief Vector addition HLS testbench
 * @details
 */


#include <iostream>
#include <iomanip>
using namespace std;
#include <stdint.h>

#include "krnl_vadd.h"

#define RUN_SIZE DATA_SIZE

int main(){
	uint32_t input1[DATA_SIZE];
	uint32_t input2[DATA_SIZE];
	uint32_t output[DATA_SIZE];
	uint32_t reference[DATA_SIZE];
	uint32_t runSize = RUN_SIZE;
	uint8_t error = 0;
	uint32_t i;

	// Initialize input and reference data
	for(i=0; i<DATA_SIZE; i++){
		input1[i] = i;
		input2[i] = i+1;
		reference[i] = 2*i+1;
	}

	// Call the DUT
	krnl_vadd(input1, input2, output, RUN_SIZE);

	// Check result
	for(i=0; i<RUN_SIZE; i++){
		if(output[i] != reference[i])
			error = 1;
	}

	cout << "-----------------------------"<<endl;
	cout << "Tested "<<runSize<<" samples."<<endl;

	if(error == 1){
		cout<<"Test failed!"<<endl;
		return 1;
	}
	else{
		cout<<"Test passed!"<<endl;
	}
	cout << "-----------------------------"<<endl;
}
