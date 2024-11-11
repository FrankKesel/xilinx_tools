/**
 * @file host.cpp
 * @author Frank Kesel
 * @date 16 Jul 2024
 * @version 1.0
 * @brief Host application for vector add demo
 * @details
 */

#include <iostream>
#include <string>
#include <chrono>
using namespace std;

// XRT includes
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

// Helper functions and classes for XRT and time measurement
#include "TimeMeasure.h"
#include "xrt_helper.h"

int main( int argc, char *argv[] )
{
    // Check the command line args
	if (argc < 3) {
	    std::cerr << "usage: vadd <xclbin-file> <data-size>\n";
	    return EXIT_FAILURE;
	}

    // Timer for time measurements
    TimeMeasure timer1;

	//Define binary file and device index
    string binaryFile = argv[1];
    int device_index = 0; //Device index should be 0 on the Kria board

    // Define data size from args and vector size for buffers in byte
    int data_size = stoi(argv[2]);
    size_t vector_size_bytes = sizeof(int) * data_size;

    int error_flag = 0; //Error flag

    cout << "-------------------------------------------------------------------"<<endl;
    cout << "-- vadd host program                                             --"<<endl;
    cout << "-------------------------------------------------------------------"<<endl;
    cout << "Load the xclbin: " << binaryFile << endl;

    // Initialize HW kernel, krnl_vadd is the name of the kernel in the FPGA binary (see HLS)
    KernelDevice vaddHW(0, binaryFile, "krnl_vadd");
    // Get the device name
    std::cout << "Device name: " << vaddHW.device.get_info<xrt::info::device::name>() << std::endl;

    // Allocate buffers for the kernel arguments with master interfaces in the
    // same memory bank as the kernel interfaces group id.
    // The group_id is the argument index of the HLS function.
    // The kernel has the following arguments (see HLS):
    // void krnl_vadd(uint32_t* in1, uint32_t* in2, uint32_t* out, int size);
    // in1, in2 and out are master interfaces with memory buffers to be allocated.
    // size is a register in the kernel (slave interface) and needs no buffer.
    KernelBuffer<int> in1(vector_size_bytes, 0, vaddHW); // Buffer for in_image
    KernelBuffer<int> in2(vector_size_bytes, 1, vaddHW); // Buffer for out_image
    KernelBuffer<int> out(vector_size_bytes, 2, vaddHW); // Buffer for filter

    // Create the test data and reference data
    int *bufReference = new int[data_size];
    for (int i = 0; i < data_size; ++i) {
        in1.ptr[i] = i;
        in2.ptr[i] = i;
        out.ptr[i] = 0; //Fill output buffer with 0s
        bufReference[i] = i+i; //Reference data
    }

    // Start timer
    timer1.setStartTime();
    // Synchronize the buffers, i.e. flush caches
    in1.buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    in2.buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    //Execute the kernel: The signature (arguments) of the kernel execution
    //corresponds to the signature of the HLS code of the kernel.
    //The arguments 0-2 are the memory accesses via the buffers,
    //the argument 3 is a scalar argument (slave register).
    auto run = vaddHW.kernel(in1.buffer, in2.buffer, out.buffer, data_size); //Start the kernel
    run.wait(); //Wait for completion of the kernel
 
    // Synchronize output buffer, i.e. flush cache
    out.buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    // Stop timer
    timer1.setStopTime();
    double hwTime = timer1.getTimeMicroSecs();
  
    // Compare device output data with the reference data
    for (int i = 0; i < data_size; ++i) {
    	if(out.ptr[i] != bufReference[i]){
    		error_flag = 1;
    	}
    }

    // Final verdict
    cout << "-------------------------------------------------------------------"<<endl;
    if(error_flag == 1)
    	cout << "Test FAILED!" << endl;
    else
    	cout << "Test PASSED!" << endl;

    //Get some info on execution times
    cout << "-------------------------------------------------------------------"<<endl;
    cout << "Data size transferred (array size): " << data_size << endl;
    cout << "HW execution time: " << hwTime << " us" <<endl;
    cout << "-------------------------------------------------------------------"<<endl;

    return 0;

}
