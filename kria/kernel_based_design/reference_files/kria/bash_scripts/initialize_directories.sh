#!/bin/bash
# This script makes and initializes the directory for the HW (firmware) and
# makes the project directory for Jupyter notebooks. Adapt the project name.
# Author: F.Kesel, 8.8.2023

# Define project name
project_name="vadd"

echo "Initialize project directories:"

fw_dir="/lib/firmware/xilinx/"
fw_target=${fw_dir}${project_name}

jupyter_dir="/home/root/jupyter_notebooks/projects/"
jupyter_target=${jupyter_dir}${project_name}

# Make firmware dir and copy shell.json
sudo mkdir ${fw_target}
sudo mv shell.json ${fw_target}/shell.json

# Check files
sudo ls -al ${fw_target}

# Make jupyter_dir
sudo mkdir ${jupyter_target}
sudo ls -al ${jupyter_dir}
