/**
 * @file project.h
 * @author Frank Kesel
 * @date 19 Nov 2024
 * @version 1.2
 * @brief Project definitions
 * @details
 */

#ifndef PROJECT_H_
#define PROJECT_H_

// Standard includes
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;


//Include HLS and XF headers
#include "hls_stream.h"
#include "ap_int.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_custom_convolution.hpp"
#include "imgproc/xf_box_filter.hpp"

//----------------------Define parameters and file paths-----------------
//Parallel processing: If used (=1) 8 pixels will be processed in parallel,
#define PARALLEL 1

// IMG_SEL: 0: 64x64 image, 1: 600x800 image, 2: 1080x1920 image
#define IMG_SEL 2

// Define filter coefficients (effectively a 3x3 matrix)
// Identity
//#define COEFFS 0, 0, 0, 0, 1, 0, 0, 0, 0
// Sobel filter (edge detection)
#define COEFFS 1, 0, -1, 2, 0, -2, 1, 0, -1
// Laplace filter (edge detection)
//#define COEFFS 0, 1, 0, 1, -4, 1, 0, 1, 0

//Define compare values for testbench
#define MAXDIFF 1 //Absolute difference per pixel

//Input images with different sizes (selected with IMG_SEL)
//Make sure the folder "images" is defined in project.cfg!
#if IMG_SEL == 0
#define SRC_IMG "images/image_64x64.bmp"
static constexpr auto  IMG_HEIGHT = 64;
static constexpr auto  IMG_WIDTH = 64;
#elif IMG_SEL == 1
#define SRC_IMG "images/image_880x656.bmp"
static constexpr auto IMG_HEIGHT = 656;
static constexpr auto  IMG_WIDTH = 880;
#elif IMG_SEL == 2
#define SRC_IMG "images/image_1920x1080.bmp"
static constexpr auto IMG_HEIGHT = 1080;
static constexpr auto  IMG_WIDTH = 1920;
#endif

// Output image files (to be found in the folder Output/csim/build)
#define OPENCV_REF "opencv_ref_image.bmp"
#define HLS_RESULT "hls_result_image.bmp"
#define DIFF_IMAGE "diff_image.bmp"

//Set the maximum resolution of the IP core
static constexpr auto MAX_HEIGHT = 1080;
static constexpr auto  MAX_WIDTH = 1920;

//Filter size and filter radius. 
static constexpr auto  FSIZE = 3;
static constexpr auto  FRADIUS = 1;

//--------------No change needed below this line--------------------------
//OpenCV image data types: 8 Bit gray pixel (0 .. 255)
#define TYPE XF_8UC1

//Definition of parallel processing
#if PARALLEL
//8 pixels are processed in parallel, data width of interface is 64 Bit (8x 8 Bit)
#define NPC XF_NPPC8
static constexpr auto AXI_DATA_WIDTH = 64;
#else
//No parallel processing of pixels, min data width = 8 for XF_8UC1
#define NPC XF_NPPC1
static constexpr auto AXI_DATA_WIDTH = 8;
#endif

// Bit width of a single pixel (should always be 8 Bit for gray images)
static constexpr auto PIXELWIDTH = XF_PIXELWIDTH(TYPE, NPC);

// Co-Simulation DEPTH: Number of bytes in image / Number of bytes per AXI bus transfer
static constexpr int COSIM_DEPTH = (IMG_HEIGHT * IMG_WIDTH * PIXELWIDTH/8) / (AXI_DATA_WIDTH/8);

#endif /* SRC_PROJECT_H_ */
