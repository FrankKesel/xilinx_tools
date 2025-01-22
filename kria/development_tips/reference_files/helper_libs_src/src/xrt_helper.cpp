/**
 * @file xrt_helper.cpp
 * @author Frank Kesel
 * @date 05 Jul 2024
 * @version 1.0
 * @brief Helper functions for Xilinx XRT drivers
 * @details
 */

#include "xrt_helper.h"

KernelDevice::KernelDevice(int deviceIndex, std::string binFileName, std::string kernelName){
    //Get the kernel object from xclbin. Load the xclbin file and get UUID.
    //The UUID is needed to open the kernel from the device.
    device = xrt::device(deviceIndex);
    auto uuid = device.load_xclbin(binFileName);
    //Then get the kernel object from the UUID, kernelName is the name of the kernel (see Vitis)
    kernel = xrt::kernel(device, uuid, kernelName);
}


