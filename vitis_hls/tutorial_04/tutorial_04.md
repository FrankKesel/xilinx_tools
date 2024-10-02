# HLS Tutorial 3: Interface Synthesis

---
## Overview
* In this tutorial we would elaborate a bit more on the topic of _Interface Synthesis_. In the previous tutorials we have already covered some interface types like for example _AP_MEMORY_, which generates the ports for Block RAMs in the component. First we have to distinguish between the interface of the C/C++ software function, which are the arguments of the function, and the interface of the generated hardware component, which are basically the ports of the component. In the case of the hardware component we can also talk of an interface, for example when we have a set of ports for attaching a Block RAM memory or a set of ports for a bus interface.
* Depending on the type of SW argument there is a default implementation of the HW interface (i.e. the ports) in Vitis HLS. This whole topic is called _Interface Synthesis_ and you can find more information in the [Vitis HLS User Guide](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Introduction-to-Interface-Synthesis). By using directives you can change the default implementation of the interfaces and we will concentrate on this topic in thi tutorial.

---
## Setting up the component and baseline synthesis
* Like in the previous tutorials download the folder `tutorial_04/reference_files` and rename this folder to `tutorial_04`. 
* Open a Linux terminal, go to the folder `tutorial_04` and execute the bash script `run_demo.sh`. Then start the Vitis GUI and open the workspace `tutorial_04`.