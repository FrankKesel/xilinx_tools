#!/usr/bin/env python3

#Convert 8 Bit BMP file to text and binary file
#FK, 26.1.23

import cv2 as cv
import numpy as np
import os.path
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--filename", type=str)
args = parser.parse_args()
filename = args.filename

print("--- Convert BMP file to 8 Bit grey image in binary format ---")
# Check if file exists
if os.path.isfile(filename) == False:
    print("File  <" + str(filename) + ">  does not exist.")
    quit()

input_file_name = filename
# Change the extension  of the output file to .bin
bin_file_name = filename.rsplit( ".", 1 )[ 0 ] + ".bin"
# Change the extension  of the output file to .txt
out_file_name = filename.rsplit( ".", 1 )[ 0 ] + ".txt"

# Read PNG image, should be 8 bit gray, 3 channels
print("Read input file: " + str(input_file_name))
in_mat = cv.imread(input_file_name, cv.IMREAD_UNCHANGED)
print("Image size: " + str(in_mat.shape))
print("Image data type: " + str(in_mat.dtype))

print("Write output file: " + str(out_file_name))
out_file = open(out_file_name, 'w')
np.savetxt(out_file, in_mat, fmt='% 3d')
out_file.close()

# Binary output
print("Write output file: " + str(bin_file_name))
in_mat.tofile(bin_file_name)
