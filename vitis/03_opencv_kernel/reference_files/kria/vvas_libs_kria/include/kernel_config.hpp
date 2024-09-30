/**
 * @file kernel_config.hpp
 * @author Andreas Schmid / Frank Kesel
 * @date 12 Dec 2023
 * @version 1.0
 * @brief Configuration for the VVAS Hard Kernel acceleration software library
 * @details Configures IP core specific parameters.
 *
 */
#ifndef __KERNEL_CONFIG_HPP__
#define __KERNEL_CONFIG_HPP__

extern "C" {
#include <vvas/vvaslogs.h>
#include <vvas/vvas_kernel.h>
}
#include <iostream> 
// Include common classes for timing measurement
#include "common.hpp"

// Print interval for timing measurement in Milliseconds
#define PRINT_INTERVAL 10000

// Define max values for width and height that the IP core can handle.
// This is used during caps negotiation.
#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080

// Use this if debugging via cout is needed, otherwise comment it out
//#define DEBUG

//---------------- KERNEL SPECIFIC > ---------------------------------------
// Here kernel specific configurations and data can be defined (e.g.
// filter coefficients). Please be aware that this implies that also at
// other places in the VVAS lib changes / adaptions have to be made, especially
// in the file hardkernel.cpp. The places will be marked by a similar commment.
// Size of filter kernel, must match the size specified in HLS kernel
#define KSIZE 9
// Define filter coefficients
typedef const short int coeff_t[KSIZE];
// Include header file with coefficients
#include "coeffs.h"
//---------------- KERNEL SPECIFIC < ---------------------------------------


// Structure for storing private kernel data that should be persistent
// accross library function calls
struct KernelPriv{
//---------------- KERNEL SPECIFIC > ---------------------------------------
    // Filter coefficients (loaded in xlnx_kernel_init())
    VVASFrame* filter_coeffs = nullptr;
//---------------- KERNEL SPECIFIC < ---------------------------------------

    // Members for kernel timing statistics
    bool timing_enable = false;
    PerfTimer global_timer;
    RunningAverage<double> avrg;
};

#endif //__KENREL_CONFIG_HPP__
