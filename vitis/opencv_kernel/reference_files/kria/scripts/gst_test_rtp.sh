#!/bin/bash
# Author: FK, 01.02.24

# Test RTP streaming over network
gst-launch-1.0 videotestsrc ! video/x-raw, width=320, height=240, format=RGB, framerate=30/1 !  rtpvrawpay ! udpsink host=141.47.69.212 port="50000"
