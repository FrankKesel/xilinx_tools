# Vivado Tutorial

---
## Overview

* This tutorial is a self-study course to introduce the Xilinx Vivado development environment. Using a simple VHDL example, the tutorial describes the necessary steps in Vivado to go from entering the VHDL source code to generating a bit file for programming an FPGA. The tutorial refers to _Vivado_ version 2024.1, with which the individual steps were tested. When you have installed the _Xilinx Vitis Core Development Kit_ then Vivado is included. You can also install Vivado as a standalone tool from the [Xilinx homepage](http://www.xilinx.com/support/download.html). Select `Vivado` and `2024.1` and then `Xilinx Unified Installer 2024.1: Windows Self Extracting Web Installer` (or the Linux Web Installer if necessary). You must request a license at the end of the installation. To do this, you must register with Xilinx.
* If you would like to test the resulting design on FPGA hardware, then the [Basys 3 FPGA board](https://digilent.com/reference/programmable-logic/basys-3/start) from Digilent would be recommended. The pin assginment in this tutorial is dedicated to this board. But you do not necessarily need the board to work through this tutorial using the Vivado software. 
* For further and more in-depth informations on the Xilinx Vivado design flow, the Xilinx documentation is recommended for example the [UltraFast Design Methodology Guide for FPGAs and SoCs ](https://docs.amd.com/r/en-US/ug949-vivado-design-methodology).
* The source files for this tutorial can be found in the folder `reference_files`. Download the complete folder `vivado_intro/reference_files` to your computer. 
  * You can either download this whole Github repository or only the folder `reference_files` using a browser extension like [GitZip](https://gitzip.org).

---
## Creating a project

* In this section, we will create a Vivado project for the corresponding target FPGA device.

* Create a work directory for the Vivado projects on your development computer, e.g. in your user directory or somewhere else where you as a user have write and read permissions.

* Start Vivado 2024.1 from the Windows (or Linux) Start menu (usually found under _Xilinx Design Tools_).

* Under `Quick Start` create a new project by pushing `Create Project`. A _Wizard_ opens as shown in the next image. Enter the relevant entries in the dialogs as described below and go to the next dialog by clicking `Next`.

![Vivado GUI](images/viv_001.png)

* Give your project a name (e.g. _intro1_). Select the previously created work directory for the Vivado project files as the `Project location` and activate the `Create project subdirectory` option. Important: Do not use spaces, special characters or umlauts in path or file names! The subdirectory will be named with the project name. Push `Next`.
* Select `RTL Project` as the project type and select `Do not specify sources at this time`. We will add the VHDL sources later. Push `Next`.
* In the next dialog the FPGA target component must be specified. The quickest way to select the FPGA component that is installed on the BASYS3 board is to use the following settings: `Family: Artix-7`, `Package: cpg236`, `Speed: -1`. Select the component `xc7a35tcpg236-1` from the list and complete the project creation by clicking `Next` and then `Finish` in the following dialog. You will be returned to the Vivado GUI as shown in the following image.

![Vivado GUI](images/viv_002.png)

* The correct specification of the FPGA component is particularly important for the implementation and the correct assignment of the pins. If the wrong component is selected, the FPGA will not be able to be configured correctly later. The creation of the Vivado project is now complete.
  * _Note_: The exact specification of the FPGA part is only necessary if you plan to download the bitfile at the end of this tutorial to the _Basys 3_ board. If you do not have the board, you can select any other part from the _Artix-7_ family. 

---
## Adding the VHDL source files

* In this section we will add the VHDL source file for our design and a corresponding VHDL testbench for the simulation. The design will be a 4-bit counter that increments with each rising edge at the input port. This input can be connected to a button on the _Basys 3_ board. The four counter outputs can be connected to the board's LEDs. The connections to the board pins will be specified later in this tutorial. The source code for the counter and the testbench can be found in the folder `reference_files`.
* In the _Flow Navigator_ (left column in the Vivado window), under _Project Manager_, click `Add Sources`.
* In the _Wizard_ that opens, select `Add or Create Design Sources`. Click `Next` to go to the next dialog and select `Add Files`. Navigate to the place where you stored the folder `reference_files` and add the file `counter.vhd`. Select `Copy sources into project` as shown in the next image. This will copy the file into the Vivado project. Click `Finish`.

![Add sources](images/viv_003.png)

* Repeat this for the testbench file: Click `Add Sources` again, but select now `Add or Create Simulation Sources`. This is important so that Vivado knows that the testbench is only there for simulation and should not be implemented in hardware. Add the testbench file `counter_tb.vhd` as shown in the next image and click `Finish`.

![Add sources](images/viv_004.png)

* Now check that you have all neccessary sources in the _Sources_ pane, which is in the _Project Manager_ window in the top left corner as shown in the next image. You can see two entries: _Design Sources_ and _Simulation Sources_ (push the arrow symbol to extend). The `counter.vhd` source file is listed under both entries, but the `counter_tb.vhd` source is listed only under _Simulation Sources_ since it is only needed for simulation. When you later on setup up a new project you should always check that you have all necessary source files and that the testbench is only visible under _Simulation Sources_. The sources belonging to the design itself should appear under _Design Sources_ **and** _Simulation Sources_ .

![Add sources](images/viv_005.png)

---
## Simulation

* In this section we will simulate the design using the testbench.
* To start the simulator, click on `Run Simulation > Run Behavioral Simulation` in the _Flow Navigator_ under _Simulation_. 

    Open this file with a text editor and try to understand where your error is. Correct the errors in the source code and start the simulator again. If the simulation started correctly, the simulator window should be open in Vivado, as shown in the figure.

    Under _Scope_ you can see the hierarchical structure of your design. The test bench _counter_tb_ is at the top level, and below that is the counter _counter_ (instance name _dut_, see Instancing in the Test Bench). The ports, signals and constants defined for the module currently selected under _Scope_ are listed in the _Objects_ window; these can in turn be transferred to the display of the signal curves on the right edge of the window using _drag & drop_. By default, the signal curves of all objects at the top hierarchy level are displayed, in this case the test bench.

    To the right of _Scope_ and _Objects_, the signal curves of the selected signals are displayed in a _Wave Window_. By default, 1000 ns are simulated, but this default setting can be changed in the _Flow Navigator_ under _Settings_$\rightarrow$_'Simulation_' before the next start of the simulator. In the _Wavedialog Display_ you will find a bar at the top with buttons for zooming and other functions. To see the complete signal curve, for example, use the _Zoom Fit_ button.

    If you want to see signals from the _counter_, then select the instance name _dut_ under _Scope_ and then you can select the corresponding signals under _Objects_ using the right mouse button: In the context menu that appears, you can then add the signal using _Add To Wave Window_, as shown in the figure. However, you will not yet see any signal curves for the new signals. To do this, you must restart the simulator: The Vivado toolbar was expanded above the simulator window when the simulator was started. There you will find several blue buttons; if you move the mouse over them, the function is shown.

    You can use the _Restart_ button to reset the simulator to time 0. You can now enter a new simulation time in the toolbar (e.g. 100 ns) and simulate the corresponding time using the _Run for ..._ button. You can also enter these commands directly in the Tcl console below (the commands you have entered so far are also displayed there), this is usually more practical. The commands are: _restart_ and _run 100 ns_. If you enter _run ..._ several times, the simulation will continue accordingly; only a _restart_ will reset the simulation.

    If you select a signal in the _Wave Window_ with the right mouse button, you can change the value representation under _Radix_ in the context menu. For example, _unsigned decimal_ might be useful for a counter.

    You can save the settings in the _Wave Window_ using the disk symbol: _Save Wavedialog Configuration_. This file will then be added to your project if desired (as prompted in the dialog), and when you start the simulator again, the added signals and all settings will be there.

    If you need to change a VHDL file, you do not need to close the simulator window. Select the relevant file in the relevant tab in the simulator window and make the changes. Don't forget to save the file afterwards. The _Relaunch_ button re-compiles the code and restarts the simulation. Important: If you change the VHDL code and only do a _Restart_, the old code will be simulated! Therefore, a _Relaunch_ must be perdialoged after every code change.

    When you are finished and want to close the simulation, click on the 'X' on the right in the blue title bar of the simulator window. You can also do this later with the other tools such as synthesis or implementation. Vivado is an integrated development environment that starts various tools such as project management, simulator, synthesis or implementation via the _Flow Navigator_ and displays them in an integrated window.

---
## RTL analysis

With the help of the so-called RTL analysis, you can get an impression of the implementation of the VHDL code in hardware. This is not yet the actual logic synthesis step, ie there is no mapping to the target technology. The representation here is done using macroblocks. However, this representation is much clearer than the actual logic synthesis, so that it allows a quick assessment of the VHDL code.

[[F11]]{#F11 label=_F11_}Vivado: Elaborated Design{#F11}

In the _Flow Navigator_ under _RTL Analysis_ click on _Open Elaborated Design_ and confirm the message with _OK_. Vivado now starts the _Elaborated Design_ window and a schematic drawing of the elaborated code should now be visible in the _Schematic_ tab. What Vivado is doing here is basically a semantic analysis of your VHDL code and shows you how the code is implemented in hardware by Vivado. In Figure 11 {reference-type=_ref_ reference=_F11_} you can see the counter register _counter_reg[3:0]_ and the incrementer. You can also see the two flip-flops _detect_reg[1:0]_ and the AND gate for edge detection. This allows you to check whether your code is implemented in hardware by Vivado as you imagined. Close the _Elaborated Design_ window by clicking on the 'X' in the blue title bar.
synthesis

In this section, logic synthesis takes place: it maps the VHDL design onto the components available in the FPGA (flip-flops, look-up tables, multiplexers, RAM). The synthesis can also reveal design errors that may not be visible in the simulation (e.g. incomplete sensitivity lists or incompletely coded IF conditions). It is therefore highly recommended to examine the messages (warnings) of the synthesis closely.

    In the _Flow Navigator_ under _Synthesis_ click on _Run Synthesis_. Do not change anything in the dialog window and confirm with _OK_. The synthesis run will take some time; you can see how far the synthesis has progressed in the Vivado window at the very bottom under _Design Runs_.

    Any warnings or errors will be displayed in the _Messages_ tab. Please check that there are no incomplete sensitivity lists or latches caused by incomplete signal assignments. These are not errors, just warnings, ie your code will be implemented in hardware, but you may end up with hardware that does not work correctly.

    If the design still contains errors (e.g. syntax errors), the synthesis is aborted. In this case, correct the error, run a simulation again and then start the synthesis again.

    When the synthesis run is finished, a window opens. Select _View Reports_. The _Reports_ tab is now highlighted at the bottom of the Vivado window. Double-clicking on the reports opens them in an editor window. Figure 12 {reference-type=_ref_ reference=_F12_} shows an excerpt from the _Utilization Report_, in which you can see that 4 slice LUTs and 6 flip-flops are required to implement the design.

    [[F12]]{#F12 label=_F12_}Utilization Report {#F12}

    In the _Flow Navigator_ under _Synthesis_ click on _Open Synthesized Design_. If you now click on _Schematic_, a window will appear with a schematic drawing of the implementation of your code in the target technology, as shown in Figure 13 {reference-type=_ref_ reference=_F13_}.

    [[F13]]{#F13 label=_F13_} Synthesis Schematic {#F13}

    Close the _Synthesized Design_ window by clicking on the 'X' in the blue title bar.

---
## Definition of constraints

In this section, the _constraints_ for the design are to be defined. The constraints are necessary for the implementation. What is definitely needed is the assignment of the ports of the VHDL code to the pins of the FPGA. Furthermore, you can (and should) also specify temporal constraints. In the simplest case, this is the specification of the maximum clock frequency that the design should be able to achieve.

    If the synthesized design is not open, click _Open Synthesized Design_ under _Synthesis_ in the _Flow Navigator_.

    In the toolbar, select the _I/O Planning_ layout for Vivado instead of the _Default Layout_. You should then get a view like in Figure 14 {reference-type=_ref_ reference=_F14_}.

    [[F14]]{#F14 label=_F14_} Vivado: I/O-Planning View{#F14}

    [[F15]]{#F15 label=_F15_} I/O Planning: I/O Ports {#F15}

    The properties of the I/O ports can be defined under the _I/O Ports_ tab at the bottom, as shown in Figure 15 {reference-type=_ref_ reference=_F15_}. This is where the ports are assigned to the FPGA pins (_Site_), the I/O standards are set, and other features such as driver strength or the _Slew Rate_ are defined. Expand the _ctr_out_ and _Scalar ports_ entries by clicking on the arrow symbol. Now enter the _Sites_ or FPGA pins for the corresponding ports as shown in Figure 15 {reference-type=_ref_ reference=_F15_}. This corresponds to an assignment for the Basys3 board, so that the counter output is on the first four LEDs, two of the buttons are used, and the clock oscillator is connected. You can find more indialogation about this in the _Reference Manual_ for the board if you want to use the board. It is also important to set the _LVCMOS33_ standard under _I/O Std_, as LEDs, buttons and the clock oscillator on the board work with 3.3 V. You can leave the remaining settings at their default values.

    [[F16]]{#F16 label=_F16_} Timing Constraints: Clock frequency{#F16}

    Save the boundary conditions with _File_ → _Constraints_ → _Save_. Confirm the message that the synthesis results may subsequently be out of date with OK. In the following dialog, give the file the name _constr.xdc_.

    To define the boundary condition for the system clock, click on _Constraints Wizard_ under _Synthesis_ in the _Flow Navigator_. In the start window of the _Timing Constraints Wizard_ click once on _Next_. In the following dialog you can enter the frequency of the clock signal _clk_ (Figure 16 {reference-type=_ref_ reference=_F16_}). Under _Recommended Constraints_ enter → Enter the value 100 in _Frequency (MHz)_ (the frequency of the clock oscillator on the BASYS3 board is 100 MHz).

    Skip all other dialog windows with _Skip to Finish_ and close the last dialog with Finish.

    Finish defining constraints by clicking on the cross in the top right corner of the blue title bar of _Synthesized Design_.

    In the _Project Manager_ window you will see under _Sources_ → _Constraints_ the file _constr.xdc_. Open the file by double-clicking it. The file contains commands for creating the constraints defined above. Such a file could also be created manually and added to the project using _Add Sources_. You can also make changes directly here in the file if, for example, the pin assignment is not correct.

    Note: Following this procedure, a synthesis run is first carried out without timing constraints having been defined beforehand. Global optimization goals are then specified during the synthesis (see synthesis settings). If a timing analysis is carried out after the synthesis (analogous to the procedure after implementation described below), it is reported that no constraints are present (entry _Check Timing_). However, after entering the timing constraints, a synthesis run can then be carried out again, in which case the timing constraints are then taken into account (and in all further synthesis runs). However, it should be noted that the delay times in particular are only estimated by the wiring (_net delay_) (see [@Kesel09]).

---
## Implementation

In this section, the _implementation_ is used to place the components on the FPGA and then wire them up. This process is also called _Place&Route_.

    In the _Flow Navigator_ under _Implementation_ click on _Run Implementation_. If you get a message saying that the synthesis is _out of date_, click on _Yes_. This will run the synthesis again. Basically, Vivado checks the time stamps of the source files (VHDL codes, constraints, etc.) and restarts the relevant tools that are affected and also runs all the previous steps again. Basically, if you have changed sources later, it is sufficient to simply click on the last step _Generate Bitstream_ again, so that all the necessary previous steps, such as synthesis and implementation, are then run again.

    After completing the implementation, click on _View Reports_ and check the messages in the _Messages_ tab. To check whether the _timing constraints_ you specified were met, you can search for _Route Design_ in the _Reports_ tab and open the _Timing Summary Report_ there. You can also get a quick overview further up in the _Project Summary_ window.

    Now open the implemented design by clicking on _Open Implemented Design_. A further tab _Timing_ opens below, in which you can find detailed indialogation about the timing (Figure 17 {reference-type=_ref_ reference=_F17_}). If you select _Intra Clock Paths_ in the navigation window on the left in Figure 17 {reference-type=_ref_ reference=_F17_}, you can see the _Intra Clock Paths_ tab. → clk_ you can get more detailed indialogation about the critical paths. Under _Setup_ you will find indialogation about the paths that go from register to register (or from input to register if constraints were specified for the inputs). A positive _slack_ means that the constraints were met (see also [@Kesel09]). Further indialogation can then be found in the respective line: start and end point of the path, total delay and the proportions of the routing and components. Under _Requirement_ you should see the constraints you specified. If you select a line with the mouse, you can call up a context menu with the right mouse button: There you can, for example, use _Schematic_ to display the path in the schema editor or use _View Path Report_ to display detailed indialogation about this path.

    [[F17]]{#F17 label=_F17_} Timing Report {#F17}

---
## Creating the bit file and configuring the FPGA

Before the FPGA can be configured, the results of the implementation run must be converted into a _bitstream_ (a configuration file for the FPGA). The following steps only make sense if you have an FPGA board available.

    In the _Flow Navigator_ under _Program and Debug_ click on _Generate Bitstream_ and wait until the bitstream generation is finished.

    If you have a Basys3 board (or another one) available, connect it to the development computer with a USB cable.

    [[F18]]{#F18 label=_F18_} Hardware Manager {#F18}

    When the bitstream generation is finished, you can click _Open Hardware Manager_. In the upper left corner of the _Hardware Manager_ window (see Figure 18 {reference-type=_ref_ reference=_F18_}), click _Open Target_ → _Auto Connect_. Make sure the board is turned on first!

    You should now see the view shown in Figure 19 {reference-type=_ref_ reference=_F19_}. If you now click on _Program Device_, you will be given the FPGA device to select and then the preset selection of the bitstream file. Click on _Program device_. The FPGA will now be programmed. When this process is finished, your design should be ready for testing on the board!

    [[F19]]{#F19 label=_F19_} Hardware Manager: Programming the target{#F19}

    Note: If no bit file appears under _Program Device_, it is usually because you may have selected the wrong FPGA type when creating the Vivado project. When the Hardware Manager connects to the FPGA, the type designation is read out (window _Hardware Device Properties_). If this does not match the type designation in the bit file, the bit file is not automatically entered for download. If this has happened, you must go to _Settings_ in the Project Manager and correct the FPGA type (_xc7a35tcpg236-1_) and then go through all the steps again by simply clicking on _Generate Bitstream_.

---
## Hints and tips

### Using the syntax checker
