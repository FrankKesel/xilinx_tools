#!/bin/bash
# Author: FK, 01.02.24

# Test H264 webcam streaming over network
gst-launch-1.0 v4l2src io-mode=mmap ! video/x-raw, width=1920, height=1080, framerate=30/1, format=NV12 ! omxh264enc target-bitrate=8500 periodicity-idr=250 ! h264parse ! rtph264pay ! queue ! udpsink host=192.168.178.62  port=50000 sync=false -v
