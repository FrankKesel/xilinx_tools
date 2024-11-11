/**
 * @file conv_top.cpp
 * @author Frank Kesel
 * @date 27 Mar 2023
 * @version 1.0
 * @brief Convolution HLS IP Core
 * @details
 */

#include "conv_top.h"

void conv_top(ap_uint<DATA_WIDTH>* in_image, ap_uint<DATA_WIDTH>* out_image,
		short int filter[FSIZE * FSIZE], uint32_t height, uint32_t width)
{
// Define the pragmas for the interfaces:
// filter: AXI Slave IF
// in_image, out_image and filter: AXI Master IF (offset in slave register)
// width, height: AXI Slave IF (registers)
// Note: The depth parameter is for co-simulation and must match
// the size of the arrays. The DATA_WIDTH of the input and output pointer is also set
// in the project.h file and depends if parallel processing is used, in this case
// the width is 64 bit otherwise 8 bit.
#pragma HLS INTERFACE mode=m_axi bundle=filter depth=9 port=filter
#pragma HLS INTERFACE mode=m_axi bundle=image depth=DEPTH port=out_image offset=slave
#pragma HLS INTERFACE mode=m_axi bundle=image depth=DEPTH port=in_image offset=slave
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE mode=s_axilite port=width
#pragma HLS INTERFACE mode=s_axilite port=height

// Use dataflow optimization
#pragma HLS dataflow

	// Define local buffers for input and output image
    xf::cv::Mat<TYPE8, MAX_HEIGHT, MAX_WIDTH, NPC> src(height, width);
	xf::cv::Mat<TYPE8, MAX_HEIGHT, MAX_WIDTH, NPC> dst(height, width);

    // Retrieve xf::cv::Mat objects from in_image data:
    xf::cv::Array2xfMat<DATA_WIDTH, TYPE8, MAX_HEIGHT, MAX_WIDTH, NPC>(in_image, src);

    // Call the OpenCV filter2D function
	xf::cv::filter2D<XF_BORDER_CONSTANT, FSIZE, FSIZE, TYPE8, TYPE8,
		MAX_HEIGHT,	MAX_WIDTH, NPC> (src, dst, filter, 0);

    // Convert _dst xf::cv::Mat object to out_image:
    xf::cv::xfMat2Array<DATA_WIDTH, TYPE8, MAX_HEIGHT, MAX_WIDTH, NPC>(dst, out_image);

}
