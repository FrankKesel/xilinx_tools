/**
 * @file hardkernel.cpp
 * @author Andreas Schmid / Frank Kesel
 * @date 12 Dec 2023
 * @version 1.0
 * @brief VVAS Hard Kernel acceleration software library for gstreamer
 * integration of HLS IP cores
 * @details Implements the necessary API functions for integration of
 * an HLS IP core in the Xilinx gstreamer framework (see
 * https://xilinx.github.io/VVAS/2.0/build/html/docs/common/Acceleration-Software-Library-Development.html)
 * The necessary API functions are:
 * xlnx_kernel_init()
 * xlnx_kernel_start()
 * xlnx_kernel_done()
 * xlnx_kernel_deinit()
 *
 * The API functions are called with a handle to the kernel data structure
 * (VVASKernel *) which is persistent over the function calls.
 * The xlnx_kernel_init() reads data from a JSON configuration file by
 * using the the jansson lib (https://jansson.readthedocs.io/en/latest/).
 *
 */


#include "../include/common.hpp"
#include "../include/kernel_config.hpp"
#include <cstring>
#include <iostream>

// When compiling with C++ compiler, declare Core API functions as extern "C"
// to prevent name mangling
extern "C"
{

/*
 * @brief Kernel init API function
 * @param handle: persistent VVASKernel handle
 * @return : 0 on success or -1 on failure
 * @details
 * This API is called by the VVAS infrastructure plug-in only once at the
 * time of plug-in initialization. Acceleration software library can
 * perform all the one-time initialization in this function.
 * The most relevant members of the VVASKernel handle passed in for initialization are:
 *  json_t* kernel_config: Pointer to the configuration data provided in the JSON file.
 *  void* kernel_priv: Used to store custom data that can later be retrieved in other functions.
 *   Usually, memory for that data is dynamically allocated in the init-function
 *   to store JSON configuration values.
 *  uint8_t is_multiprocess: Should be set to 1 (true) in hard kernels,
 *   since the kernel will be operated in XRT managed mode.
 * Configuration data is read from a JSON file, which is given by the call in
 * the gstreamer pipeline (see e.g. bash script for gstreamer). JSON data
 * is read using the JANSSON lib.
 * The pad capabilities for gstreamer are also set here.
*/
int32_t xlnx_kernel_init(VVASKernel *handle)
{

#ifdef DEBUG
    std::cout << "------------------------------- Kernel init ----------------------------" << std::endl;
#endif

    // Define a pointer to data structure for the private kernel data (defined
    // in kernel_config.hpp)
    KernelPriv* kpriv;
    // Allocate memory for private kernel data structure, which is passed later
    // to the kernel handle
    kpriv = (KernelPriv*)calloc(1, sizeof(KernelPriv));

    // If allocation failed, print error message and return failure
    if (!kpriv)
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, 0, "Error: Unable to allocate memory for kernel private data!");
        return -1;
    }

//---------------- KERNEL SPECIFIC > ---------------------------------------
    //-------------------------- Load the filter coefficients ------------------
    coeff_t coeffs = COEFF_VAL;
    // Allocate buffer for coefficients in kernel handle and copy the
    // coefficients to that buffer
    kpriv->filter_coeffs = vvas_alloc_buffer(handle, sizeof(coeff_t), VVAS_INTERNAL_MEMORY, DEFAULT_MEM_BANK, NULL);
    // Copy coefficients to buffer
    memcpy(kpriv->filter_coeffs->vaddr[0], coeffs, sizeof(coeff_t));
//---------------- KERNEL SPECIFIC < ---------------------------------------

    //-------------------------- Set multiprocess mode -------------------------
    // Set multiprocess mode to true to use the kernel in XRT managed mode
    // This is important to use the vvas_kernel_start() and vvas_kernel_done()
    // APIs in the xlnx_kernel_start() function later on
    handle->is_multiprocess = true;

    //------------- Read the kernel configuration data from the JSON file ------
    // Currently only an enabling of timing measurements is provided
    // Local JSON variable
    json_t *val;
    // Pointer to the JSON configuration data provided in the JSON file.
    json_t* jconfig = handle->kernel_config;

    // Get the timing variable from JSON
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

    // Set timing enable to boolean value from JSON file
    kpriv->timing_enable = json_boolean_value(val);
    // Initialize a global timer, but dont start it
    kpriv->global_timer = PerfTimer(false);

    //--------------------- Link the kernel private data to the kernel handle --
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


#ifdef DEBUG
    std::cout << "------------------------End of  Kernel init ----------------------------" << std::endl;
#endif

    return 0;
}

/*
 * @brief Kernel start API function
 * @param handle: VVAS kernel handle which has kernel context
 * @param start: flag to indicate start of the kernel. Mainly useful in streaming kernel mode
 * @param input: Array of input frames populated in VVASFrame handle
 * @param output: Array of output frames populated in VVASFrame handle
 * @return : 0 on success or -1 on failure
 * @details This API is called by infrastructure plug-in for each input
 *  frame/buffer it has received for further processing by the kernel
 *
*/
int32_t xlnx_kernel_start(VVASKernel *handle, int start, VVASFrame *input[MAX_NUM_OBJECT], VVASFrame *output[MAX_NUM_OBJECT])
{
    // Get a pointer to the private data in the kernel handle
    KernelPriv* kpriv = (KernelPriv*)handle->kernel_priv;

//---------------- KERNEL SPECIFIC > ---------------------------------------
    // Get the filter coefficients
    VVASFrame *coeffs = kpriv->filter_coeffs;
    // Get **physical** address of coefficient buffer
    uint32_t coeff_data = (uint32_t)coeffs->paddr[0];
//---------------- KERNEL SPECIFIC < ---------------------------------------

    //-------------------- Get input and output frame pointer ------------------
    // Get **physical** address of input frame, needed by IP kernel.
    // Index of input frame is 0 (only 1 plane)
    // Address is a 64 bit address in the Kria system. Since the IP kernel
    // uses a 32 bit address register (see HLS) we convert it here to 32 bit.
    uint32_t indata = (uint32_t)input[0]->paddr[0];
    // Get height and width of input frame
    uint32_t height = input[0]->props.height;
    uint32_t width = input[0]->props.width;
    // Get **physical** address of output frame, needed by IP kernel
    uint32_t outdata = (uint32_t)output[0]->paddr[0];

    //--------- Start perf timer for kernel execution time ---------------------
    PerfTimer kernel_exec_timer(true);

//---------------- KERNEL SPECIFIC > ---------------------------------------
    // Start kernel execution on PL side
    // String must correspond to the order and type of the arguments passed to
    // the kernel (corresponds to the HLS code):
    // p = Pointer, u = unsigned int (uint32_t)
    // For a full list of arguments see here:
    // https://xilinx.github.io/VVAS/3.0/build/html/docs/common/Acceleration-Software-Library-Development.html
    int ret = vvas_kernel_start(handle, "pppuu", indata, outdata, coeff_data, height, width);
//---------------- KERNEL SPECIFIC < ---------------------------------------

    if (ret < 0) 
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, 0, "Failed to issue kernel start command");
        return ret;
    }

    // Wait for kernel completion
    ret = vvas_kernel_done (handle, 1000);

    if (ret < 0) 
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, 0, "Failed to receive response from kernel");
        return ret;
    }

    // Get execution time
    double kernel_exec_time_us = (double)((kernel_exec_timer.get().value()).count());

    // If timing is enabled, calculate average kernel execution duration and print
    // every <PRINT_INTERVAL> ms
    if(kpriv->timing_enable)
    {
        // Update average kernel execution time
        kpriv->avrg.update(kernel_exec_time_us);

        // Get global timer value
        if(auto t = kpriv->global_timer.get())
        {
            // If global timer is larger than print interval
            if(t.value() >= std::chrono::milliseconds(PRINT_INTERVAL))
            {
                std::cout << "Average kernel execution time: " << kpriv->avrg.get() << " us" << std::endl;

                // Reset global timer and average
                kpriv->global_timer = PerfTimer(true);
                kpriv->avrg = RunningAverage<double>();
            }
        }
    }

    return 0;
}

/*
 * @brief Kernel done API function
 * @param handle: VVAS kernel handle which has kernel context
 * @return : 0 on success or -1 on failure
 * @details To know whether the kernel has finished processing the current
 * frame/buffer, infrastructure plug-in or an application will call this API.
*/
int32_t xlnx_kernel_done(VVASKernel* handle)
{
    // Always return success since vvas_kernel_done() was successful in xlnx_kernel_start() function
    return 0;
}

/*
 * @brief Kernel deinit API function
 * @param handle: VVAS kernel handle which has kernel context
 * @return : 0 on success or -1 on failure
 * @details This API is called by the infrastructure plug-in when plug-in is
 * de-initializing. Acceleration software library must perform any clean-up,
 * de-initialization tasks such as freeing private handles and internal memory
 * allocation as part of the library initialization process.
*/
uint32_t xlnx_kernel_deinit(VVASKernel* handle)
{
    KernelPriv* kpriv = (KernelPriv*) handle->kernel_priv;
//---------------- KERNEL SPECIFIC > ---------------------------------------
//---------------- KERNEL SPECIFIC < ---------------------------------------

    free(kpriv);
    return 0;
}
} // extern "C"
