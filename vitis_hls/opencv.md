# OpenCV

## Installing OpenCV library 4.4.0 on the host computer

* In order to use the OpenCV library in Vitis HLS an installation of the standard OpenCV library is needed on the host computer. This must be version 4.4.0. Following are the steps to install OpenCV on the host computer.
* For installation hints see https://xilinx.github.io/Vitis_Libraries/vision/2022.1/tutorial.html
* Install Ubuntu packages: `sudo apt update && apt install g++ make cmake build-essential git unzip pkg-config libprotobuf-dev`
* If multiple versions of openCL are used install packages `ocl-icd-libopencl1, opencl-headers` and `ocl-icd-opencl-dev`, see https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/OpenCL-Installable-Client-Driver-Loader?tocId=ervPBb7z4ZP_dkCp9hc0dw
* Clone OpenCV into a temporary folder
	```
	git clone https://github.com/opencv/opencv.git
	cd opencv
	git checkout 4.4.0
	cd ..
	git clone https://github.com/opencv/opencv_contrib.git
	cd opencv_contrib
	git checkout 4.4.0
	cd ..
	```	
* Compile and install OpenCV to `/opt/opencv`
	```	
	export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/:$LIBRARY_PATH
	export CPATH="/usr/include/x86_64-linux-gnu"

	mkdir opencv_build
	cd opencv_build

	cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/opt/opencv -D WITH_V4L=ON -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules -DBUILD_TESTS=OFF -DBUILD_ZLIB=ON -DBUILD_JPEG=ON -DWITH_JPEG=ON -DWITH_PNG=ON -DBUILD_EXAMPLES=OFF -DINSTALL_C_EXAMPLES=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_OPENEXR=OFF -DBUILD_OPENEXR=OFF -D CMAKE_CXX_COMPILER=/opt/Xilinx/Vitis_HLS/current/tps/lnx64/gcc-6.2.0/bin/g++ ../opencv

	make -j8
	sudo make install
	```	
* Configure dynamic linker run-time bindings: `sudo echo /opt/opencv/lib > /etc/ld.so.conf.d/opencv.conf && ldconfig -v` 

## Downloading the Xilinx Vitis Libraries
* Download the Xilinx Vitis Libraries from https://github.com/Xilinx/Vitis_Libraries/tree/main
  * Make sure you download the same version as the Vitis version (2024.1)
* Unpack the zip file. The library needed for OpenCV is in the directory `/vision/L1`. You can just keep this directory or the whole Xilinx library.

## 


Links:

- <https://support.xilinx.com/s/feed/0D54U00005mjpjtSAA?language=en_US>
- <https://support.xilinx.com/s/article/Fatal-error-sys-cdefs-h-No-such-file-or-directory?language=en_US>
- <https://support.xilinx.com/s/article/Vitis-2021-1-Libraries-Compiling-and-Installing-OpenCV?language=en_US>