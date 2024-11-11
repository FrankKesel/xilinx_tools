# HLS script for convolution demo
# FK, 11.01.2023

# Set path to Vitis vision library. Adapt this to your path!
set vision_path /home/Data/Daten_diverse/xilinx/vitis_vision_l1_2021_2/include

# Open project
open_project -reset hls_proj
# Set toplevel of design (function name)
set_top conv_top

# Add source files for design (only cpp needed, no header files)
# Note: Inclusion of Vitis Vision headers must be adapted to your paths
# c++14 compiler flags needed
add_files src/conv_top.cpp -cflags "-I$vision_path -D__SDSVHLS__ -std=c++14"
# Add source files for testbench
add_files -tb src/conv_main.cpp -cflags "-I$vision_path -D__SDSVHLS__ -std=c++14" -csimflags "-I$vision_path"
# Add the images folder to the testbench
add_files -tb ../data

# Open solution, use Vitis flow (generate .xo-files)
open_solution -flow_target vitis -reset "solution1"
# Use Ultrascale Zynq (Kria KV260)
set_part {xck26-sfvc784-2LV-c}
# Set clock period (ns)
create_clock -period 5 -name default

# Config AXI bus interface: No 64 Bit addresses, 32 Bit addresses for Zynq
config_interface -m_axi_addr64=0

# Design steps
# Simulation of C-Code
csim_design
# HLS
csynth_design
# Co-Simulation of generated IP Core VHDL code
cosim_design -trace_level port -rtl vhdl
# Export the design as IP Core
export_design -rtl vhdl -format xo -description "convolution IP Core" -vendor "hspf" -version "1.0" -display_name "convolution"

exit
