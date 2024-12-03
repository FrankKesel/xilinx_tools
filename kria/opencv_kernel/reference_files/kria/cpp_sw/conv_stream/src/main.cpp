/**
 * @file main.cpp
 * @author Frank Kesel
 * @date 03 Dec 2024
 * @version 1.0
 * @brief OpenCV Gstreamer example - Read video frames with OpenCV, stream out with Gstreamer
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <atomic>

// XRT includes
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

#include "project.h"
#include "TimeMeasure.h"
#include "xrt_helper.h"

// Stop the main loop with a thread: 
// This thread will run in parallel with the main
// function. If you press 'q' or 'Q' in the terminal
// of the Kria target, the main loop will stop and
// the program terminates.
std::atomic<bool> stop_flag(false);
void checkForExit() {
    char input;
    std::cin >> input;
    if (input == 'q' || input == 'Q') {
        stop_flag = true;
    }
}

// ---------------- Main function starts here ---------------------------------
int main(int argc, char** argv) {

    //Check the command line args: 
    //1st arg: path to the FPGA binary file
	if (argc < 2) {
	    std::cerr << "usage: conv_stream <xclbin-file>\n";
	    return EXIT_FAILURE;
	}
    //Store path to FPGA binary file 
    std::string binaryFile = argv[1];

    // OpenCV matrices for frames:
    // Input frames, will be initialized by reading the image frames
    cv::Mat in_frame, gray_frame;
    // Output frames, will be initialized to 8 Bit grayscale
    cv::Mat out_frame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);
    // Frame size in bytes (1 byte per pixel)
    size_t frame_size_bytes = sizeof(uint8_t) * HEIGHT * WIDTH;

    //Define convolution kernel data for OpenCV SW function and for HW
    cv::Mat kernel = (cv::Mat_<double>(3,3) << COEFFS); //SW
    int16_t filter_kernel_hw[FSIZE*FSIZE] = {COEFFS}; //HW
    // Filter size in bytes 
    size_t filter_size_bytes = sizeof(int16_t) * HEIGHT * WIDTH;

    std::cout << "------------------- Convolution Video Streaming ---------------------------"<<std::endl;
    // Set OpenCV video writer to feed GST pipeline. The first argument defines the Gstreamer pipeline
    // (see project.h). The third argument is the fourcc code (=0). The last argument specifies that 
    // no color frames are used (the convolution can only accept grayscale images).
    cv::VideoWriter out(GST_PIPELINE, cv::CAP_GSTREAMER, 0, FRAME_RATE, cv::Size(WIDTH, HEIGHT), false);
    if (!out.isOpened()) {
        std::cerr << "Error: Unable to open video writer" << std::endl;
        return -1;
    }
    // Video capture source: integrated laptop cam:
    // Should be /dev/video0 on the Kria board, using V4L2 framework (Video for Linux)
    cv::VideoCapture cap("/dev/video0", cv::CAP_V4L2);   
    if (!cap.isOpened()){
        std::cerr << "Error: Cannot open USB cam." << std::endl;
        return EXIT_FAILURE;
    }
    // Set height and width of video frames
    cap.set(cv::CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, HEIGHT);
    // Get the camera frame rate, it should be 30 fps 
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps == 0) {
        std::cerr << "Error: Could not retrieve frame rate." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Camera frame rate: " << fps << " FPS" << std::endl;
    std::cout << "Image size: " << WIDTH << " x " << HEIGHT << std::endl;

    //---------------- Initialize the FPGA HW ---------------------------------
 #if defined(HW)
    std::cout << "Using HW kernel." << std::endl;
    // Initialize HW kernel, conv_top is the name of the kernel in the FPGA binary (see HLS).
    // The first argument is the device index, it should always be 0 for this tutorial
    KernelDevice convHW(0, binaryFile, "conv_top");
    // Get the device name
    std::cout << "Device name: " << convHW.device.get_info<xrt::info::device::name>() << std::endl;
    // Allocate buffers for the kernel arguments with master interfaces in the
    // same memory bank as the kernel interfaces group id.
    // The group_id is the argument index of the HLS function.
    // The kernel has the following arguments (see HLS):
    // void conv_top(ap_uint<AXI_DATA_WIDTH>* in_image, ap_uint<AXI_DATA_WIDTH>* out_image,
    // short int filter[FSIZE * FSIZE], uint32_t height, uint32_t width)
    // in_image, out_image and filter are master interfaces with memory buffers to be allocated.
    // height and width are registers in the kernel (slave interface) and need no buffers.
    // The size of the image buffer is the number of pixels (1 byte per pixel). The size
    // of the filter buffer is FSIZE*FSIZE (2D filter) with 2 byte per filter value (int16_t).
    KernelBuffer<uint8_t> in_image(frame_size_bytes, 0, convHW); // Buffer for in_image
    KernelBuffer<uint8_t> out_image(frame_size_bytes, 1, convHW); // Buffer for out_image
    KernelBuffer<int16_t> filter(filter_size_bytes, 2, convHW); // Buffer for filter

    // Define a OpenCV image frame (wrapper) and link it to the XRT input buffer using
    // the XRT buffer pointer. The OpenCV wrapper is used to store the 
    // camera image directly in the XRT buffer when reading the cam frames.
    cv::Mat in_image_opencv_wrapper(HEIGHT, WIDTH, CV_8UC1, in_image.ptr);

    // Fill filter via the user pointer of the filter buffer
    for (int i = 0; i < FSIZE*FSIZE; ++i) {
    	filter.ptr[i] = filter_kernel_hw[i];
    }
    // Synchronize the filter buffer, i.e. transfer the data to physical memory
    filter.buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);
#else
    std::cout << "Using SW kernel." << std::endl;
#endif

    // Start the thread to check for exit command
    std::thread exit_thread(checkForExit);
    std::cout << "System running, entering main loop. Stop with <q>."<<std::endl;

    // Set a frame timer
    TimeMeasure timer1;
    // Set a 2nd timer for time measurements in debug mode 
    TimeMeasure timer2;
    // Start the frame timer
    timer1.setStartTime();
 
    //-------------------- Main Loop ------------------------------------------
#ifdef DEBUG
    // During debugging we run only a certain number of frames
    while (timer1.getNumberOfFrames() < DEBUG_FRAMES) {
#else
    // Streaming: Endless loop, quit by pressing 'q' (see checkForExit() thread)
    while (true) {
#endif
#ifdef DEBUG   
       std::cout << "-----------------------------------------------"<<std::endl;
       timer2.setStartTime();
#endif
        // Capture a frame
        cap >> in_frame;
        // Check if frame is not empty
        if (in_frame.empty()) {
            break;
        }
#if defined(HW)
        // Convert the input frame to grayscale and store the result in the
        // XRT input buffer (wrapped as OpenCV matrix)
         cv::cvtColor(in_frame, in_image_opencv_wrapper, cv::COLOR_BGR2GRAY);
#else
        // Convert frame to grayscale frame for SW processing
        cv::cvtColor(in_frame, gray_frame, cv::COLOR_BGR2GRAY);
#endif
#ifdef DEBUG   
        timer2.setStopTime();
        std::cout << "Frame Capture time: "<< timer2.getTimeMicroSecs() << " us" << std::endl; 
#endif

        //---------------- FPGA HW processing ---------------------------------
#if defined(HW)
#ifdef DEBUG   
        timer2.setStartTime();
#endif
         //Synchronize the XRT input image buffer, i.e. transfer data to kernel memory
        in_image.buffer.sync(XCL_BO_SYNC_BO_TO_DEVICE);       
        //Execute the kernel: The arguments must appear in the order as they 
        //appear in the corresponding HLS function. These are the input image,
        //the output image, the filter and height and width of the image.
        //The first three arguments are the XRT buffers and the last two are scalar arguments.
        auto run = convHW.kernel(in_image.buffer, out_image.buffer, filter.buffer, HEIGHT, WIDTH);
        run.wait(); //Wait for completion of the kernel
        // Synchronize the XRT output image buffer, i.e. transfer data from kernel memory
        out_image.buffer.sync(XCL_BO_SYNC_BO_FROM_DEVICE); 
        // Use the XRT buffer user pointer out_image.ptr to link the data 
        // to the OpenCV image out_frame (needed for GStreamer backend) 
        out_frame.data = out_image.ptr;
#ifdef DEBUG       
        timer2.setStopTime();
        std::cout << "HW processing time: "<< timer2.getTimeMicroSecs() << " us" << std::endl; 
#endif
        //----------------- OpenCV SW processing ------------------------------
#else
#ifdef DEBUG   
        timer2.setStartTime();
#endif
        // Call the OpenCV filter function
        //cv::filter2D(gray_frame, out_frame, CV_8U, kernel);
        cv::filter2D(gray_frame, out_frame, -1 , kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
#ifdef DEBUG   
        timer2.setStopTime();
        std::cout << "SW processing time: "<< timer2.getTimeMicroSecs() << " us" << std::endl; 
#endif

#endif
#ifdef DEBUG   
        timer2.setStartTime();
#endif
        // Write the frame to the OpenCV video writer output, this
        // feeds the Gstreamer pipeline and the frame is effectively
        // send via RTP to the destination.
        out.write(out_frame);
        // Increment frame counter (to get the number of frames)
        timer1.incFrameCount(); 
        // Check if the user has requested to stop (checkForExit thread)
        if (stop_flag) {
            break;
        }
#ifdef DEBUG   
        timer2.setStopTime();
        std::cout << "GStreamer backend processing: "<< timer2.getTimeMicroSecs() << " us" <<std::endl; 
#endif

    }
    //----------------------- Final processing --------------------------------
    // Stop the frame timer
    timer1.setStopTime();
     std::cout << "Number of frames processed: " << timer1.getNumberOfFrames() << std::endl;
    std::cout << "Total time in main loop: " << timer1.getTimeSecs() << " sec" << std::endl; 
    std::cout << "Frame rate: " << timer1.getFrameRate() << " FPS" << std::endl;
    std::cout << "------------ Finished -----------------"<<std::endl;

    // Release the capture object 
    cap.release();
    // Wait for the exit thread to finish
    exit_thread.join();

    return 0;
}
