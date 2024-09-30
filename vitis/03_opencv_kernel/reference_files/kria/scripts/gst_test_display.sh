#!/bin/bash
# Author: FK, 01.02.24

# Test output on display
gst-launch-1.0 videotestsrc ! video/x-raw, width=1280, height=720 ! kmssink driver-name=xlnx plane-id=39 fullscreen-overlay=true -v
