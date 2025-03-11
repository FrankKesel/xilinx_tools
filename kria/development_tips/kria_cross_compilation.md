# Kria Cross-Compilation

## Overview
* In the Kria tutorials we developed the necessary C++ based applications directly on the Kria target. Although this has some advantages it can be slow for larger programs since the compilation directly on the Kria target is rather slow compared to the compilation on a laptop or PC. A so-called _cross compilation_ compiles the software on the host computer. In this document we describe the necessary steps to install a cross compilation toolchain and using VS Code for cross development (compiling, linking, running and debugging on the target).
* As in the Kria tutorials we will use again VS Code with CMake. The necessary files can be found in the folder `reference_files/convolution_xcompile` and `reference_files/helper_libs_xcompile`. Please download these folders to your host computer.
* We also assume that your host computer is running Linux, preferably Ubuntu Linux 22.04 (or a derivative), and that VS Code is installed.

---
## Toolchain Installation
* If you want to compile software for a target with a different instruction set than your host computer you need a so-called _cross compiler toolchain_. Your host computer will be most probably a laptop or desktop which executes an _Intel x86_ instruction set. The Kria target system is an _ARM 64 Bit_ instruction set based machine, so that it cannot execute _x86_ instructions. Therefore we need a cross compiler! 
* Install the cross compiler toolchain for ARM 64 bit systems (_aarch64_) on the Linux host: `sudo apt update && sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu`
* Check version: `aarch64-linux-gnu-gcc --version`
  * If you installed the toolchain under Ubuntu Linux 22.04 you should have the version 11.4.0.

---
## Download and Install Sysroot
* For cross compiling a so-called _sysroot_ is needed  which is a copy of the necessary directories and files from the _root file system_ of the target. The _sysroot_ is needed for compilation of the source code and linking of the program executable, since it will be executed on the Kria target system. 
* You can download a _sysroot_ for the Ubuntu Linux which runs on the Kria board from the following website: https://ubuntu.com/download/amd
	* Scroll down on this website to the section _Ubuntu Desktop 22.04 LTS_  and find the _Sysroot for cross-compilation_ file `iot-limerick-kria-classic-server-2204-classic-22.04-kd05-20240223-170-sysroot.tar.xz`. 
* Download the file and unpack it on your computer. You will later on need the path to the sub-directory (the sysroot path) `/iot-limerick-kria/aarch64-xilinx-linux` in the `CMakeLists.txt` files.

---
## Install Sysroot from Kria Ubuntu
* If you have no access to the official sysroot you can also extract the root file system from the Ubuntu OS on the Kria target, although we recommend to use the official sysroot (as described in the previous section).
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
## Cross Compile and Install the Helper Libs
* As described in the tutorial [kernel_based_design](../kernel_based_design/kernel_based_design.md) we use a small library for our C application projects. This library must also be cross compiled on the host computer.
* In the folder `reference_files/helper_libs_xcompile` you can find a bash script `make_and_install.sh`. Edit this script and adapt the installation path of the helper lib to your needs. You will need this path later on in the cross-compilation of the application.
* Edit also the `CMakeLists.txt` file and adapt the path to your `sysroots` directory. 
* Then execute the bash script which should compile and install the helper libs. In the path where you installed the library you should find two folders named `include` and `lib`. The first one contains the necessary include files and the latter one the compiled library, which will be linked to you application project.

---
## VS Code Cross Compile with CMake
* In the folder `reference_files/convolution_xcompile` you can find an example project for the cross compilation of the convolution example (it is the `conv_single` example). Copy the folder to a directory on your host computer and open the directory in VS Code, which will be the workspace.  
* In the `CMAkeLists.txt` file adapt the paths to the sysroot directory and the helper libs directory on your host computer (see steps described above).
* When the path to the _sysroot_ is set correctly the build system should find the libraries (e.g. OpenCV and XRT) in the sysroot directories. When you see compiler or linker errors then first check the path. Also check that helper lib has been also cross-compiled.
* You can then proceed as decribed in the tutorial [kernel_based_design](../kernel_based_design/kernel_based_design.md#sw-development-in-c-on-the-kria-target), with some differences concerning the cross-compilation described in the following.
* In the CMake tab when starting to compile you will need to configure the _kit_. Here you select the installed _cross compiler toolchain_, i.e. `GCC 11.4.0 aarch64-linux-gnu` (depending on the version you installed). Then build the executable as usual. Be careful and **do not** select the _x86_ toolchain, the generated code will not execute on your target!
* After building you must transfer the binary executable (e.g. via _sftp_) to the Kria target system and execute it there. You need a terminal program on your host computer and connect to the target via _ssh_. Do not forget the arguments for your application. For the convolution example this is the FPGA binary and the image file.
* If you want to debug the code you can do a remote debug with VS Code as described in the following.

---
## Remote debugging with gdb
* If you want to debug your code, you must do a _remote_ debug on the target system. This can also be done in VS Code with the settings given in the following. We will use a _gdbserver_ on the target which will run the executable and we will connect to the server from the _gdb client_ in VS Code.
* Install gdbserver on **target**: `sudo apt install gdbserver`
* Install gdb multiarch on **host**: `sudo apt install gdb-multiarch`
* In the reference folder `reference_files/convolution_xcompile` there is a `launch.json` file in the sub-folder `.vscode` which defines the debug setting in VS Code. You may have to check and adapt some entries in this file:
  * `"program": "${workspaceFolder}/build/conv_single",`
    * This entry defines where the executable is located (it should be correct if you followed the steps above):   
  * `"miDebuggerServerAddress": "192.168.178.64:1234",`
    * This entry defines the IP address of your target system and you must adapt the IP address. `1234` is the port where the _gdbserver_ is listening on the target. You should not change this.
* In the bottom of `launch.json` there is an entry `"preLaunchTask": "start_remote_debug"` which defines that a _task_ `start_remote_debug` is run before the debugger starts. This task is defined in the file `task.json` (also in the folder `.vscode`) and uses a second task `upload_executable` (also defined in `task.json`). Both tasks together load the excutable via _scp_ to the target and start the _gdbserver_ listening on port 1234, such that the debugger on your host computer can connect to it. 
* You should not change anything in the code of the tasks in `task.json`. Adaptions to your project are done via the file `settings.json` (also in `.vscode`), which defines some _variables_. Here you can or must change the following:
  * `"remote_dir"`: The remote project folder
  * `"executable_name"`: The name of the executable (defined in `CMakeLists.txt`)
  * `"target_url"`: IP address of the target, including the user (`ubuntu`)
  * `"target_pw"`: Password of user `ubuntu`
  * `"hw_binary"`: Name of FPGA PL binary (1st argument of executable)
  * `"image_file"`: Path to the image file (2nd argument of exeutable)
  * `"target_command"`: Command for the executable of the application, uses some of the variables above. If your application has less or more than 2 arguments you must change it here. 
* You can then start the debugger in VS Code, not in the _CMake_ view but by selecting the _Run and Debug_ view. Here you should find the _C++ Remote Debug_ entry, which was defined in `launch.json`. When you start the debugger it will take some time, since the executable must be uploaded and the gdbserver must be started on the target. 
* You can then debug as usual by setting breakpoints etc. Be aware that you are connected to your target via a GDB client-server connection, where the client is on your host computer and the server is running on the Kria target. You will see the (remote) output of your application in the _terminal_ of VS Code on your host. When you stop debugging the gdbserver will be stopped on the target. The next time you start debugging the executable will be uploaded and the gdbserver is started again. Happy debugging!