/**
 * @file project.h
 * @author Frank Kesel
 * @date 25 Nov 2024
 * @version 1.0
 * @brief Host application for convolution demo
 * @details
 */

// Define image format for cam
#define WIDTH 1920
#define HEIGHT 1080
//#define WIDTH 640
//#define HEIGHT 480
//#define WIDTH 320
//#define HEIGHT 240

// Define if SW or HW kernel is used (comment out for SW kernel)
#define HW

// Define filter coefficients 
#define COEFFS 0, 1, 0, 1, -4, 1, 0, 1, 0  //Edge filter
//#define COEFFS 0, 0, 0, 0, 1, 0, 0, 0, 0 //Identity

// Define filter size (can not be changed, see HLS code)
#define FSIZE 3

// Define Gstreamer pipelines: Frames are sent via RTP to the host
// Adapt the IP address to the IP address of the receiving computer 
#define GST_PIPELINE "appsrc  ! videoconvert ! rtpvrawpay ! udpsink host=192.168.178.62 port=50000"

// Define frame rate (should be 30 fps)
#define FRAME_RATE 30