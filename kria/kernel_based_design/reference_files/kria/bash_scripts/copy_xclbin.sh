#!/bin/bash
# This script copies the FPGA binary and the device tree file to
# the firmware directory on the Kria system. Should be executed
# when the HW design has been generated or modified. Copy the the files
# binary_container_1.xclbin and pl.dtbo from your HLS/Vitis project to
# the Kria project directory. Adapt the project name to the name of
# your project below.
# Author: F.Kesel, 8.8.2023

# Define project name
project_name="vadd"
xclbin_name="vadd_hw.xclbin"

echo "Copy the FPGA binary to firmware directory:"

# Path definition for firmware (do not change)
fw_dir="/lib/firmware/xilinx/"
fw_target=${fw_dir}${project_name}

# Copy files
sudo cp ${xclbin_name} ${fw_target}/binary_container_1.bin
sudo cp pl.dtbo ${fw_target}/pl.dtbo

# Check files
sudo ls -al ${fw_target}
