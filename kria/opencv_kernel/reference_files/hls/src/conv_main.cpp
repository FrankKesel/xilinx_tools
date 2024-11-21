/**
 * @file conv_main.cpp
 * @author Frank Kesel
 * @date 19 Nov 2024
 * @version 1.2
 * @brief Convolution HLS Testbench
 * @details
 */

// OpenCV Includes (include only in testbench, not in kernel code!)
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "conv_top.h"

int main(int argc, char *argv[]){
    //Define convolution kernel data for OpenCV SW function and for HW
    cv::Mat kernel = (cv::Mat_<double>(FSIZE, FSIZE) << COEFFS); //SW
    short int filter[FSIZE*FSIZE] = {COEFFS}; //HW

    //Define OpenCV Mat objects for image data
    cv::Mat in_img, out_img, ocv_ref, diff;
    //Read input image as grayscale image
    in_img = cv::imread(SRC_IMG, cv::IMREAD_GRAYSCALE);
    //Check that file could be opened
    if (in_img.data == NULL) {
        fprintf(stderr, "Cannot open image at %s\n", SRC_IMG);
        return 0;
    }

    //Get size of image
    int in_height = in_img.rows;
    int in_width = in_img.cols;
    int pixels = in_height * in_width;
	cout << "---  Convolution HLS testbench  ----"<<endl;
	cout << "Image height: "<< in_height <<endl;
	cout << "Image width: "<< in_width <<endl;
	cout << "Number of pixels: "<< pixels <<endl;

	//Create memory for reference image, output image and difference image
    ocv_ref.create(in_img.rows, in_img.cols, CV_8U);
    out_img.create(in_img.rows, in_img.cols, CV_8U);
    diff.create(in_img.rows, in_img.cols, CV_8U);

    //Generate reference image by OpenCV convolution filter function
     cv::filter2D(in_img, ocv_ref, -1 , kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

	//Run the DUT
	conv_top((ap_uint<AXI_DATA_WIDTH>*)in_img.data, (ap_uint<AXI_DATA_WIDTH>*)out_img.data,
	 		filter, in_height, in_width);

	//Calculate the difference of the reference and output image and store in new image
    absdiff(ocv_ref, out_img, diff);
    //Write images for debugging purposes
    imwrite(OPENCV_REF, ocv_ref);
    imwrite(HLS_RESULT, out_img);
    imwrite(DIFF_IMAGE, diff); 

    // Get the number of different pixels and the minumum and maximum pixel difference
    int mindiff = 255, maxdiff = 0, pixeldiff = 0;
    int diffpixels = 0;
    for (int i = 0; i < diff.rows; i++) {
        for (int j = 0; j < diff.cols; j++) {
        	pixeldiff = int(diff.at<uchar>(i, j));
            if (pixeldiff > 0) diffpixels++;
            if (mindiff > pixeldiff) mindiff = pixeldiff;
            if (maxdiff < pixeldiff) maxdiff = pixeldiff;
        }
    }
    cout<<"Number of pixel differences: "<<diffpixels<<endl;
    cout<<"Min pixel diff: "<<mindiff<<", Max pixel diff: "<<maxdiff <<endl;
    // calculate the percentage of the number of different pixels
    float err_per = 100.0 * (float)diffpixels / pixels;
    cout<<"Relative number of pixel differences: "<<err_per<<" %"<<endl;

    //Generate final verdict
    if (maxdiff > MAXDIFF) {
    	cout<<"DUT test failed!"<<endl;
        return 1;
    } else {
    	cout<<"DUT test successful!"<<endl;
    	return 0;
    }

}


