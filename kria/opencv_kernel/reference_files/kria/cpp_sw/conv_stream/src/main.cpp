/**
 * @file main.cpp
 * @author Frank Kesel
 * @date 06 Jun 2024
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

#include "TimeMeasure.h"

// Define image format for cam
#define WIDTH 640
#define HEIGHT 480
//#define WIDTH 320
//#define HEIGHT 240

// Define if SW or HW kernel is used
#define HW

// Define filter coefficients (effectively a 3x3 matrix)
#define COEFFS 0, 1, 0, 1, -4, 1, 0, 1, 0
//#define COEFFS 0, 0, 0, 0, 1, 0, 0, 0, 0
// Define filter size
#define FSIZE 3

// Define Gstreamer pipelines: Frames are sent via RTP to the host
// No H.264 encoding, runs without Docker container
#define GST_PIPELINE "appsrc  ! videoconvert ! rtpvrawpay ! udpsink host=192.168.178.62 port=50000"
// With H.264 encoding, needs Docker container
//#define GST_PIPELINE "appsrc  ! videoconvert ! omxh264enc target-bitrate=3000 periodicity-idr=250 ! rtph264pay ! udpsink host=192.168.178.62  port=50000 sync=false"

// Stop the main loop with a thread
std::atomic<bool> stop_flag(false);
void checkForExit() {
    char input;
    std::cin >> input;
    if (input == 'q' || input == 'Q') {
        stop_flag = true;
    }
}


int main(int argc, char** argv) {

	//Define binary file and device index
    std::string binaryFile = "/home/ubuntu/projects/hw/convolution/binary_container_1.xclbin";
    int device_index = 0; //Device index should be 0 on the Kria board

    // Frame size
    int pixels = HEIGHT * WIDTH;
 
    // OpenCV mat for frame
    cv::Mat in_frame, gray_frame;
    cv::Mat out_frame = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);

    //Define convolution kernel data for OpenCV SW function and for HW
    cv::Mat kernel = (cv::Mat_<double>(3,3) << COEFFS);
    short int filter[FSIZE*FSIZE] = {COEFFS};

    std::cout << "------------------- Convolution Video Streaming ---------------------------"<<std::endl;

    // GStreamer pipeline for video output
    std::string output_pipeline = GST_PIPELINE;
    // Set OpenCV video writer to feed GST pipeline, grayscale frames
    cv::VideoWriter out(output_pipeline, cv::CAP_GSTREAMER, 0, 30.0, cv::Size(WIDTH, HEIGHT), false);
    if (!out.isOpened()) {
        std::cerr << "Error: Unable to open video writer" << std::endl;
        return -1;
    }

    // Video capture source: integrated laptop cam
    cv::VideoCapture cap(0 + cv::CAP_V4L2);
    if (!cap.isOpened()){
        fprintf(stderr, "Cannot open USB cam\n");
        return 0;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, HEIGHT);

    // Get the camera frame rate
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps == 0) {
        std::cerr << "Error: Could not retrieve frame rate." << std::endl;
        return -1;
    }
    std::cout << "Camera frame rate: " << fps << " FPS" << std::endl;

  
 #if defined(HW)
    std::cout << "Using HW kernel." << std::endl;
    //Open device:
    //Get the kernel object from xclbin. Load the xclbin file and get UUID.
    //The UUID is needed to open the kernel from the device.
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin: " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);
    // Get the device name
    std::cout << "Device name: " << device.get_info<xrt::info::device::name>() << "\n";
    //Then get the kernel object from the UUID, "conv_top" is the name of the kernel (see Vitis)
    auto krnl = xrt::kernel(device, uuid, "conv_top");

    // Allocate buffers for the kernel arguments with master interfaces in the
    // same memory bank as the kernel interfaces group id.
    auto bo_in = xrt::bo(device, pixels, krnl.group_id(0));
    auto bo_out = xrt::bo(device, pixels, krnl.group_id(1));
    auto bo_filter = xrt::bo(device, FSIZE*FSIZE, krnl.group_id(2));

    // Data transfer between host and device by the buffer map API:
    // Map the host side backing pointer of the buffer to the user pointers,
    // then use the user pointers to fill the buffers with test data.
    auto bo_in_map = bo_in.map<uint8_t*>();
    auto bo_out_map = bo_out.map<uint8_t*>();
    auto bo_filter_map = bo_filter.map<int16_t*>();

    // Fill filter
    for (int i = 0; i < FSIZE*FSIZE; ++i) {
    	bo_filter_map[i] = filter[i];
    }
    bo_filter.sync(XCL_BO_SYNC_BO_TO_DEVICE);
#else
    std::cout << "Using SW kernel." << std::endl;
#endif

    // Start the thread to check for exit command
    std::thread exit_thread(checkForExit);

    std::cout << "System running, entering main loop. Stop with <q>."<<std::endl;

    // Set a frame timer
    TimeMeasure timer1;
    timer1.setStartTime();

    // Endless loop, quit by pressing 'q'
    while (true) {
        // Capture a frame
        cap >> in_frame;
        if (in_frame.empty()) {
            break;
        }
        // Convert frame to grayscale
        cv::cvtColor(in_frame, gray_frame, cv::COLOR_BGR2GRAY);

#if defined(HW)
	    //Copy the frame to the memory addressed by user pointer
        std::memcpy(bo_in_map, gray_frame.data, HEIGHT*WIDTH);
        bo_in.sync(XCL_BO_SYNC_BO_TO_DEVICE);

        //Execute the kernel: 
        auto run = krnl(bo_in, bo_out, bo_filter, HEIGHT, WIDTH ); //Start the kernel
        run.wait(); //Wait for completion of the kernel

        // Transfer device data to host using the sync API and copy to output frame
        bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        std::memcpy(out_frame.data, bo_out_map, HEIGHT*WIDTH);
#else
        cv::filter2D(gray_frame, out_frame, CV_8U, kernel);
#endif

        // Write the frame to the output
        out.write(out_frame);

        // Increment frame counter
        timer1.incFrameCount();
 
        // Check if the user has requested to stop
        if (stop_flag) {
            break;
        }
    }
    timer1.setStopTime();
    std::cout << "Number of frames processed: " << timer1.getNumberOfFrames() << std::endl;
    std::cout << "Execution time of main loop: " << timer1.getTimeSecs() << " sec" << std::endl; 
    std::cout << "Frame rate: " << timer1.getFrameRate() << " FPS" << std::endl;
    std::cout << "------------ Finished -----------------"<<std::endl;

    // Release the capture object and close all OpenCV windows
    cap.release();
    cv::destroyAllWindows();

   // Wait for the thread to finish
    exit_thread.join();

    return 0;
}
