/**
 * @file project.h
 * @author Frank Kesel
 * @date 25 Nov 2024
 * @version 1.0
 * @brief Host application for convolution demo
 * @details
 */


// Define paths to reference image, output image and diff image
// (will be stored in the folder where the program executes)
#define REF_IMG "ref.bmp"
#define RES_IMG "res.bmp"
#define DIFF_IMG "diff.bmp"

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
