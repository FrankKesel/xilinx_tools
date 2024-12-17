# Kria KV260 Starter Kit Tutorials Overview

* Here you can find tutorials for the _Kria KV260 Vision AI Starter Kit_. For each tutorial you will find a documentation and the source codes and scripts (for each tutorial in a folder `reference_files`). The tutorials assume that the _Starter Kit_ runs Ubuntu Linux and is accessible via LAN in your local network from your development computer.
* For the tutorials you will need a so-called _Extensible Platform_. This platform is used in Vitis to link the IP kernels to a custom-built FPGA design for the _Zynq UltraScale+ MPSoC_. The extensible platform provides everything what is needed in order to integrate and use the IP kernels designed in the tutorials. We provide our own custom-built platform which you can download as a zip-file [here](../kria/resources/extensible_platform/). 
* You should start with the introductory tutorial [Design of a simple kernel based design for the KV260 Starter Kit](kernel_based_design/kernel_based_design.md), where the basic steps of the IP kernel design, the integration in the extensible platform and the use of the kernel on the Kria KV260 system under Linux is shown with a simple example IP kernel. 
* In a second step you can complete the tutorial [Design of a OpenCV kernel based design for the KV260 Starter Kit](opencv_kernel/pencv_design.md), where an OpenCV kernel will be developed with Vitis HLS and implemented on the Kria KV260 system. With this kernel it will be possible to process images and also to stream video frames from the USB cam on the Kria board.
* The Vitis tool version used is 2024.1. We assume that you have an installation on a Linux computer running Ubuntu 22.04.

---
## Vitis Tutorials
* [Design of a simple kernel based design for the KV260 Starter Kit](kernel_based_design/kernel_based_design.md)
* [Design of a OpenCV kernel based design for the KV260 Starter Kit](opencv_kernel/opencv_design.md)

---
## Development Tips
* [Cross-Compiling C++ Software for Kria](development_tips/kria_cross_compilation.md)

---
## Additional Documentation
* [Kria KV260 Vision AI Starter Kit](https://www.amd.com/en/products/system-on-modules/kria/k26/kv260-vision-starter-kit.html)

