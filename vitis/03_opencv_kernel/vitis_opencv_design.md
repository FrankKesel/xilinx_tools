# Vitis OpenCV Design Tutorial

---

- [Vitis OpenCV Design Tutorial](#vitis-opencv-design-tutorial)
  - [Overview](#overview)
  - [Installation of OpenCV Library and Jupyter Notebook](#installation-of-opencv-library-and-jupyter-notebook)
  - [Design of the OpenCV application with Jupyter Notebook](#design-of-the-opencv-application-with-jupyter-notebook)
  - [IP development in Vitis HLS](#ip-development-in-vitis-hls)
  - [Multiscaler kernel](#multiscaler-kernel)
  - [FPGA programmable logic design in Vitis](#fpga-programmable-logic-design-in-vitis)
  - [SW development with Jupyter notebooks](#sw-development-with-jupyter-notebooks)
  - [SW development with C++](#sw-development-with-c)
  - [Further experiments](#further-experiments)

---
## Overview
* This tutorial is intended to show the complete design flow for a kernel based design using the Xilinx OpenCV library. This tutorial follows the kernel based design flow as described in the tutorial [vitis_kernel_based_design](../vitis_tutorials/vitis_kernel_based_design.md) and thus this tutorial should have been done beforehand. We will reference to this tutorial and describe here only what is necessary and important w.r.t. the OpenCV library and streaming applications with the generated hardware. The tutorial assumes you have a Kria KV260 board with a Ubuntu Linux 22.04 and a host computer running Ubuntu Linux 22.04, with the Xilinx Vitis Core Development Kit (Version 2024.1). 
* The PL hardware which we will design here will support also a streaming application using the Gstreamer framework on the Kria target board. In order to support a streaming application we need to use the extensible platform with the integrated VCU (see tutorial `vitis_extensible_streaming_platform_design.md`) and we need to add in this tutorial an additional IP core, the `multiscaler`. 
* Information on OpenCV can be found here: https://opencv.org Information on the Xilinx OpenCV library can be found [here](https://xilinx.github.io/Vitis_Libraries/vision/2022.1/index.html). The OpenCV library is part of the so-called `Vitis Vision Library` and is contained in the `L1` (Level 1) directory structure. We will use here only the `L1` library with the OpenCV kernels and not the other levels. The OpenCV kernel APIs are documented in [Vitis Vision Library API Reference](https://xilinx.github.io/Vitis_Libraries/vision/2022.1/api-reference.html).
* Not all OpenCV functions are available in the Xilinx OpenCV library and sometimes there are slight differences between the OpenCV library and the Xilinx implementation of the OpenCV functions, concerning functionality and also the result values (e.g. rounding effects). If you want to implement an OpenCV function in hardware you first have to check if the intended function is available in the Xilinx OpenCV library and if it has potential differences in the functionality. Be prepared that you may see slight differences in the pixel values between the two implementations (which will not be the case for the convolution function used in this tutorial). 
* The design flow is as follows:
  * Design the OpenCV application with Python on your laptop/PC using a Jupyter notebook. The notebook will generate the input image data and the reference image data in binary format for use in the HLS testbench. The OpenCV function which we will use in this tutorial will be a simple convolution function.
  * Design the OpenCV kernel with Vitis HLS and test it with a testbench that reads the binary input image and the reference image.
  * Build the hardware bitfile with Vitis and the extensible platform, integrate also the multiscaler kernel. Export the hardware to the Kria KV260 board (running Ubuntu 22.04). 
  * Test the OpenCV kernel on the Kria board with Python and a Jupyter notebook.
  * The generated hardware can then be also used in a further tutorial in a Gstreamer based application (see tutorial `gstreamer_application.md`), which enables real time video streaming.
* This tutorial, which uses the OpenCV convolution function, should serve as a blueprint for the development of other OpenCV based IP cores. Therefore there are some hints in the document w.r.t. the implementation of other OpenCV functions.
* The necessary source codes for this tutorial can be found in the folder `demos/convolution_opencv`. Copy the complete folder hierarchy to your computer and prefereably keep the hierarchy as is. The subfolders are:
  * `data`: Here the testbench data will be stored. There are two Python scripts for data format conversion (if needed).
  * `dtbo`: Here are scripts for the generation of the device tree.
  * `hls`: Script and source files for HLS design of the OpenCV function (convolution).
  * `images`: Image example files with different sizes.
  * `jupyter`: Jupyter notebook for the laptop/PC.
  * `kria`: Jupyter notebooks, C++ application SW and scripts for the Kria KV260 target.
  * `kernels`: Multiscaler kernel needed for streaming applications.
  * `system`: Use this (empty) folder as workspace for the Vitis project.
* The development described here has been tested with Vivado/Vitis Versions 2024.1.
* The Kria KV260 board used is running Ubuntu 22.04.
---
## Installation of OpenCV Library and Jupyter Notebook
* You need an installation of Python including the OpenCV library and Jupyter Notebook on your laptop or PC (host computer preferably running Ubuntu Linux 22.04 with Xilinx Vitis Core Development Kit (Version 2024.1)). You can find information here:
  * Python: https://www.python.org
  * OpenCV: https://docs.opencv.org/4.x/da/df6/tutorial_py_table_of_contents_setup.html
  * Jupyter Notebooks: https://jupyter.org
* For the Jupyter notebook you need the additional packages `numpy`, `matplotlib`, `opencv-python` and `jupyter`. They can normally be installed with the Python package installer `pip`. There's also an Ubuntu package `libgl1`, which needs to be installed on your Ubuntu 22.04 host computer.
* For Vitis HLS you need the Xilinx Vitis Vision Library L1. 

---
## Design of the OpenCV application with Jupyter Notebook
* In the folder `jupyter` you can find a Jupyter notebook. Start Jupyter notebook on your computer which should open in your web browser (use `jupyter notebook` in a terminal to start). Navigate to the folder in the project directory, which you copied from the repository. Open the Jupyter notebook `convolve_image.ipynb`. 
* The Jupyter notebook is commented, so we will be brief here. The OpenCV function is a simple 2D filter, a.k.a. convolution. You can change the filter matrix, for example an edge filter or the simple identity. 
* You can use several images, the images are 8 Bit gray scale images (0: black, 255: white). The image size of the OpenCV kernel has a maximum resolution of full HD (1080x1920 pixel), but can process also smaller images. We suggest to start with a small image, which can be better debugged in case there are errors in the design (which should not be the case in the tutorial). There is `#define`-switch with which the IP core can process 8 pixels in parallel and thus gets a higher throughput. But in this case the width of the image must be a multiple of 8, so not all test images are suited for this case.
* After you have executed the notebook you will have the binary input image and the reference image for the subsequent HLS step in the folder `data`. The notebook generates also a C header file `coeffs.h` for the filter coefficients. Copy this file to the HLS sources in the folder `hls/src`.

---
## IP development in Vitis HLS
* In the folder `hls/project` you can find a bash script for HLS which you can use to setup the project and run it completely. In the HLS `.cfg` file the path to the OpenCV lib is specified, this must be changed to the path where you have copied the OpenCV library. The source codes can be found in the subfolder `src`.
* You will also need to adjust the path to the `data` folder inside the file `project.h`. All necessary parameters can be set in the this file, there is no need to touch the other files, although it may be good to try to understand the code if you want to design your own IP core later on. There is a parameter/define which is called `PARALLEL`: If you uncomment this line, then the IP core is configured to process 8 pixels in parallel. This also will generate a wider bus interface with 64 bit for the input and output image. We suggest you first leave this commented and thus do no parallel processing. You may also have to adapt the image size in `project.h`, depending on the image size you used in the Jupyter notebook.
* The testbench or main function is in the file `conv_main.cpp`. It reads the input image, the reference image and the filter data which have been generated in the previous step. Then the IP kernel is run and the resulting data is compared to the reference data.
  * If there are differences between the IP kernel results and the reference data generated by the Python OpenCV function, then one reason could be differences in the implementation between the standard OpenCV function and the Xilinx implementation, which mostly seems to be rounding effects. There is no way to change this. For the convolution function in this tutorial there should be no differences. 
  * There are two functions for writing out data in binary or in text mode, which can be used to analyze the differences. 
  * If you have differences between the result and reference data you can "cheat" the testbench: Use the result as reference data by copying the binary result data over the reference data. The reason for doing this is the co-simulation: The co-simulation needs a match between result and reference data, otherwise the testbench returns 1 which is interpreted as a failing co-simulation. There are two python scripts in the folder `data` with which you can convert the different image file formats.
* The IP core (kernel) code is in the file `conv_top.cpp`. The interface consists of the two pointers for input and output image and the filter data and the width and height of the actual image. The first three interfaces are AXI-Master interfaces, the two image pointers are bundled together and the filter image is bundled seperately. This is necessary since the image data and the filter data are of different size. Width and height are user registers in the IP core. The IP core thus gets two AXI-Master ports and one AXI-Slave port (for control data and master DMA addresses) for connecting to the processor system. As already mentioned the size of the pointer are changed from 8 bit to 64 bit if you use the parallel processing mode of the OpenCV function.
* There are two functions in the kernel code (`xf::cv::Array2xfMat` and `xf::cv::xfMat2Array`) which copy the interface data to local buffers which are of type `xf::cv::Mat`, this is the standard OpenCV matrix for image data. This is the Xilinx recommended method of converting the interfaces to the arguments of the OpenCV functions. If you want to modify the code for other OpenCV functions you should keep this conversion method as is.
* The IP core/kernel is exported as `Vitis Kernel` as described in the tutorial [vitis_kernel_based_design](../vitis_tutorials/vitis_kernel_based_design.md).

---
## Multiscaler kernel
* This step is optional and only needed if you want to include the convolution kernel in a Gstreamer pipeline.
* The `multiscaler` kernel is part of the `VVAS (Vitis Video Analytics SDK)` framework provided by Xilinx and supports hardware accelerated scaling, color conversion and cropping. The kernel needs to be included in the hardware design in order to implement the color conversion in the Gstreamer pipeline, which will be described in the tutorial `gstreamer_application.md`. Using hardware accelerated color conversion is necessary because if it would be done in software this would be a bottleneck in the video processing pipeline, severely impacting the framerate of the video output.  
* The `multiscaler` kernel is provided as HLS code and can be found in the folder `kernels/multiscaler`.
* The kernel can be configured for specific features by editing the `v_multi_scaler_config.h` header file in the multiscaler directory. By disabling features not needed in the application, the usage of FPGA resources can be reduced. For example, different color formats can be disabled when support for them is not neccessary. For the purpose of this tutorial, the default configuration is sufficient, so no editing is needed. 
* To build the kernel, a `Makefile` is provided in the `multiscaler` directory, which must be executed in a Linux shell (normally `bash` shell by opening the console/terminal).
* In the Makefile, the path to the platform file (`.xpfm`) has to be adjusted to the extensible platform location on your host computer (see tutorial `vitis_extensible_streaming_platform_design.md`). Since the Makefile uses the Vitis `v++` compiler the path to the `v++` executable must be known in the Linux shell. Therefore execute the following command: `source /opt/Xilinx/Vitis/2022.2/settings64.sh`. The path to this script may differ on your host computer, depending on the installation path (for version 2022.1 the path should be `/opt/Xilinx/Vitis/2022.1/settings64.sh`). Check the success by typing `v++` in the shell, which should start the `v++` compiler (it will just show the help menu). 
* After that, the kernel can be built via the make command: `make all` 
* The HLS synthesis may take a while. The resulting kernel `multi_scaler.xo` file can be found in the newly generated `xo` folder. The multiscaler kernel (file `multi_scaler.xo`) has to be generated only once and can be re-used in other streaming projects. You can also copy the `.xo`-file to a different location, if you want.  

---
## FPGA programmable logic design in Vitis
* Follow the steps as described in the tutorial [vitis_kernel_based_design](../vitis_tutorials/vitis_kernel_based_design.md).

---
## SW development with Jupyter notebooks
* You can follow here as well the steps described in the tutorial [vitis_kernel_based_design](../vitis_tutorials/vitis_kernel_based_design.md). We again suggest to make a separate project directory in the directory `/home/root/jupyter_notebooks`. 
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
