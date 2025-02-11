# Vitis Tips


- [Vitis Tips](#vitis-tips)
  - [Preface](#preface)
  - [Getting information on the used platform](#getting-information-on-the-used-platform)
  - [Linking the kernels with user defined configurations](#linking-the-kernels-with-user-defined-configurations)
    - [Entering user defined configurations](#entering-user-defined-configurations)
    - [Defining the kernels](#defining-the-kernels)
    - [Assigning IP ports to PS interfaces](#assigning-ip-ports-to-ps-interfaces)
    - [Specify the clock for the kernel](#specify-the-clock-for-the-kernel)


---
## Preface
* In this document you can find additional tips and infos on the Vitis kernel based design flow.

---
## Getting information on the used platform
* In some cases (see below) it is necessary to get information on the platform, e.g. for memory interfaces or clocks.
* `platforminfo <platform>.xpfm`
  * located in `<path>/Vitis/<version>/bin`
  * Shows info on memory interfaces, clocks etc.

---
## Linking the kernels with user defined configurations
* see https://docs.amd.com/r/en-US/ug1702-vitis-accelerated-reference/v-Linking-and-Packaging-Options
* The PL kernel compilation process results in a Xilinx object (.xo) file whether the kernel is written in C/C++ or RTL. During the linking stage (`v++ --link ...`), one or more PL kernels are linked with the platform to create the FPGA binary container file (.xclbin). If you do not specify a configuration, then the Vitis v++ linker will for example automatically select the memory interfaces.  Changes to the default HW configuration (e.g. memory interfaces, clocks) can be defined by the user, which is described below.

### Entering user defined configurations
* The user defined configurations can be entered in a configuration file which is used when `v++` is executed (e.g. with switch `--config link.cfg`). There are dedicated sections (e.g. `[connectivity]`) below which the configurations are entered. 
* You can find a complete configuration file in the folder `reference_files`.

### Defining the kernels
* The number of kernels and their instances names can be defined. For example here we specify one instance of the kernel `conv_top`:
```
[connectivity]
nk=conv_top:1:conv_top_1
```

### Assigning IP ports to PS interfaces
* The Kria SOM offers the following memory interfaces: HP0 - HP3, HPC0 - HPC1, where the HPC interfaces are cache coherent. Check which interfaces are available in your extensible platform, not all of them may be available for the kernels. 
* The ports of the IP core can be assigned to memory interfaces by entering the following in the above mentioned configuration:
```
[connectivity]
sp=<ip_name>.<argument_name>:<memory_interface>
```
* The `<ip_name>` is the instance name of the IP core (you can find it in the link summary), the `<argument_name>` is the name of the argument in the C++ code of the IP core. The `<memory_interface>` is the above mentioned memory interface. An example entry looks like this (instance `conv_top_1`, port `in_image` and memory interface `HPC0`):
```
[connectivity]
sp=conv_top_1.in_image:HPC0
```
* If names do not fit then check the link summary after a default run, since they may differ from the names in the C++ code. Or you can check the HLS synthesis results. You can also use the interface names and prescede them with `m_axi_`, for example:
```
[connectivity]
sp=conv_top_1.m_axi_image_r:HPC0
sp=conv_top_1.m_axi_filter:HP0
```

### Specify the clock for the kernel
* If you do not specify the clock frequency Vitis will take the default clock source for the kernel as specified in the extensible platform. Below you can see part of the output of `platforminfo` for the extensible platform. You can see that the clock index 1 is the default and it has a clock frequency of 200 MHz. 

```
=================
Clock Information
=================
  Default Clock Index: 1
  Clock Index:         0
    Frequency:         99.999000
    Status:            fixed
  Clock Index:         1
    Frequency:         199.998000
    Status:            fixed
  Clock Index:         2
    Frequency:         299.997000
    Status:            fixed

```
* You can change the clock frequency for the kernel with the following command in the configuration file. The frequency must be given in Hz (250 MHz in the example) and the kernel instance is specified.

```
[clock]
freqHz=250000000:conv_top_1
```
* If you select a clock frequency that is different to the available clocks then Vitis will try to generate hardware which derives the requested frequency from the avalaible clocks. 
* Be aware that your specified clock may be too high and Vitis will generate error messages concerning timing violations. In this case the PL binary will not be generated. 