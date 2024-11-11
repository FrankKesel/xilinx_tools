#!/bin/bash
# Author: FK, 01.02.24

# Test H264 streaming over network
gst-launch-1.0 videotestsrc ! video/x-raw, width=720, height=480, format=NV12, framerate=30/1 ! omxh264enc target-bitrate=1800 periodicity-idr=240 ! h264parse ! rtph264pay ! udpsink host=141.47.208.237 port=50000 -v
