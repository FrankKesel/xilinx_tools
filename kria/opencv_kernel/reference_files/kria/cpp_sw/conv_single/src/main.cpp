/**
 * @file host.cpp
 * @author Frank Kesel
 * @date 25 Nov 2024
 * @version 1.0
 * @brief Host application for convolution demo
 * @details A test image is read and processed via OpenCV SW function as well
 * as processed via the OpenCV HW kernel. Both resulting images are compared
 * in order to verify that the HW kernel works correctly.
 */

#include <iostream>
#include <string>
#include <chrono>
using namespace std;

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// XRT includes
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

#include "project.h"
#include "TimeMeasure.h"
#include "xrt_helper.h"

int main( int argc, char *argv[] )
{

    //Check the command line args: 
    //1st arg: path to the FPGA binary file
    //2nd arg: path to the input image file
	if (argc < 3) {
	    std::cerr << "usage: conv_single <xclbin-file> <input image>\n";
	    return EXIT_FAILURE;
	}

    //Input image file
    string inputImageFile = argv[2];

    // Timer for time measurements
    TimeMeasure timer1;

    //Define convolution kernel data for OpenCV SW function and for HW
    cv::Mat kernel = (cv::Mat_<double>(FSIZE, FSIZE) << COEFFS);
    short int filter[FSIZE*FSIZE] = {COEFFS};

    cout << "-------------------------------------------------------------------"<<endl;
    cout << "-- convolution host program                                      --"<<endl;
    cout << "-------------------------------------------------------------------"<<endl;

    //Read input image as grayscale image
    cv::Mat inImg = cv::imread(inputImageFile, cv::IMREAD_GRAYSCALE);
    //Check that file could be opened
    if (inImg.data == NULL) {
        std::cerr << "Cannot open image at:" << inputImageFile << "\n";
        return EXIT_FAILURE;
    }
    std::cout << "Load the input image: " << inputImageFile << std::endl;
    //Get size of image
    int inHeight = inImg.rows;
    int inWidth = inImg.cols;
    int pixels = inHeight * inWidth;
	cout << "Image height: "<< inHeight <<endl;
	cout << "Image width: "<< inWidth <<endl;
	cout << "Number of pixels: "<< pixels <<endl;
    cout << "-------------------------------------------------------------------"<<endl;

//--------------------------- SW test -------------------------------------------------------------
	//Create memory for reference image
    cv::Mat ocvRefImage(inHeight, inWidth, CV_8U);

    //Generate reference image by OpenCV filter2D function
    //See: https://docs.opencv.org/4.x/d4/dbd/tutorial_filter_2d.html
    timer1.setStartTime();
    cv::filter2D(inImg, ocvRefImage, -1 , kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
    timer1.setStopTime();
    double swTime = timer1.getTimeMicroSecs();

//--------------------------- HW test -------------------------------------------------------------
	//Define binary file and device index
    std::string binaryFile = argv[1];
    std::cout << "Load the xclbin: " << binaryFile << std::endl;

    // Initialize HW kernel, conv_top is the name of the kernel in the FPGA binary (see HLS)
    KernelDevice convHW(0, binaryFile, "conv_top");
    // Get the device name
    std::cout << "Device name: " << convHW.device.get_info<xrt::info::device::name>() << std::endl;

    // Allocate buffers for the kernel arguments with master interfaces in the
    // same memory bank as the kernel interfaces group id.
    // The group_id (2nd arg) is the argument index of the HLS function.
    // The kernel has the following arguments (see HLS):
    // void conv_top(ap_uint<DATA_WIDTH>* in_image, ap_uint<DATA_WIDTH>* out_image,
	//	   short int filter[FSIZE * FSIZE], uint32_t height, uint32_t width)
    // in_image, out_image and filter are master interfaces with memory buffers to be allocated.
    // height and width are registers in the kernel (slave interface) and need no buffer.
    KernelBuffer<uint8_t> inImgBuf(pixels, 0, convHW); // Buffer for in_image
    KernelBuffer<uint8_t> outImgBuf(pixels, 1, convHW); // Buffer for out_image
    KernelBuffer<uint16_t> filterBuf(FSIZE*FSIZE, 2, convHW); // Buffer for filter
    
    // Link OpenCV Mat to the XRT buffers in order to use them as OpenCV mat
    cv::Mat outImg(inHeight, inWidth, CV_8U, outImgBuf.ptr);
    cv::Mat hwInImg(inHeight, inWidth, CV_8U, inImgBuf.ptr);
    // Copy input image to kernel input buffer
    inImg.copyTo(hwInImg);    
 
	//Copy the filter data to the device memory addressed by user pointer
    //and synchronize buffer
    for (int i = 0; i < FSIZE*FSIZE; ++i) {
    	filterBuf.ptr[i] = filter[i];
    }
    filterBuf.buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    timer1.setStartTime();
    // Synchronize the input buffer (i.e. flush caches)
    inImgBuf.buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    //Execute the kernel: The signature (arguments) of the kernel execution
    //corresponds to the signature of the HLS code of the kernel.
    //The arguments 0-2 are the memory accesses via the buffers,
    //the argument 3 and 4 is a scalar argument (slave register).
    auto run = convHW.kernel(inImgBuf.buffer, outImgBuf.buffer, filterBuf.buffer, inHeight, inWidth ); 
    run.wait(); //Wait for completion of the kernel 
    // Synchronize output buffer (i.e. flush caches), result is in 
    // OpenCV Mat container outImg, since outImgBuf is linked to the OpenCV Mat
    outImgBuf.buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    timer1.setStopTime();
    double hwTime = timer1.getTimeMicroSecs();
  
//--------------------------- Compare results --------------------------------------------------------
    //Calculate the difference of the reference and output image and store in new image
    cv::Mat diff(inHeight, inWidth, CV_8U);
    absdiff(ocvRefImage, outImg, diff);
    imwrite(RES_IMG, outImg);
    imwrite(REF_IMG, ocvRefImage);
    imwrite(DIFF_IMG, diff);
 
    // Get the number of different pixels and the minumum and maximum pixel difference
    int error_flag = 0; //Error flag
    int mindiff = 255, maxdiff = 0, pixeldiff = 0;
    int diffpixels = 0;
    for (int i = 0; i < inHeight; i++) {
        for (int j = 0; j < inWidth; j++) {
        	pixeldiff = int(diff.at<uchar>(i, j));
            if (pixeldiff > 0) diffpixels++;
            if (mindiff > pixeldiff) mindiff = pixeldiff;
            if (maxdiff < pixeldiff) maxdiff = pixeldiff;
        }
    }
    cout << "-------------------------------------------------------------------"<<endl;
    cout << "Number of pixel differences: "<<diffpixels<<endl;
    cout << "Min pixel diff: "<<mindiff<<", Max pixel diff: "<<maxdiff <<endl;
    // calculate the percentage of the number of different pixels
    float err_per = 100.0 * (float)diffpixels / pixels;
    cout << "Relative number of pixel differences: "<<err_per<<" %"<<endl;
    cout << "-------------------------------------------------------------------"<<endl;
    cout << "SW execution time: " << swTime << " us" << endl; 
    cout << "HW execution time: " << hwTime << " us" <<endl;
 
    // Final verdict
    cout << "-------------------------------------------------------------------"<<endl;
     //Generate final verdict
    if (maxdiff > MAXDIFF) {
    	cout<<"DUT test failed!"<<endl;
    } else {
    	cout<<"DUT test successful!"<<endl;
    }


    return 0;

}
