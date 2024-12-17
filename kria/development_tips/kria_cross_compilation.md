# Kria Cross-Compilation

## Overview
* In the Kria tutorials we developed the necessary C++ based applications directly on the Kria target. Although this has some advantages it can be slow for larger programs since the compilation directly on the Kria target is rather slow compared to the compilation on a laptop or PC. A so-called _cross compilation_ compiles the software on the host computer. In this document we describe the necessary steps to install a cross compilation toolchain and using VS Code for cross development (compiling, linking, running and debugging on the target).
* As in the Kria tutorials we will use again VS Code with CMake. You can re-use the `CMakeLists.txt` files from the tutorial projects. You only have to add the path to the _sysroot_ directory to the `CMakeLists.txt` files. Since we are using a small library in the tutorials you also have to cross-compile the library with the same scripts by adding again the _sysroot_ directory to the `CMakeLists.txt` files.
* We also assume that your host computer is running Linux, preferably Ubuntu Linux 22.04 (or a derivative).

---
## Toolchain Installation
* Install the cross compiler toolchain for ARM 64 bit systems (_aarch64_) on the Linux host: `sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu`
* Check version: `aarch64-linux-gnu-gcc --version`
  * If you installed the toolchain under Ubuntu Linux 22.04 you should have the version 11.4.0.

---
## Download and Install Sysroot
* For cross compiling a so-called _sysroot_ is needed  which is a copy of the necessary directories and files from the root file system of the target. The _sysroot_ is needed for compilation of the source code and linking of the program executable, since it will be executed on the Kria target system which is a ARM 64 bit system. 
* You can download a _sysroot_ for the Ubuntu Linux which runs on the Kria board from the following website: https://ubuntu.com/download/amd
	* Scroll down on this website to the section _Ubuntu Desktop 22.04 LTS_  and find the _Sysroot for cross-compilation_ file `iot-limerick-kria-classic-server-2204-classic-22.04-kd05-20240223-170-sysroot.tar.xz`. 
* Download the file and unpack it on your computer. You will later on need the path to the sub-directory (the sysroot path) `/iot-limerick-kria/aarch64-xilinx-linux` in the `CMakeLists.txt` files.

---
## Install Sysroot from Kria Ubuntu
* You can also extract the root file system from the Ubuntu on the Kria target, although we recommend to use the official sysroot (as described in the previous section).
* Insert Kria SD-Card with installed Ubuntu in Host PC (running Ubuntu)
* Copy the rootfs to the Host PC: 
	* `rsync -avz <sd_rootfs> <sysroot_path>/`
	* `<sd_rootfs>`: path to the SD card rootfs (the root of the filesystem)
	* `<sysroot_path>`: path where the rootfs is stored on the Host PC
* Convert absolute links in the rootfs to relative links:
	* Get conversion script: `wget https://raw.githubusercontent.com/abhiTronix/rpi_rootfs/master/scripts/sysroot-relativelinks.py`
	* `sudo chmod +x sysroot-relativelinks.py`
	* `./sysroot-relativelinks.py <sysroot_path>`

---
## VS Code Cross Compile with CMake
* Setup a VS Code project with CMake as usual (see e.g. [kernel based design tutorial](../kernel_based_design/kernel_based_design.md#sw-development-in-c-on-the-kria-target)). Bear in mind that in this case the project is not remote on the Kria target but local on your development computer, so we do not have to connect to the Kria target.
* In the `CMAkeLists.txt` file add the Sysroot path with: `set(CMAKE_SYSROOT <sysroot_path>)`
  * The `<sysroot_path>` is the directory path where you installed the _sysroot_ directory (see section [Download and Install Sysroot](kria_cross_compilation.md#download-and-install-sysroot))
* Below you can see an excerpt from a `CMakeLists.txt` file showing the definition of the _sysroot_ path (replace the `path to sysroot` with the path on your computer). You also have to set the path to the cross-compiled library.
```
# Set the project name = name of executable
set(PROJ_NAME conv_single)

# Define minimum required cmake version and
# project name, version and language (C++)
cmake_minimum_required(VERSION 3.18)
project(${PROJ_NAME} VERSION 1.0 LANGUAGES CXX)

# Define SYSROOT for cross compile
set(CMAKE_SYSROOT <path to sysroot>/iot-limerick-kria/aarch64-xilinx-linux)

# Define path to helpers library (must be cross compiled for aarch64!)
set(HELPER_INCLUDE <path to lib>/cpp_libs/helper_libs/include)
set(HELPER_LIBS <path to lib>/cpp_libs/helper_libs/lib/libhelper.a)

# Search OpenCV package and include dirs
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Set the paths to the include directories
set(XRT_INCLUDE ${CMAKE_SYSROOT}/usr/include/xrt)
# Set necessary project libraries (for linking)
set(XRT_LIBS xrt_coreutil)

...
```
* When the path to the _sysroot_ is set correctly the build system should find the libraries (e.g. OpenCV and XRT) in the sysroot directories. When you see compiler or linker errors then first check the path. Also check that libraries which you generated yourself (e.g. the _helpers_ library in the example) have been also cross-compiled.
* In the CMake tab when starting to compile you will need to configure the _kit_. Here you select the installed cross compiler toolchain, i.e. `GCC 11.4.0 aarch64-linux-gnu` (depending on the version you installed). Then build the executable as usual. 
* After building you must transfer the binary executable to the Kria target system and execute it there.

---
## Remote debugging with gdb
* If you want to debug your code, you must do a _remote_ debug on the target system. This can also be done in VS Code with the settings given in the following. We will use a _gdbserver_ on the target which will run the executable and we will connect to the server from the _gdb client_ in VS Code.
* Install gdbserver on target: `sudo apt install gdbserver`
* Install gdb multiarch on host: `sudo apt install gdb-multiarch`
* In the VSCode project folder add a debug configuration file `.vscode/launch.json` with the following content: 
	* `"program"`: The executable built in the previous step
	* `"miDebuggerServerAddress"`: This is the IP address and port of the target where you start the gdbserver
```
{
	"configurations": [
	{
		"name": "C++ Remote Debug",
		"type": "cppdbg",
		"request": "launch",
		"program": "${workspaceFolder}/build/conv_single",
		"args": [],
		"stopAtEntry": true,
		"cwd": "${fileDirname}",
		"environment": [],
		"externalConsole": true,
		"MIMode": "gdb",
		"miDebuggerPath": "/usr/bin/gdb-multiarch",
		"miDebuggerServerAddress": "192.168.178.64:1234",
		"setupCommands": [
		{
			"description": "Enable pretty-printing for gdb",
			"text": "-enable-pretty-printing",
			"ignoreFailures": true
		}
		]
	}
	]
}
```
* Copy the executable to the target working directory via _sftp_.
* Open a _ssh_ connection to the target and start the gdbserver on the target, e.g. `gdbserver :1234 ./conv_single conv_hw.xclbin images/image_64x64.bmp`
	* Use the same port (`1234`) as used in the debug config. The gdbserver will now listen on this port and wait on the gdb client on the host computer.
	* Start the executable with the necessary arguments (e.g. `conv_hw.xclbin images/image_64x64.bmp`)
* In VS Code go to the debugger tab and start the debugger with the configuration with the name given above. Before starting you set breakpoints in the source code.
* Any outputs of the executable will be seen on the target ssh console.