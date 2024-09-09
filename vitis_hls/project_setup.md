# Project Setup


---
## Overview
* In the following the setup of Xilinx Vitis HLS component projects is described.
* The project setup can be done from the Vitis GUI, this is described in [Creating a HLS component](#creating-a-hls-component). 
* You can setup a project also on the command line, this is described in [Creating components from the command line](#creating-components-from-the-command-line). You can then open the Vitis GUI and open the project workspace and continue working with the project. We recommend to setup a project on the command line and then continue in the GUI.
* It is essential that you understand the concept of workspaces and components before you start with a component project. This is described in [Workspaces and components](#workspaces-and-components).
* All setup data of a component is stored in a configuration file. If you setup a component project on the command line you need such a configuration file. You can find the content of a sample file in [Creating components from the command line](#creating-components-from-the-command-line). If you make changes later on in the Vitis GUI all changes will be stored in the same configuration file which you used for setting up the project.
* Xilinx SW version: Vitis 2024.1


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

---
## Starting Vitis
* You can launch Vitis in a terminal on the command line by using the following command:

```
source <Vitis_Installation_Directory>/settings64.sh
vitis  -w <workspace>
```
* Where `<workspace>` indicates a folder to hold the elements of your HLS component (HLS doc), or other design projects. The workspace is used to group together the source and data files that make up a design, or multiple designs, and stores the configuration of the tool for that workspace (see [Workspaces and components](#workspaces-and-components)).
* Or open Vitis without a workspace and select a workspace in the Vitis GUI.

---
## Creating a HLS component

* With the Vitis IDE opened, from the main menu select `File > New Component > HLS` 
	* You can also select `New Component` on the Welcome page, or from the right-click menu in the Explorer view.
* This opens the Choose Name and Location page of the Create HLS Component wizard. Enter component name and location (i.e. the workspace) and then `Next`.
* This opens the Configuration File page of the wizard as shown below. The HLS component configuration file contains commands and directives used for the synthesis, simulation, and export of the component.
* The Configuration File page of the Create HLS Component wizard provides three ways to add a configuration file to your component:
	* Empty File: Specifies the creation of a new empty configuration file to be populated after component creation . Selecting this option lets you specify a name in the New configuration file name field.
	- Existing File: Specifies the use of an existing configuration file. Selecting this option lets you enter the path and name in the File field, or select Browse to locate the file.
	- Generate from Vitis HLS Project: This option generates a new config file from an existing Vitis HLS project hls.app file. The tool lets you identify the hls.app file inside the project, and enter a New configuration file name for the config file generated from the hls.app file.
	- Choose the method of adding the config file to your HLS component, and specify a name or files as needed. Select `Next` to proceed to the Source Files.
- The Source Files page of the wizard opens:
	- Under Design Files you can select the `Add files` symbol to select the primary source file for the design, or add multiple files as needed. After adding one or more source files, you can identify the Top Function which is the function to be synthesized as the top-level function for the HLS component. Any functions called by the top-function will also be synthesized into RTL.
	- Under Test Bench Files you can select `Add files` symbol or select `Add folders` to add files containing test bench and associated files for the HLS component.
	- For both design files and test bench files you can add CFLAGS and CSIMFLAGS to specify directives to be used during compilation and simulation. You can specify directives to be applied to all files or to specific files from each category. After adding files and folders and directives, select Next to proceed.
- The Select Part page of the wizard is opened. Select the Part or select the Platform for the HLS component and select `Next` to proceed. 
	- If you select Platform when creating the HLS component, the Vitis unfied IDE extracts the part from the specified platform and adds `part=` to your configuration file.
- This displays the Settings page of the wizard. 
	- The Settings page lets you specify a clock period or frequency for the design, and a clock uncertainty. The default clock for the tool is a clock period of 10 ns (or 100 MHz), and a clock uncertainty of 27%
	- The Settings page also lets you specify the flow_target for the HLS component build process as being either to generate a Vivado IP or a Vitis kernel as described in [Target Flow Overview](https://docs.amd.com/r/4lwvWeCi9jb~DWzdfWuVQQ/P4DbGaxyPEWYEJfsO7o1Dw). The interface requirements of the Vivado IP or a Vitis kernel are different as explained in [Interfaces of the HLS Design](https://docs.amd.com/r/4lwvWeCi9jb~DWzdfWuVQQ/wEdlxulOAT50bjDEn06U5Q). The default flow_target is the Vivado IP flow.
		- Note: Selecting the Vitis flow_target does not automatically change the output format to Vitis kernel. The default is still Vivado IP, but you can manually change the output format if needed.
	- Below the flow_target you can also specify the package.output.format for the tool to generate when packaging the design. The default output format is the Vivado IP, which generates a .zip file to be added to your IP catalog, letting you use the HLS generated RTL design in other designs.
	- Select `Next` to proceed to the Summary page.
- The Summary page reflects the choices you have made on the prior pages. Review the summary and select Finish to create the HLS component, or select Back to return to earlier pages and change your selections.
	- When the HLS component is created the vitis-comp.json file for the component is opened in the central editor window, and the HLS component becomes active in the Flow Navigator as shown below.
	- When multiple HLS components are opened, the component name appears in the tab alongside the vitis-comp.json file name to help you identify the different components.
- After creation, the HLS component becomes the active component in the Flow Navigator, or you can select the Component to make it the active component in the tool. When the HLS component is the active component, the Flow Navigator enables running C Simulation, C Synthesis, C/RTL Co-simulation, Package, and Implementation.

---
## Creating components from the command line
* If you want to generate components from the command line, e.g. for comparing variants you can run Vitis HLS from the command line. Switch to the component folder (directory structure as described above) and execute the following commands. This will setup a component project and start a HLS synthesis run, according to the settings given in the configuration file  `component1_config.cfg`. All output data will be stored in the work directory `component_work`.
  
```
source <vitis installation path>/Vitis/<Version>/settings64.sh
v++ -c --mode hls --config ./component1_config.cfg --work_dir component_work
```

* _Tip_: Put the commands in a bash shell script and execute the script.
* The command or script should be executed in the component subdirectory of a workspace, see [Workspaces and components](#workspaces-and-components).
* Below you can see the content of a sample configuration file. Refer to the [Vitis HLS documentation](https://docs.amd.com/r/en-US/ug1399-vitis-hls/HLS-Config-File-Commands) for the meaning of the entries in the file.
```
part=xc7a35tcpg236-1

[hls]
flow_target=vivado
package.output.format=ip_catalog
package.output.syn=false
syn.top=fir
syn.file=../src/fir.c
tb.file=../src/fir_test.c
tb.file=../src/out.gold.dat
clock=10ns
clock_uncertainty=10%
csim.code_analyzer=1
```
