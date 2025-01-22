/**
 * @file xrt_helper.h
 * @author Frank Kesel
 * @date 05 Jul 2024
 * @version 1.0
 * @brief Helper functions for Xilinx XRT drivers
 * @details
 */

#ifndef xrt_helper_h
#define xrt_helper_h


// XRT includes
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"


// Struct for FPGA device and kernel
// initialization by constructor, see xrt_helper.cpp
struct KernelDevice
{
    xrt::device device; //Device
    xrt::kernel kernel; //Kernel
    // Constructor
    KernelDevice(int deviceIndex, std::string binFileName, std::string kernelName);
};

// Struct for kernel buffer
template <typename T>
struct KernelBuffer
{
    T *ptr; // User pointer to buffer
    xrt::bo buffer; //XRT buffer
    // Constructor
    KernelBuffer(int size, int groupID, KernelDevice kernelDevice){
        buffer = xrt::bo(kernelDevice.device, size, kernelDevice.kernel.group_id(groupID));
        ptr = buffer.map<T*>(); //Map buffer to user pointer
    };
};




#endif
