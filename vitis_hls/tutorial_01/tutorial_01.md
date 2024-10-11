# HLS Tutorial 1: Project Setup and Basic Steps

---
## Overview
* In this tutorial the setup of a Vitis HLS component project is described. Furthermore the basic steps _C Simulation_, _C Synthesis_ and _C/RTL Cosimulation_ are described. As an example we take a simple FIR filter, the source code for the component and the testbench can be found in the folder `reference_files/src`.
* The project setup will be done in the Vitis GUI, this is described in [Creating a HLS component](#creating-a-hls-component). 
* You can setup a project also on the command line, this is described in [tutorial 1b](tutorial_01_b.md). You can then open the Vitis GUI and open the project workspace and continue working with the project. 
* It is essential that you understand the concept of workspaces and components before you start with a component project. This is described in [Workspaces and components](#workspaces-and-components).

---
## Workspaces and components
* A workspace can contain more than one component. If you want to generate several variants of an IP core, in order to compare them, you must generate for each variant a component (this was called _solutions_ in the old Vitis version).
* In each component folder there must be a configuration file which contains commands and directives for HLS and a work directory, which holds the various outputs of HLS.
* All components can refer to the same source files (in a folder `src`), so a directory structure for a workspace holding several variants of the same IP core should look like the following:
```
workspace_dir
			|-src
			|-component1
				|-component1_work
				component1_config.cfg
			|-component2
				|-component2_work
				component2_config.cfg
			|-...
```
* The directory structure for this tutorial follows this structure: In the folder `reference_files` you can find two solutions/components for the same source code. We will use them in the section [Creating components from the command line](#creating-components-from-the-command-line).

---
## Starting Vitis
* You can launch Vitis in a terminal on the command line by using the following command (replace `<Vitis_Installation_Directory>/` with the path to your Vitis installation which should normally be `/opt/xilinx/Vitis/2024.1/`), where `<workspace>` indicates the workspace mentioned in [Workspaces and components](#workspaces-and-components):

```
source <Vitis_Installation_Directory>/settings64.sh
vitis  -w <workspace> 
```

* Or you can start the Vitis GUI without a workspace with the command `vitis` on the command line and select an existing workspace in the Vitis GUI.
* For the tutorial make a folder with the file manager (for example in your home directory) to be used as a workspace and start the Vitis GUI as described above. You should see the Vitis GUI as shown in the following image (There are already some recent workspaces in the image which you will not see when you open your Vitis IDE).

![Vitis GUI](images/hls_01.png)

---
## Creating a HLS component

* With the Vitis IDE opened, from the main menu select `File > New Component > HLS` 
	* You can also select `Create HLS Component` on the navigation pane on the left side or on the Welcome page in the _HLS Development_ section.
* This opens the _Choose Name and Location_ page of the _Create HLS Component_ wizard, as shown in the image below. Enter the component name (`fir`) and location (i.e. the workspace folder) and then push `Next`. The workspace should be the folder which you created as workspace.

![Create HLS component](images/hls_02.png)


* This opens the _Configuration File_ page of the wizard as shown below. The HLS component configuration file contains commands and directives used for the synthesis, simulation, and export of the component and is _the_ most important file (besides the source code files).

![Configuration page](images/hls_03.png)


* The _Configuration File_ page of the _Create HLS Component_ wizard provides three ways to add a configuration file to your component:
	* `Empty File`: Specifies the creation of a new empty configuration file to be populated after component creation . Selecting this option lets you specify a name in the `New configuration file name` field.
	* `Existing File`: Specifies the use of an existing configuration file. Selecting this option lets you enter the path and name in the File field, or select _Browse_ to locate the file.
	* `Generate from Vitis HLS Project`: This option generates a new config file from an existing Vitis HLS project _hls.app_ file. 
	* Choose `Empty File` as the method of adding the config file to your HLS component, you can keep the name `hls_config` as file name. Select `Next` to proceed to the Source Files.
* The _Source Files_ page of the wizard opens:
	* Under _Design Files_ you can select the `Add files` symbol (marked in red in the image below) to select the primary source file for the design, or add multiple files as needed (not for this tutorial). Copy the `src` folder from the folder `reference_files` to your workspace (with the Linux file manager) and then select the source file `fir.c` with the  `Add files` symbol. There is no need to include the header file `fir.h`, Vitis will find it itself.
	* After adding the source file, you must identify the _Top Function_ which is the function to be synthesized as the top-level function for the HLS component. Push `Browse` and select the function `fir`.
	* Under _Test Bench Files_ you can select with `Add files` (marked in red) the testbench source code `fir_test.c` and a data file `out.gold.dat` which is needed by the testbench.
 	* For both design files and test bench files you can add CFLAGS and CSIMFLAGS to specify directives to be used during compilation and simulation. You can specify directives to be applied to all files or to specific files from each category. Adding these flags can also be done later on, for this tutorial leave the fields empty. After adding files and folders and directives, select `Next` to proceed.

![Source page](images/hls_04.png)


* The _Select Part_ page of the wizard is opened. It is mandatory to select the correct FPGA part for which the component should be used later on. For this tutorial we use the Artix-7 part as shown in the next image. Proceed with `Next`. 

![Select part page](images/hls_05.png)


* You will now see the _Settings_ page of the wizard. 
	* The _Settings_ page lets you specify a clock period or frequency for the design, and a clock uncertainty. The default clock for the tool is a clock period of 10 ns (or 100 MHz), and a clock uncertainty of 27%. In this case enter the default clock frequency of 10 ns but change the uncertainty to 2 ns as shown in the image below.
	* The _Settings_ page also lets you specify the _flow_target_ for the HLS component build process as being either to generate a Vivado IP or a Vitis kernel as described in [Target Flow Overview](https://docs.amd.com/r/4lwvWeCi9jb~DWzdfWuVQQ/P4DbGaxyPEWYEJfsO7o1Dw) in the Xilinx User Guide. The interface requirements of the Vivado IP or a Vitis kernel are different as explained in [Interfaces of the HLS Design](https://docs.amd.com/r/4lwvWeCi9jb~DWzdfWuVQQ/wEdlxulOAT50bjDEn06U5Q). The default _flow_target_ is the Vivado IP flow. Keep these default settings for this tutorial.
	* Below the _flow_target_ you can also specify the _package.output.format_ for the tool to generate when packaging the design. The default output format is the Vivado IP, which generates a zip-file to be added to your IP catalog, letting you use the HLS generated RTL design in other designs.
	* The data you entered here and above are all stored in the configuration file.
	* Select `Next` to proceed to the Summary page.


![Settings page](images/hls_06.png)

* The _Summary_ page reflects the choices you have made on the prior pages. Review the summary and select `Finish` to create the HLS component, or select Back to return to earlier pages and change your selections.
* When the HLS component is created you will get back to the Vitis IDE with the opened component workspace. Now it is time to explain the Vitis IDE in more detail. In the image below you can see five areas or panes in the IDE, if you are familiar with Visual Studio Code you may notice some similarities. The pane numbered with 1 is the _tool bar_ which allows you to switch between major features of the tool: the _Vitis Component Explorer_ (this is where you are at the moment), the _Search_ function, _Source Control_, the _Debug_ view, _Examples_ and the _Analysis_ view. Pane 2 is the _Vitis Component Explorer_ which can be used to view a virtual hierarchy of the workspace. The view displays the different elements of the component or project, such as sources and the outputs folder. It does not show the real folder structure on the file system. In the _Flow Navigator_ in pane 3  you can select the actual component (you have only one component `fir`) and the tools you can apply to it (C Simulation, C Synthesis C/RTL Cosimulation,  Package and Implementation). Pane 4 is the central editor window where you can edit the sources and view reports etc. Pane 5 is the console and terminal area, which may not be opened in your Vitis GUI since you have not started a tool yet.  

![Vitis GUI with component](images/hls_07.png)

* Please review now also the folder structure in your workspace (with the Linux file manager). You have one component named `fir`, which uses the source codes in the `src` folder. In the component folder you can see the configuration file `hls_config.cfg` and the JSON file `vitis-comp.json`, plus another JSON file. You can see the sources and settings files also in the  _Vitis Component Explorer_ as shown in the image below.

```
workspace_dir
			|-src
				fir.c
				fir.h
				fir_test.c
				out.gold.dat
			|-fir
				compile_commands.json
				hls_config.cfg
				vitis-comp.json
```

![Source data](images/hls_08.png)

* In the next sections we will run C simulation, C synthesis and C/RTL cosimulation. When you close the Vitis GUI and open it again you will see your existing workspaces on the _Welcome_ page. Select the workspace name with the mouse and it will open up again. It may be a good idea to save the complete workspace folder (e.g. by zipping the folder) in case you destroy anything. The most important files are the source files in the folder `src` and the configuration file (`hls_config.cfg`).

---
## C Simulation
* The first step in a HLS project is to verify the correct function of the C code of the component. In order to test the component a _test bench_ is needed, which generates input data for the component and compares the output data with reference data. This is the same approach as in VHDL/Verilog design, the difference is that everything is coded in C/C++. So what we basically do here is to compile the code for the testbench and component and run the compiled code as application. 
* First open the C sources in Vitis with the _Component Explorer_ and study the code for the component (`fir.c`) and the testbench (`fir_test.c`). In the component code you can see a loop _Shift_Accum_Loop_ which codes the functionality of an FIR filter with a shift register `shift_reg[N]` where the coefficients (argument `c[N]`) are multiplied with the shifted data and accumulated in the variable `acc` and returned via the pointer `y`. It is good practice to label loops in order to be able to identify them during analysis of the HLS synthesis results.  
* The testbench (`fir_test.c`) defines the coefficients for the FIR filter in the variable `taps[N]`. In line 18 the file `out.gold.dat` is opened which contains the input data and the expected output data for comparison. In the loop the input and reference data is read and the component top level function is called in line 26. Finally the output data generated by the component is compared and an error flag is set, if there are discrepancies. An HLS testbench, i.e. the main function, needs to return a value, which is 0 in case of success and 1 if the output data of the component is not correct. This is important because the C/RTL Co-Simulation uses the same testbench and also checks the output of the generated VHDL or Verilog code against the reference data. This means that an HLS testbench always must be written in such a way that it is self-checking and generates a final pass (0) or fail (1) value as return value.
* Run the C Simulation by pushing the `Run` button in the _Flow Navigator_ under `C Simulation`. A window will pop up and ask you if you want to enable the _Code Analyzer_, you can disable this feature for this tutorial. After simulation is completed you will see the output of the simulation below the editor window in the console area.  
* If you write yourself code you may want to use a debugger by pushing the `Debug` button instead of `Run`. Vitis now starts the debugger as shown in the image below. Here you have the standard debugger functions like _Continue_, _Step Over_, _Step Into_ etc. (marked in red). You can also watch the variables (marked in red) or set break points in the source code editor. It is very similar in functionality to the debugger in Visual Studio Code. For more information on the debugger you can consult the [Xilinx User Guide](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Debugging-the-HLS-Component).
* If you want to stop the debugger then push the red square button (`Stop`). In order to come back to the normal Vitis view, which you had before you started debugging, you have to push (most left pane) the _Vitis Components_ symbol (marked in green) in the tool bar .

![Debug code](images/hls_09.png)


---
## C Synthesis
* The most important step is the _C Synthesis_, which is in fact the High Level Synthesis (HLS) of the component coded in C/C++. To start the HLS go to the _Flow Navigator_ and push the `Run` button under `C Synthesis`. It will take some seconds depending on the compute power of your machine, when it is finished you will see _Synthesis finished successfully_ in the output console.
* Go to the _Flow Navigator_, expand _REPORTS_ in the _C Synthesis_ section and push `Synthesis`. You should now see the synthesis report as shown in the next image. The most interesting informations are the _Timing Estimate_ and the _Performance & Resource Estimate_. As you can see in the report our constraint was a clock cycle time of _10 ns_ and Vitits estimates the critical path of the component to _6.86 ns_, so we have a slack of _3.14 ns_ which is comfortable. But this is only a rough estimate since the component has not yet be implemented. 
* In the _Performance & Resource Estimate_ you can see the latency and interval values (16 clock cycles and 11 clock cycles). The latency is the time for one operation of the component, which means one operation lasts _16 x 10 ns = 160 ns_. The _Interval_ (shorthand for _Initiation Interval_) is the number of clock cycles before new inputs can be applied. Since we have 11 cycles for the _Interval_ this means that we could apply new input values before the operation of the component has finished (which takes 16 cycles). 
* The performance is mainly determined by the loop, which iterates 11 times (_TRIP COUNT_). Each loop iteration needs 5 clock cycles (_ITERATION LATENCY_). The loop is operated in a pipelined manner, which you can also see in the report in the column _PIPELINED_. This means that loop iterations can start earlier, otherwise we would need _5 x 11 cycles = 55 cycles_ for all iterations. Loop pipelining is always applied automatically, since it is very effective. We will come back on pipelining in another tutorial.  

![Synthesis report](images/hls_10.png)

* There are some more reports, for example the _Function Call Graph_, which shows you the function hierarchy (not really important for this tutorial). Another important report for analysis of the generated component hardware is the _Schedule Viewer_. If you open it you should see the scheduling of the hardware operation as shown in the next image. 
* In the first column you will see some cryptic names, these refer to identifiers in the generated VHDL or Verilog code and represent operations in the hardware. In the upper row (marked in red) you will see numbers beginning from 0. These are the so-called _control states_ of the finite state machine (FSM), which controls the operation of the component. One control state equals one clock cycle. You can easily identify the loop _Shift_Accum_Loop_ which takes 5 cycles for one iteration (this corresponds to the value in the synthesis report). With the _Focus_ field (marked in red) you can set the focus to the loop, such that you can only inspect the loop. On the right side you can activate a legend (i-symbol marked in red).
* Although this view may appear cryptic at a first glance, this is the main tool if you want to understand how the component operates and what could possibly be optimized. From experience the loops and memory accesses determine mainly the performance in terms of clock cycles and with some practice the schedule viewer can be helpful in analyzing the bottlenecks of your design. If you push one of the operations with the left mouse button you will see some green lines which show you the dependencies of the operations, which can be also helpful.

![Schedule viewer 1](images/hls_11.png)

* If you push the right mouse button over one of the operations in the left column you get a context menu `Goto Source` which jumps to your C source code and shows you the relation to your C code. If you select for example the identifier `c_load` and go to the source you can see in line 19 that this is the  access to the argument `c[i]`, which is in fact a memory access in hardware (it is assumed that a Block RAM is attached to the component which stores the coefficients). This is a so-called _multicycle resource_, since the corresponding bar spans over a clock cycle (and the bar has a center line). In case of the Block RAM access this means that the address and control signals are applied before the rising clock edge and the Block RAM will deliver the coefficient value after the clock edge.

![Schedule viewer 2](images/hls_12.png)

* There are also more informations in the reports. For example when you scroll down the synthesis report you will see informations on the hardware interfaces and SW I/O (where you can see for example the memory port for argument `c[N]`) and finally operator and storage information. We will come back on these issues in a later tutorial.

---
## C/RTL Cosimulation
* You can inspect the generated VHDL or Verilog code under _Output_ in the _Vitis Components Explorer_. In the image below you can see that the toplevel VHDL code `fir.vhd` has been opened. Since it is not sensible or possible to inspect the whole generated VHDL/Verilog code, the only efficient way to verify it is by simulation. The _C/RTL Cosimulation_ will simulate the generated VHDL or Verilog code by executing the C testbench and simulating the VHDL or Verilog code with the Xilinx simulation tool. Since we use the same self-checking testbench, that we have used for C simulation, we can be sure that the generated hardware component produces the same output data than our C code, if we have a _Pass_. Therefore _C/RTL Cosimulation_ is an important step and although you could proceed without cosimulation you should not do that. If cosimulation fails there can be several reasons for it, but one reason could be in fact that the hardware works not correctly.

![VHDL code](images/hls_13.png)

* Push the `Run` button in the section _C/RTL COSIMULATION_ in the _Flow Navigator_. When co-simulation has finished you should see the message marked in red in the next image in the console. In the console window there is a navigation area (marked in red) where you can switch between the console outputs of the tools.
* Under _REPORTS_ select _ Cosimulation_ and you will see the _Performance & Resource Estimates_. If you compare this with the values of the synthesis report you may notice that the initiation interval is 15 cycles and not 11 cycles. If you scroll up in the console output there is a warning related to this problem:

```
WARNING: [HLS 200-626] This design is unable to schedule all read ports in the first II cycle. The RTL testbench may treat the design as non-pipelined
 ```
* This is a pipelining issue, which means that new data can only be applied after 15 clock cycles. We will come back on pipelining issues in a later tutorial. 

![Cosimulation](images/hls_14.png)

* When you push on _Timeline Trace_ under the reports the timeline trace report is opened as shown in the next image. In our testbench `fir_test.c` we call the `fir` function 26 times and you see there are 26 executions of the hardware component in this view. Each execution needs 15 cycles, and there is always an overlap of 1 cycle, where new data is applied. So the total execution time is 390 cycles for all 26 executions of the component, which gives an average execution time of 15 clock cycles.

![Timeline](images/hls_15.png)

* You can see that there is also an additional report called `Wave Viewer`. This is basically starting the Vivado wave viewer where you could watch the waveform traces of the VHDL/Verilog simulation. It is greyed out by default and would need some special settings in the configuration file. We will skip this here.

---
## Package and Implementation
* The last important step is to export the component as _IP Core_ such that it can be used in Vivado. Go to the _PACKAGE_ section in the _Flow Navigator_ and push `Run` (see image below). After a while you will see the message in the console marked in red which says that a zip-file `fir.zip` was generated in the folder `fir`, where the path is relative to the component folder such that you can find it in `<workspace>/fir/fir/fir.zip`. 
* In addition the files of the IP Core are also generated in the Output section in the `impl` folder in the _Vitis Component Navigator_ as you can see in the image below (the true file path to this folder is `<workspace>/fir/fir/hls/impl`).

![Package](images/hls_16.png)

* In order to use the IP Core in Vivado you have to include the folder `<workspace>/fir/fir` as a _repository_ in Vivado. Then you can use the IP Core in an _IP Integrator_ block diagram as shown in the image below. There are some options for the export which can all be specified in the configuration file, for more details refer to the [Vitis HLS documentation](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Packaging-the-RTL-Design).

![Vivado](images/hls_17.png)

* The very last step in the _Flow Navigator_ is _IMPLEMENTATION_, which is optional. If you push `Run` then logic synthesis and implementation (place&route) is run by running the corresponding Vivado tools in the background. Although it is not necessary to run the implementation (and it can take some time) it gives you the real numbers on resource usage and the critical path of the component. In the following image you see the results (report _Place and Route_) of the implementation in terms of resource usage and timing. You can see that the critical path (_CP_) is 5.692 ns and that is below that what HLS estimated (6.86 ns), so the estimate was too pessimistic. Finally things can change again with respect to the critical path when the component is embedded into a larger design in Vivado, but the target clock cycle of 10 ns should be safely achievable.

![Vivado](images/hls_18.png)


