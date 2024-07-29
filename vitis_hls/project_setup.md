# HLS 24.1

---
## Starting Vitis
* You can launch this tool by using the following command:

```
source <Vitis_Installation_Directory>/settings64.sh
vitis  -w <workspace>
```
* Where `<workspace>` indicates a folder to hold the elements of your HLS component (HLS doc), or other design projects. The workspace is used to group together the source and data files that make up a design, or multiple designs, and stores the configuration of the tool for that workspace.
* Or open Vitis without workspace and select a workspace.

---
## Workspaces and components
* A workspace can contain more than one component. If you want to generate several variants of an IP core, in order to compare them, you must generate for each variant a component (this was called solutions in the old Vitis version).
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
* If you want to generate components from the command line, e.g. for comparing variants you can run Vitis HLS from the command line. Switch to the component folder (directory structure as described above) and execute the commands: 
```
source <vitis installation patj>/Vitis/<Version>/settings64.sh
v++ -c --mode hls --config ./component1_config.cfg --work_dir component_work

```
* Tip: Put the commands in a bash shell script and execute the script.