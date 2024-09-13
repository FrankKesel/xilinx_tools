# HLS Tutorial 1b: Creating components from the command line


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
