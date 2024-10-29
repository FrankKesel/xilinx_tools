# Vitis Tutorials Overview

* Here you can find tutorials for the Xilinx Vitis tool. For each tutorial you will find a documentation and the source codes and scripts (for each tutorial in a folder `reference_files`). The tutorials are targeted to the _Kria KV260 Vision AI Starter Kit_. The tutorials assume that the _Starter Kit_ runs Ubuntu Linux and is accessible via LAN in your local network from your development computer.
* For the tutorials you will need a so-called _Extensible Platform_. This platform is used in Vitis to link the IP kernels to a custom-built FPGA design for the _Zynq UltraScale+ MPSoC_. The extensible platform provides everything what is needed in order to integrate and use the IP kernels designed in the tutorials. We provide our own custom-built platform which you can download as a zip-file [here](../vitis/resources/extensible_platform/). 
* You should start with the introductory tutorial [Design of a simple kernel based design for the KV260 Starter Kit](kernel_based_design/vitis_kernel_based_design.md), where the basic steps of the IP kernel design, the integration in the extensible platform and the use of the kernel on the Kria KV260 system under Linux is shown with a simple example IP kernel. 
* The Vitis tool version used is 2024.1. We assume that you have an installation on a Linux computer running Ubuntu 22.04.

---
## Vitis Tutorials
* [Design of a simple kernel based design for the KV260 Starter Kit](kernel_based_design/vitis_kernel_based_design.md)
* [Design of a OpenCV kernel based design for the KV260 Starter Kit](opencv_kernel/vitis_opencv_design.md)

---
## Additional Documentation
* [Kria KV260 Vision AI Starter Kit](https://www.amd.com/en/products/system-on-modules/kria/k26/kv260-vision-starter-kit.html)

