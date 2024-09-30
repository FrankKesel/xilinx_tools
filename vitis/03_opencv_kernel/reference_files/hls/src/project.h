/**
 * @file project.h
 * @author Frank Kesel
 * @date 27 Mar 2023
 * @version 1.1
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
//Generate debug data, i.e. text output files for image and reference
//Comment out if not needed
#define DEBUG

//Image size: Set height and width of the image, MUST correspond
//to the size of the test image, for parallel execution of the core
//(8 pixels parallel) the width of the image must be a multiple of 8!
static constexpr auto  IMG_HEIGHT = 64;
static constexpr auto  IMG_WIDTH = 64;
//static constexpr auto IMG_HEIGHT = 656;
//static constexpr auto  IMG_WIDTH = 880;
//static constexpr auto IMG_HEIGHT = 1080;
//static constexpr auto  IMG_WIDTH = 1920;

//Set the maximum resolution of the IP core
static constexpr auto MAX_HEIGHT = 1080;
static constexpr auto  MAX_WIDTH = 1920;

//Parallel processing: If used 8 pixels will be processed in parallel,
//the bus interface will be 64 bit wide (8 x 8 Bit), if commented
//out there will be no parallel processing. If parallel processing is used
//then the width of the image MUST be a multiple of 8, otherwise the result
//may be incorrect.
#define PARALLEL

//Define paths to the data files
// Define path to image data (add the path to the testbench, see .tcl script)
#define PATH "/home/Data/Design/sopc/kria/05_rs_lab_projects/convolution_opencv/data/"
#define IMAGE PATH "input_image.bin"
#define REF_IMAGE PATH "ref_image.bin"
#define RESULT_IMAGE_TXT PATH "result.txt"
#define RESULT_IMAGE_BIN PATH "result.bin"
#define REF_IMAGE_TXT PATH "ref_image.txt"

//Filter size and filter radius. MUST match the filter in Jupyter notebook!
static constexpr auto  FSIZE = 3;
static constexpr auto  FRADIUS = 1;

//--------------No change needed below this line--------------------------

//OpenCV image data types: 8 Bit greyscale pixel (0 .. 255)
#define TYPE8 XF_8UC1 //8 Bit unsigned

//Definition of parallel processing
#ifdef PARALLEL
//8 pixels are processed in parallel, data width of interface is 64 Bit
#define PIXELS_PER_WORD 8
#define NPC XF_NPPC8
#define DATA_WIDTH 64
#else
//No parallel processing of pixels
#define PIXELS_PER_WORD 1
#define NPC XF_NPPC1
#define DATA_WIDTH 8
#endif

static constexpr int DEPTH = IMG_HEIGHT*IMG_WIDTH/PIXELS_PER_WORD;

#endif /* SRC_PROJECT_H_ */
