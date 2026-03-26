# OpenCV Installation

## opencv 4.4.0 / Vitis 2025.1 / Ubuntu 24.04

1. Install Ubuntu packages: `sudo apt update && apt install g++ make cmake build-essential git unzip pkg-config libprotobuf-dev`
1. If multiple versions of openCL are used install packages `ocl-icd-libopencl1, opencl-headers` and `ocl-icd-opencl-dev`
1. Clone OpenCV into a temporary folder
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
1. Compile and install OpenCV to `/opt/opencv`
	```	
	export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/:$LIBRARY_PATH
	source /opt/Xilinx/2025.1/Vitis/settings64.sh

	mkdir opencv_build
	cd opencv_build

	cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/opt/opencv -D WITH_V4L=ON -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules -DBUILD_TESTS=OFF -DBUILD_ZLIB=ON -DBUILD_JPEG=ON -DWITH_JPEG=ON -DWITH_PNG=ON -DBUILD_EXAMPLES=OFF -DINSTALL_C_EXAMPLES=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DWITH_OPENEXR=OFF -DBUILD_OPENEXR=OFF -D CMAKE_CXX_COMPILER=$XILINX_HLS/tps/lnx64/gcc-8.3.0/bin/g++ ../opencv

	make -j8
	sudo make install
	```	
1. Configure dynamic linker run-time bindings: `sudo echo /opt/opencv/lib > /etc/ld.so.conf.d/opencv.conf && ldconfig -v` 


Links:

- <https://adaptivesupport.amd.com/s/article/Vitis-Libraries-Compiling-and-Installing-OpenCV?language=en_US>
- <https://support.xilinx.com/s/feed/0D54U00005mjpjtSAA?language=en_US>
- <https://support.xilinx.com/s/article/Fatal-error-sys-cdefs-h-No-such-file-or-directory?language=en_US>
