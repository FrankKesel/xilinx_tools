/**
 * @file softkernel.cpp
 * @author Frank Kesel / Andreas Schmid
 * @date 12 Dec 2023
 * @version 1.0
 * @brief VVAS Hard Kernel acceleration software library for gstreamer
 * integration with OpenCV function SW implementation
 * @details Implements the necessary API functions for integration of
 * an soft core function in the Xilinx gstreamer framework (see
 * https://xilinx.github.io/VVAS/2.0/build/html/docs/common/Acceleration-Software-Library-Development.html)
 * The necessary API functions are:
 * xlnx_kernel_init()
 * xlnx_kernel_start()
 * xlnx_kernel_done()
 * xlnx_kernel_deinit()
 *
 * The API functions are called with a handle to the kernel
 * (VVASKernel *) which is persistent over the function calls.
 * The xlnx_kernel_init() reads data from a JSON configuration file by
 * using the the jansson lib (https://jansson.readthedocs.io/en/latest/).
 *
 */
#include "../include/common.hpp"
#include "../include/kernel_config.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// When compiling with C++ compiler, declare Core API functions as extern "C" to prevent name mangling
extern "C"
{

int32_t xlnx_kernel_init(VVASKernel *handle)
{
    KernelPriv* kpriv;
    
    // Allocate memory for kernel's private data, i.e. the filter coefficients
    kpriv = (KernelPriv*)calloc(1, sizeof(KernelPriv));

    // If allocation failed, print error message and return failure
    if (!kpriv) 
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, 0, "Error: Unable to allocate memory for filter2d private data!");
        return -1;
    }

//---------------- KERNEL SPECIFIC > ---------------------------------------
    //-------------------------- Load the filter coefficients -------------------------
    coeff_t coeffs = COEFF_VAL;
    // Allocate buffer for coefficients in kernel handle and copy the coefficients to that buffer
    kpriv->filter_coeffs = vvas_alloc_buffer(handle, sizeof(coeff_t), VVAS_INTERNAL_MEMORY, DEFAULT_MEM_BANK, NULL);
    // Copy coefficients to buffer
    memcpy(kpriv->filter_coeffs->vaddr[0], coeffs, sizeof(coeff_t));
//---------------- KERNEL SPECIFIC < ---------------------------------------

    //------------- Read the kernel configuration data from the JSON file ------
    // Currently only an enabling of timing measurements is provided
    // Local JSON variable
    json_t *val;
    json_t* jconfig = handle->kernel_config;

    // Enable time measurement of kernel execution if "timing_enable" is set to true
    val = json_object_get(jconfig, "timing_enable");

    if(!val)
    {
        LOG_MESSAGE(LOG_LEVEL_ERROR, 0, "Error: Could not find \"timing_enable\" object in json file");
        return -1;
    }
    if(!json_is_boolean(val))
    {
        LOG_MESSAGE(LOG_LEVEL_ERROR, 0, "Error: Object \"timing_enable\" not a boolean value!");
        return -1;
    }

    kpriv->timing_enable = json_boolean_value(val);
    // Don't start the global timer yet
    kpriv->global_timer = PerfTimer(false);
    
    // Link the private config to kernel handle
    handle->kernel_priv = (void*)kpriv;

    //------------------------ Pad capabilities --------------------------------
    // Add pad capabilities to the kernels source and sink (only source and sink
    // pad number 0 supported yet)
    kernelcaps *new_caps;
    // Define that pad caps below are rigid
    vvas_caps_set_pad_nature (handle, VVAS_PAD_RIGID);

    //Define caps for variable height and width
    //vvas_caps_new() - Create new caps with input parameters
    // range_height = true : kernel support variable range of height
    // lower_height : this holds the lower value of the height
    // upper_height : this holds the upper value of the height
    // range_width : same as above for width
    // lower_width :
    // upper_width :
    // variable range of formats supported terminated by 0, VVAS_VFMT_Y8 => GRAY8
    new_caps = vvas_caps_new (true, 1, MAX_HEIGHT, true, 1, MAX_WIDTH, VVAS_VFMT_Y8, 0);
    if (!new_caps) {
        LOG_MESSAGE(LOG_LEVEL_ERROR, 0, "failed to create caps");
        return false;
    }
    // Add caps to sink pad
    if (vvas_caps_add_to_sink (handle, new_caps, 0) == false) {
        vvas_caps_free (handle);
        LOG_MESSAGE(LOG_LEVEL_ERROR, 0, "failed to add sink caps");
        return false;
    }
    // Add caps to source pad
    if (vvas_caps_add_to_src (handle, new_caps, 0) == false) {
        vvas_caps_free (handle);
        LOG_MESSAGE(LOG_LEVEL_ERROR, 0, "failed to add source caps");
        return false;
    }
    //Define fixed caps as initial suggestion for caps negotiation
    //lower values are the fixed values, upper values are 0
    new_caps = vvas_caps_new (false, MAX_HEIGHT, 0, false, MAX_WIDTH, 0, VVAS_VFMT_Y8, 0);
    if (!new_caps) {
        LOG_MESSAGE(LOG_LEVEL_ERROR, 0, "failed to create caps");
        return false;
    }
    // Add caps to sink pad
    if (vvas_caps_add_to_sink (handle, new_caps, 0) == false) {
        vvas_caps_free (handle);
        LOG_MESSAGE(LOG_LEVEL_ERROR, 0, "failed to add sink caps");
        return false;
    }
    // Add caps to source pad
    if (vvas_caps_add_to_src (handle, new_caps, 0) == false) {
        vvas_caps_free (handle);
        LOG_MESSAGE(LOG_LEVEL_ERROR, 0, "failed to add source caps");
        return false;
    }

    return 0;
}

int32_t xlnx_kernel_start(VVASKernel *handle, int start, VVASFrame *input[MAX_NUM_OBJECT], VVASFrame *output[MAX_NUM_OBJECT])
{
    KernelPriv* kpriv = (KernelPriv*)handle->kernel_priv;
    
    // Get **virtual** address of input frame
    uint32_t *indata = (uint32_t *) input[0]->vaddr[0];
    // Get height and width of input frame
    uint32_t height = input[0]->props.height;
    uint32_t width = input[0]->props.width;

   //  Get **virtual** address of input frame
    uint32_t *outdata = (uint32_t *) output[0]->vaddr[0];

//---------------- KERNEL SPECIFIC > ---------------------------------------
    // Convert frames to opencv matrices
    Mat src (height, width, CV_8UC1, indata);
    Mat dst (height, width, CV_8UC1, outdata);

    // Convert filter coefficients to opencv matrix
   	const coeff_t* pcoeff = (const coeff_t *) kpriv->filter_coeffs->vaddr[0];
	Mat kernel = Mat (KSIZE, KSIZE, CV_16SC1, (short *) pcoeff);

    // Start timer
    PerfTimer kernel_exec_timer(true);

    // Call OpenCV software function
    filter2D (src, dst, -1, kernel);
//---------------- KERNEL SPECIFIC < ---------------------------------------

    // Get execution time
    double kernel_exec_time_us = (double)((kernel_exec_timer.get().value()).count());

    // If timing is enabled, calculate average kernel execution duration and print every <PRINT_INTERVAL> ms
    if(kpriv->timing_enable)
    {
        kpriv->avrg.update(kernel_exec_time_us);

        if(auto t = kpriv->global_timer.get())
        {
            if(t.value() >= std::chrono::milliseconds(PRINT_INTERVAL))
            {
                std::cout << "Average kernel execution time: " << kpriv->avrg.get() << " us" << std::endl;

                // Reset print timer and average
                kpriv->global_timer = PerfTimer(true);
                kpriv->avrg = RunningAverage<double>();
            }
        }
    }

    return 0;
}

int32_t xlnx_kernel_done(VVASKernel* handle)
{
    // Always return success since vvas_kernel_done() was successful in xlnx_kernel_start() function
    return 0;
}

uint32_t xlnx_kernel_deinit(VVASKernel* handle)
{
    KernelPriv* kpriv = (KernelPriv*) handle->kernel_priv;
     free(kpriv);
    return 0;
}
} // extern "C"
