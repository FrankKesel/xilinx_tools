#!/bin/bash
# Author: FK, 01.02.24

# Test webcam, output to display
gst-launch-1.0 v4l2src device="/dev/video0" ! video/x-raw, width=1280, height=720, format=YUY2 ! kmssink driver-name=xlnx plane-id=39 fullscreen-overlay=true -v
