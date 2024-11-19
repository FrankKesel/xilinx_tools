# Vitis OpenCV Design Tutorial

---
## Overview
* This tutorial is intended to show the complete design flow for a kernel based design using the _Xilinx Vision Library_, which basically implements _OpenCV_ kernels in hardware. This tutorial follows the kernel based design flow as described in the tutorial [kernel_based_design](../kernel_based_design/kernel_based_design.md) and thus this tutorial should have been done beforehand. We will reference to this tutorial and describe here only what is necessary and important w.r.t. the _Xilinx Vision Library_ and streaming applications with the generated hardware. 
* The tutorial assumes you have a _Kria KV260 Starter Kit_ board with Ubuntu Linux 22.04 and a host computer running Ubuntu Linux 22.04, with the _Xilinx Vitis Core Development Kit_ (Version 2024.1). The FPGA hardware which we will design here will support also a streaming application using the _Gstreamer_ framework on the Kria target board. 
* The Xilinx OpenCV library is part of the so-called _Vitis Vision Library_ and is contained in the _L1_ (Level 1) directory structure. We will use here only the _L1_ library with the OpenCV kernels. 
* Further information on OpenCV and the Xilinx Vision library can be found here:
  * [OpenCV](https://opencv.org )
  * [Xilinx Vision Library](https://xilinx.github.io/Vitis_Libraries/vision/2022.1/index.html)
  * The OpenCV kernel APIs are documented in [Vitis Vision Library API Reference](https://xilinx.github.io/Vitis_Libraries/vision/2022.1/api-reference.html).
* Not all OpenCV functions are available in the Xilinx OpenCV library and sometimes there are slight differences between the OpenCV library and the Xilinx implementation of the OpenCV functions, concerning functionality and also the result values (e.g. rounding effects). If you want to implement an OpenCV function in hardware you first have to check if the intended function is available in the Xilinx OpenCV library and if it has potential differences in the functionality. Be prepared that you may see slight differences in the pixel values between the two implementations (which will not be the case for the convolution function used in this tutorial). 
* The design flow is as follows:
  * Design the OpenCV kernel with Vitis HLS and test it with a C++ testbench that reads the input image. The reference data will be generated in the testbench using the OpenCV software function.
    * _Note_:  In the folder `jupyter` you can find a Jupyter notebook with the same OpenCV function, if you want to play around with the OpenCV function and its parameters.  
  * Build the hardware bitfile with Vitis and the extensible platform. Export the hardware to the Kria KV260 board (running Ubuntu 22.04). 
  * Test the OpenCV kernel on the Kria board with a Jupyter notebook and also with a C++ based application.
  * This tutorial, which uses the OpenCV convolution function, should serve as a blueprint for the development of other OpenCV based IP cores. Therefore there are some hints in the document w.r.t. the implementation of other OpenCV functions.
* The necessary source codes for this tutorial can be found in the folder `reference_files`. Copy the complete folder hierarchy to your computer and  keep the hierarchy as is. Rename the folder to `convolution`. Prefereably you copy the folder to the same destination as the folder for the tutorial [kernel_based_design](../kernel_based_design/kernel_based_design.md). We will also need the same extensible platform. 
* The subfolders are:
  * `dtbo`: Here are scripts for the generation of the device tree.
  * `hls`: Script and source files for HLS design of the OpenCV kernel.
  * `images`: Image example files with different sizes.
  * `jupyter`: Jupyter notebook for the laptop/PC.
  * `kria`: Jupyter notebooks, C++ application SW and scripts for the Kria KV260 target.
  * `system`: Workspace for the Vitis project.
* The development described here has been tested with Vivado/Vitis Versions 2024.1. The Kria KV260 board used is running Ubuntu 22.04.

---
## Installation of Libraries
* You need an installation of the OpenCV C++ library on your development computer. If it is not already installed then we provide instructions on how to install the library on a Linux computer [here](../resources/opencv_installation.md). Further information on the installation of OpenCV for the Vitis Vision Library can be found [here](https://github.com/Xilinx/Vitis_Libraries/tree/main/vision).
* If you want to use the Jupyter notebook for testing the OpenCV convolution function you need an installation of Python including the OpenCV library and Jupyter Notebook on your computer. You can find information here:
  * Python: https://www.python.org
  * OpenCV: https://docs.opencv.org/4.x/da/df6/tutorial_py_table_of_contents_setup.html
  * Jupyter Notebooks: https://jupyter.org
  * For the Jupyter notebook you need the additional packages `numpy`, `matplotlib`, `opencv-python` and `jupyter`. They can normally be installed with the Python package installer `pip`. There's also an Ubuntu package `libgl1`, which needs to be installed on your Ubuntu 22.04 host computer.
* For Vitis HLS you need the _Xilinx Vitis Vision Library L1_. You can find a zip-file with the library [here](../resources/vitis_vision_L1/vision_L1.zip). Download the file and unpack it in the folder, where the HLS projects reside. There are some file paths in the HLS project setup which must match the location, where you copied the library.

---
## Using the Jupyter Notebook
* As we already mentioned this step is optional. It can serve for getting an impression how the convolution filter works and getting familiar with Jupyter notebooks, since we will use them also on the Kria target for testing the OpenCV hardware kernel. The notebook uses the same images as for the HLS testbench.
* In the folder `jupyter` you can find a Jupyter notebook. Start Jupyter notebook on your computer which should open in your web browser (use `jupyter notebook` in a terminal to start). Navigate to the folder in the project directory, which you copied from the repository. Open the Jupyter notebook `convolve_image.ipynb`. 
* The Jupyter notebook is commented, so we will be brief here. The OpenCV function is a simple 2D filter, a.k.a. convolution. You can change the filter matrix, for example an edge filter or the simple identity. 

---
## IP development in Vitis HLS
* In the folder `hls/project` you can find a bash script `run_hls.sh` for HLS which you can use to setup the project and run it completely. The bash script uses the HLS configuration file `project.cfg`. Open this file with a text editor. You should see the specification for the _C flags_  as shown below.
  * The `-I` flag specifies the path to the Vitis Vision L1 library as a relative path. The starting point for this path is the folder `convolution/hls/project` (assuming that your project folder is named _convolution_). This means that the library has been unpacked parallel to the project folder. If you get errors during the execution of the bash script then please check the path to the library.
  * For the `tb.cflags` there is also a second `-I` flag which specifies the path to the OpenCV software library. If you installed the OpenCV library as described [here](../resources/opencv_installation.md), then the library should be found in `/opt/opencv`. 
  * To make it clear: There are **two** OpenCV libraries, the software library installed in `/opt/opencv` and the Vitis L1 library for the OpenCV hardware kernels. The software library is only used in the testbench, it can _not_ be used in the C++ code for synthesis. 
```
syn.cflags=-I../../../vision_L1/include -std=c++14
tb.cflags=-I../../../vision_L1/include -I/opt/opencv/include/opencv4/ -std=c++14
```
* Run the bash script `run_hls.sh`. Check that no errors occurred. If the path to the L1 library is correct you should see no errors. Then open the Vitis GUI and select the folder `convolution/hls` as a workspace. 


* All necessary parameters can be set in the this file, there is no need to touch the other files, although it may be good to try to understand the code if you want to design your own IP core later on. There is a parameter/define which is called `PARALLEL`: If you uncomment this line, then the IP core is configured to process 8 pixels in parallel. This also will generate a wider bus interface with 64 bit for the input and output image. We suggest you first leave this commented and thus do no parallel processing. You may also have to adapt the image size in `project.h`, depending on the image size you used in the Jupyter notebook.
* The testbench or main function is in the file `conv_main.cpp`. It reads the input image, the reference image and the filter data which have been generated in the previous step. Then the IP kernel is run and the resulting data is compared to the reference data.
  * If there are differences between the IP kernel results and the reference data generated by the Python OpenCV function, then one reason could be differences in the implementation between the standard OpenCV function and the Xilinx implementation, which mostly seems to be rounding effects. There is no way to change this. For the convolution function in this tutorial there should be no differences. 
  * There are two functions for writing out data in binary or in text mode, which can be used to analyze the differences. 
  * If you have differences between the result and reference data you can "cheat" the testbench: Use the result as reference data by copying the binary result data over the reference data. The reason for doing this is the co-simulation: The co-simulation needs a match between result and reference data, otherwise the testbench returns 1 which is interpreted as a failing co-simulation. There are two python scripts in the folder `data` with which you can convert the different image file formats.
* The IP core (kernel) code is in the file `conv_top.cpp`. The interface consists of the two pointers for input and output image and the filter data and the width and height of the actual image. The first three interfaces are AXI-Master interfaces, the two image pointers are bundled together and the filter image is bundled seperately. This is necessary since the image data and the filter data are of different size. Width and height are user registers in the IP core. The IP core thus gets two AXI-Master ports and one AXI-Slave port (for control data and master DMA addresses) for connecting to the processor system. As already mentioned the size of the pointer are changed from 8 bit to 64 bit if you use the parallel processing mode of the OpenCV function.
* There are two functions in the kernel code (`xf::cv::Array2xfMat` and `xf::cv::xfMat2Array`) which copy the interface data to local buffers which are of type `xf::cv::Mat`, this is the standard OpenCV matrix for image data. This is the Xilinx recommended method of converting the interfaces to the arguments of the OpenCV functions. If you want to modify the code for other OpenCV functions you should keep this conversion method as is.
* The IP core/kernel is exported as `Vitis Kernel` as described in the tutorial [vitis_kernel_based_design](../02_kernel_based_design/vitis_kernel_based_design.md).


---
## FPGA programmable logic design in Vitis
* Follow the steps as described in the tutorial [vitis_kernel_based_design](../02_kernel_based_design/vitis_kernel_based_design.md).

---
## SW development with Jupyter notebooks
* You can follow here as well the steps described in the tutorial [vitis_kernel_based_design](../02_kernel_based_design/vitis_kernel_based_design.md). We again suggest to make a separate project directory in the directory `/home/root/jupyter_notebooks`. 
* There is a Jupyter notebook `convolution_test.ipynb` in the folder `jupyter_kria` which you can use to check the functionality of the IP kernel on the Kria board and measure the execution time of the IP core. You have to transfer the necessary image files to the project directory on the Kria target. The notebook measures also the execution time of the OpenCV software function. 
* There is also a second notebook `convolution_hd.ipynb` which reads an image from an attached USB camera and applies the convolution to it.

## SW development with C++
* You can find also two C++  applications for testing the kernel.
* ...

---
## Further experiments
* Now that you have worked through a complete flow with the OpenCV IP core we suggest you to play around with the code: 
  * Enable parallel processing for the HD image and compare execution times with the non-parallel implementation and compare also ressource allocations.
  * Change the filter coefficients to try different filters.
* Be aware that you have to go through the complete flow if you change the parallel processing. But you will notice that this can be done quite quickly: 
  * Generate a new data set with the Juypter notebook on the laptop.
  * Run HLS with the new image size.
  * Import the IP core in Vitis and generate the `xclbin`-file. Transfer the file to the Kria board and also the new image data.
  * Test the core with the Jupyter notebook. The experience of the author is that you need approx. half an hour turnaround-time for this (depending on the performance of your laptop).
