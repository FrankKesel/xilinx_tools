/**
 * @file conv_top.h
 * @author Frank Kesel
 * @date 19 Nov 2024
 * @version 1.2
 * @brief Convolution HLS IP Core
 * @details
 */

#ifndef CONV_TOP_H_
#define CONV_TOP_H_

#include "project.h"

/**
 * @brief Convolve IP Core
 * @details Input and output of the IP core are simple matrices
 * 		in memory, accessed by a pointer. We do not use OpenCV
 * 		mat as input or output, since there is an overhead in
 * 		signals or flags and the burst optimization is not possible
 * 		if an AXI Master IF should be used.
 * @param in_image: Pointer to input image
 * @param out_image: Pointer to output image
 * @param filter: convolution filter matrix
 * @param height: height of actual image
 * @param width: width of actual image
*/
void conv_top(ap_uint<AXI_DATA_WIDTH>* in_image, ap_uint<AXI_DATA_WIDTH>* out_image,
		short int filter[FSIZE * FSIZE], uint32_t height, uint32_t width);

#endif /* CONV_H_ */
