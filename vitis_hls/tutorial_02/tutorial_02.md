# HLS Tutorial 2: Using Directives and Pragmas

In [tutorial 1](../tutorial_01/tutorial_01.md) and [tutorial 1b](../tutorial_01/tutorial_01_b.md) you have learned how to setup a Vitis HLS project. In this tutorial we will show how you can use directives to modify the component and therefore the resulting hardware. The directives can be stored either in the configuration file (we have done this already in [tutorial 1](../tutorial_01/tutorial_01.md)) or in the source code. In the latter case the directives are called _pragmas_.

For the sake of simplicity we will use the same source code as in [tutorial 1](../tutorial_01/tutorial_01.md). In the configuration file [sol1_config.cfg](../tutorial_02/reference_files/sol1/sol1_config.cfg) you will find the directive `syn.compile.pipeline_loops=0` which prevents the component from being automatically loop pipelined and the directive `syn.compile.enable_auto_rewind=0` which prevents auto rewind of the loop (see the discussion in [tutorial 1b](../tutorial_01/tutorial_01_b.md)). Although you would normally accept an automatic loop pipelining we will first generate an unpipelined and therefore non-optimized component (sometimes called a _baseline implementation_) and then apply a pragma in the source code for loop pipelining in order to show how pragmas can be applied to the code. 

After that we will show how you can apply directives in order to modify the interfaces of the component. These interface directives can be also either stored in the source code as pragmas or in the configuration file.

## Setting up the component and applying loop pipelining
* Download the folder `tutorial_02/reference_files` and rename this folder to `tutorial_02`. You will now have a similar folder structure as in [tutorial 1b](../tutorial_01/tutorial_01_b.md) but now we have only one solution or component which we will modify in the Vitis GUI. 
* Open a Linux terminal, go to the folder `tutorial_02` and execute the bash script `run_demo.sh`. Then start the Vitis GUI and open the workspace `tutorial_02`. Since synthesis has already been run you can open the synthesis report as it is shown in the following image.

![Vitis GUI with opened workspace](images/hls_50.png)

* As you can see in the synthesis report the loop is not pipelined, this is due to the fact that we have applied the directive `syn.compile.pipeline_loops=0` in the configuration file `sol1_config.cfg` (Which you can verify by opening the configuration file).
* Now open the source file `fir.c` in the editor window by clicking on the file name in the _Vitis Components Explorer_. On the right side of the Vitis GUI you can see the symbol for the _HLS Directives_ (marked in red in the next image). When you click on this symbol you should see the pane for the _HLS directives_ as shown in the next image. You can see here the objects from the source code for which you can apply directives. 

![Vitis GUI directives](images/hls_51.png)

* When you hover the mouse over the loop _Shift_Accum_Loop_ (this is the reason why you should name the loops in the code) in the _HLS Directives_ pane you can see a `+` sign. Click on the `+` sign and you should see a dialog for entering the directive as shown in the next image. In the upper field select `PIPELINE`. You can choose to either store the directive in the source code as _pragma_ or in the configuration file. Keep it in the source file for the moment. The rest of the fields can be left blank. Press the `OK` button to close the dialog.

![Vitis GUI loop directive](images/hls_52.png)

* When you close the dialog you should see the directive embedded as pragma in the code as can be seen in the next image.

![Vitis GUI loop pragma](images/hls_53.png)

* Start a new _C Synthesis_ run and open the synthesis report. You should see that the loop has been pipelined leading to an improvement of the latency and interval.

![Vitis GUI loop pragma](images/hls_54.png)

* When you hover again over the `# HLS PIPELINE` directive in the _HLS Directives_ pane you can see now a pencil symbol which allows you to edit the directive. Edit the directive and choose now `Config File` instead of `Source File`, press `OK`. Open the configuration file `sol1_config.cfg` and select the source editor view. You should see now the directive `syn.directive.pipeline=fir/Shift_Accum_Loop` stored in this file as shown in the next image. In the rendered view _Settings Form_ of the configuration file you can find the directive in the section `Design Directives > Pipeline`

![Vitis GUI loop directive](images/hls_55.png)

* When you run synthesis you will see that you get the same result as in the case when you stored the directive as pragma in the source code. So what is better, storing the directives in the source code or in the configuration file? From our experience directives which will not change, if you want to generate several variants, should be stored as pragmas in the source code, for example interface directives (see next section). Directives which may change from variant to variant, like optimization directives as loop pipelining, are better stored in the configuration file, since you will have for each variant a separate component with a configuration file. The advantage of storing the directives as pragma in the source code is that you have everything in one place, on the other hand the source code may be cleaner if you store the directives in the configuration file. So, it depends and you have to choose which is best for you and your project.
* Maybe you are a bit confused now by the two pipeline directives in the configuration file `syn.compile.pipeline_loops=0` and `syn.directive.pipeline=fir/Shift_Accum_Loop`? Loop pipelining is a very efficient optimization and therefore Vitis HLS applies loop pipelining by default to all loops in a component (nested loops have to be discussed in more detail later). If you want to selectively apply loop pipelining, as in the example here, you first have to switch the global optimization off (`syn.compile.pipeline_loops=0`) and then apply loop pipelining to dedicated loops (`syn.directive.pipeline=fir/Shift_Accum_Loop`). 


## Applying interface directives
* So far we did not cover the interface of a component. In terms of the source code the interface are the arguments of the function. With respect to the hardware implementation the interface are the ports of the component. There are default implementations for the hardware interfaces depending on the data type of the arguments. For our example you can get information on the interfaces when you scroll down the synthesis report, as shown in the next image.

![Vitis GUI interfaces](images/hls_56.png)

* Open also the source code `fir.c`. The argument `c[N]` are the coeffients of the FIR filter and it is an array of data type `int`. For arrays the default implementation is `ap_memory`, which means that it is assumed that for example a _Block RAM_ will be attached later to the component (refer to https://docs.amd.com/r/en-US/ug1399-vitis-hls/Interfaces-for-Vivado-IP-Flow). In the synthesis report you can see that there is a port `c_address0` with a bit width of 4 bit. This is the address port for the Block RAM and since the array has a size of 11 elements (see the header file `fir.h`) we need 4 bit to address them. Each element of the array has a size of 32 bit due to the data type `int` and therefore the data port `c_q0` for the Block RAM has a size of 32 bit. The address port is an output of the component and the data port is an input. There will be an additional port `c_ce0` for the Block RAM interface, which is not shown here (this is the so-called _clock enable_). 
* The other arguments are `x` as a scalar argument and `y` as a pointer. For a scalar argument the default implementation is `ap_none`, which means you simply get an imput port with a bit width of 32 bit due to the `int` data type used. For the pointer `y` Vitis HLS can deduce from the code that the pointer is only written to and therefore an output port is sufficient. The output port gets an additional valid signal (`y_ap_vld`), which is meant by the mode `ap_vld`.
* Besides the discussed ports there are also top level control signals like clock and reset (`ap_clk`, `ap_rst`) and the block level control signals `ap_ctrl`, which can also be seen in the synthesis report. 
* When you scroll further down in the synthesis report you will find also the section _SW-to-HW Mapping_ which gives you a bit more details on the ports. You can also open the toplevel VHDL source code, which has been generated by Vitis HLS, if you navigate in the _Vitis Components Explorer_ under _Output_ to the file `syn/vhdl/fir.vhd` and open it in the text editor by double clicking. As shown in the next image you can see the top level control ports in green, the ports for argument `y` in yellow, the ports for the array `c[N]` in red and the port for the scalar argument `x` in blue and these are exactly the ports from the _SW-to-HW Mapping_ section.

![Vitis GUI VHDL](images/hls_57.png)

* We will come back on the interface topic in a later tutorial in more detail. Here we just want to show you how you can modify the default implementation of the interface by applying a directive, in this case for argument `x` as an example. 
* Open the source code `fir.c` in the Vitis GUI text editor (you should already have it opened). Open also the _HLS Directives_ pane and hover the mouse over the argument `x`. You should see again the `+`sign, push it and you will see the _Add Directive_ dialog as shown in the next image. Select `INTERFACE` from the pull-down menu and select the mode `ap_vld`, keep it in the source file. Push `OK` to close the dialog.  

![Vitis GUI interface](images/hls_58.png)

* When you close the dialog you should see now the directive as pragma in your source code as shown in the next image. Run synthesis again, open the synthesis report and the generated VHDL code. You can see now that the argument `x` has an additional valid port `x_ap_vld` as input. This is a so-called _handshake_ signal and tells the component at which point in time the input signal `x` is valid and can be read by the component. In the default implementation there is no handshaking and that means that the value on port `x` must be valid (i.e. stable) over the _whole_ operation time of the component, since the component may read `x` at any time. In the same sense the default handshake port `y_ap_vld` for argument `y` shows when the output `y` of the component is valid (to the outside world).

![Vitis interface pragma](images/hls_59.png)
