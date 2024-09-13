# HLS Tutorial 1b: Creating components from the command line

---
## Overview
* In tutorial 1 you have set up a project with the Vitis GUI. Since this is quite time consuming it is often better to setup and run projects from the command line (in a Linux terminal). This is especially true if you want to examine several variants of a component. 
* In this tutorial we will setup basically the same component as in [tutorial 1](tutorial_01.md), but generate two different variants. Fortunately all settings for a component or variant are stored in _one_ configuration file, so the only thing you need is the configurations files for the variants and some Linux bash shell scripts.
* For this tutorial you should have completed the [tutorial 1](tutorial_01.md) since we will not explain the details of the tool steps (simulation, synthesis, co-simulation etc.) here again.

---
## Running the bash scripts
* First download the complete folder `reference_files` to your computer and rename the folder to `fir_ws`. This folder will later on be your workspace which you will open in the Vitis GUI.
* We will first explain the folder structure of the workspace folder `fir_ws`. In the workspace folder you find a bash script `run_demo.sh` which basically starts the two bash scripts `sol1.sh` and `sol2.sh` in the sub-folders `sol1` and `sol2`. These sub-folders hold the two variants (which are in the Vitis terminology two _components_).

```
fir_ws
    run_demo.sh
	|-src
        ... (sources)
	|-sol1
		|-sol1_work
		sol1_config.cfg
        sol1.sh
	|-sol2
		|-sol2_work
		sol2_config.cfg
        sol2.sh
```
* If you look into the bash script `sol1.sh` (`sol2.sh` is basically the same for `sol2`) you will find the following code. The first line says that this is a script to be executed as bash shell script. The second line sources the necessary settings from the Vitis installation. In the third line Vitis HLS is executed in script mode and with the `--config` switch we tell Vitis to use the configuration file. The last switch `--work_dir` specifies that this directory should be used for all output of Vitis. Do not confuse this directory with the Vitis workspace directory (which is in fact a bit confusing) which is the root directory `fir_ws` in which we have the two components. 
 
```
#!/bin/bash
source /opt/xilinx/Vitis/2024.1/settings64.sh
v++ -c --mode hls --config ./sol1_config.cfg --work_dir sol1_work
```

* Before you can run the shell scripts you have to make them executable with `sudo chmod a+x run_demo.sh` (in a Linux terminal) and the same also for the two sub-scripts `sol1.sh` and `sol2.sh`.
* Then go back to the workspace directory `fir_ws.sh` and execute to top-level script with `./run_demo.sh`. You should now see the ouput of Vitis running. Basically the HLS step is executed for both variants.
* If Vitis is finished open the Vitis GUI. On the Welcome page under _Get Started_ push the `Open Workspace` button. Navigate to the workspace folder `fir_ws` and select it. Now you should see the opened workspace as in the following image. In the _Vitis Components Explorer_ you can see now two components `sol1` and `sol2`, `sol1` is selected in the _Flow Navigator_. For both components `C SYNTHESIS` has already been executed. 

![Vitis GUI with opened workspace](images/hls_30.png)
 
---
## 
