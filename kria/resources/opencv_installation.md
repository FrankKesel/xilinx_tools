# OpenCV Installation

* Install Ubuntu packages: 
  ```
  sudo apt update 
  sudo apt install g++ make cmake build-essential git unzip pkg-config libprotobuf-dev
  sudo apt install libgtk2.0-dev
  ```
* Install opencl packages: 
  ```
  sudo apt install ocl-icd-libopencl1
  sudo apt install opencl-headers 
  sudo apt install ocl-icd-opencl-dev
  ```

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

