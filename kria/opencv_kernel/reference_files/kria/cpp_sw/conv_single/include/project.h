/**
 * @file project.h
 * @author Frank Kesel
 * @date 28 May 2024
 * @version 1.0
 * @brief Host application for convolution demo
 * @details
 */

// Define path to FPGA binary
#define HW_PATH "/home/ubuntu/projects/hw/convolution/conv_hw.xclbin"

// Define paths to input image, reference image and output image
#define PATH "/home/ubuntu/projects/hw/convolution/images/"
//#define SRC_IMG PATH "image_64x64.bmp"
//#define SRC_IMG PATH "image_880x656.bmp"
#define SRC_IMG PATH "image_1920x1080.bmp"
#define REF_IMG PATH "ref.bmp"
#define RES_IMG PATH "res.bmp"
#define DIFF_IMG PATH "diff.bmp"

// Define filter coefficients (effectively a 3x3 matrix)
// Identity
//#define COEFFS 0, 0, 0, 0, 1, 0, 0, 0, 0
// Sobel filter (edge detection)
//#define COEFFS 1, 0, -1, 2, 0, -2, 1, 0, -1
// Laplace filter (edge detection)
#define COEFFS 0, 1, 0, 1, -4, 1, 0, 1, 0

// Maximum difference of pixel values between OpenCV reference and kernel result
#define MAXDIFF 0

// Define filter size
#define FSIZE 3
