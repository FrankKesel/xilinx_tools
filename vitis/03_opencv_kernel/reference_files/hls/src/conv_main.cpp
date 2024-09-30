/**
 * @file conv_main.cpp
 * @author Frank Kesel
 * @date 27 Mar 2023
 * @version 1.1
 * @brief Convolution HLS Testbench
 * @details
 */

#include "conv_top.h"
#include "tb_helpers.h"
#include "coeffs.h"

int main(int argc, char *argv[]){

	int  sum=0; //Error sum

	//Matrices for input and output to DUT
	//Qualified as static, since they might be large,
	//and are stored in static segment not on stack
	static ap_uint<8> in_mat[IMG_HEIGHT][IMG_WIDTH];
	static ap_uint<8> ref_mat[IMG_HEIGHT][IMG_WIDTH];
	static ap_uint<8> out_mat[IMG_HEIGHT][IMG_WIDTH];

	//Convolution filter matrix (stored as array)
	//Get initialization from include file
	short int filter[FSIZE*FSIZE] = COEFF_VAL;

	cout << "---  Convolution HLS testbench  ----"<<endl;
	cout << "Image height: "<< IMG_HEIGHT <<endl;
	cout << "Image width: "<< IMG_WIDTH <<endl;

	//Load filter, input and ref image
	read_image<IMG_HEIGHT, IMG_WIDTH>(IMAGE, in_mat);
	read_image<IMG_HEIGHT, IMG_WIDTH>(REF_IMAGE, ref_mat);

	//Run the DUT
	conv_top((ap_uint<DATA_WIDTH>*)&in_mat[0][0], (ap_uint<DATA_WIDTH>*)&out_mat[0][0],
			filter, IMG_HEIGHT, IMG_WIDTH);

	//Calculate difference of pixels
	sum = compare_image<IMG_HEIGHT, IMG_WIDTH>(out_mat, ref_mat);

#ifdef DEBUG
	//Write to output file for debugging
	write_image<IMG_HEIGHT, IMG_WIDTH>(RESULT_IMAGE_TXT, out_mat);
	write_image_bin<IMG_HEIGHT, IMG_WIDTH>(RESULT_IMAGE_BIN, out_mat);
	write_image<IMG_HEIGHT, IMG_WIDTH>(REF_IMAGE_TXT, ref_mat);
#endif

	//If pixel sum of diff image is > 0, then the DUT is wrong
	if(sum > 0){
		cout << "DUT test failed! Diff="<< sum <<endl;
		return 1;
	}
	else{
		cout << "DUT test successful! Diff="<< sum <<endl;
		return 0;
	}

}


