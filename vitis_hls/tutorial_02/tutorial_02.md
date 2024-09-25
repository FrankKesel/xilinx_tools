# HLS Tutorial 2: Using Directives and Pragmas

In [tutorial 1](../tutorial_01/tutorial_01.md) and [tutorial 1b](../tutorial_01/tutorial_01_b.md) you have learned how to setup a Vitis HLS project. In this tutorial we will show how you can use directives to modify the component and therefore the resulting hardware. The directives can be stored either in the configuration file (we have done this already in [tutorial 1](../tutorial_01/tutorial_01.md)) or in the source code. In the latter case the directives are called _pragmas_.

For the sake of simplicity we will use the same source code as in [tutorial 1](../tutorial_01/tutorial_01.md). In the configuration file [sol1_config.cfg](../tutorial_02/reference_files/sol1/sol1_config.cfg) you will find the directive `syn.compile.pipeline_loops=0` which prevents the component from being automatically loop pipelined and the directive `syn.compile.enable_auto_rewind=0` which prevents auto rewind of the loop (see the discussion in [tutorial 1b](../tutorial_01/tutorial_01_b.md)). Although you would normally accept an automatic loop pipelining we will first generate an unpipelined and therefore non-optimized component (sometimes called a _baseline implementation_) and then apply a pragma in the source code for loop pipelining in order to show how pragmas can be applied to the code. 

After that we will show how you can apply directives in order to modify the interfaces of the component. These interface directives can be also either stored in the source code as pragmas or in the configuration file.

## Setting up the component and applying loop pipelining
* Download the folder `tutorial_02/reference_files` and rename this folder to `tutorial_02`. You will now have a similar folder structure as in [tutorial 1b](../tutorial_01/tutorial_01_b.md) but now we have only one solution or component which we will modify in the Vitis GUI. 
* Open a Linux terminal, go to the folder `tutorial_02` and execute the bash script `run_demo.sh`. Then start the Vitis GUI and open the workspace `tutorial_02`. Since synthesis has already been run you can open the synthesis report as it is shown in the following image.

![Vitis GUI with opened workspace](images/hls_50.png)

* As you can see in the synthesis report the loop is not pipelined, this is due to the fact that we have applied the directive `syn.compile.pipeline_loops=0` in the configuration file `sol1_config.cfg` (Which you can verify by opening the configuration file).
* Now open the source file `fir.c` in the editor window by clicking on the file name in the _Vitis Components Explorer_. On the right side of the Vitis GUI you can see the symbol for the _HLS Directives_ (marked in red in the next image). When you click on this symbol you should see the window for the HLS directives as in the next image. You can see here the objects from the source code for which you can apply directives. 

![Vitis GUI directives](images/hls_51.png)

* When you hover the mouse over the loop _Shift_Accum_Loop_ (this is the reason why you should name the loops in the code) you can see a `+` sign. Click on the `+` sign and you should see a dialog for entering the directive. In the upper field select `PIPELINE`. You can choose to either store the directive in the source as _pragma_ or in the configuration file. Keep it in the source file. The rest of the fields can be left blank.

![Vitis GUI loop directive](images/hls_52.png)

* When you close the dialog you should see the directive embedded as pragma in the code as can be seen in the next image.

![Vitis GUI loop pragma](images/hls_53.png)

* Start a new _C Synthesis_ run and open the synthesis report. You should see that the loop has been pipelined leading to an improvement of the latency and interval.

![Vitis GUI loop pragma](images/hls_54.png)
