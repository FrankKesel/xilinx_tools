#!/bin/bash
# This script unloads a running PL HW on the Kria system
# and loads the project HW (firmware). Adapt the name of the project.
# Author: F.Kesel, 8.8.2023

# Define project name and paths
project_name="vadd"

echo "Load app:"

# Load app
sudo xmutil listapps
sudo xmutil unloadapp
sudo xmutil loadapp ${project_name}
sudo xmutil listapps
