#!/bin/bash
echo "Load app:"
# Define project name and paths
project_name="convolution"

# Load app
sudo xmutil listapps
sudo xmutil unloadapp
sudo xmutil loadapp ${project_name}
sudo xmutil listapps
