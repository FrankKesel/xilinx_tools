#!/usr/bin/env python3

# Convert an 8 Bit grey value binary file to a BMP file
# FK, 26.1.23

import cv2 as cv
import numpy as np
import os.path
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--width", type=int)
parser.add_argument("--height", type=int)
parser.add_argument("--filename", type=str)
args = parser.parse_args()
width = args.width
height = args.height
filename = args.filename

print("--- Convert 8 Bit grey image in binary format to BMP file ---")
# Check if file exists
if os.path.isfile(filename) == False:
    print("File  <" + str(filename) + ">  does not exist.")
    quit()

print("Width: " + str(width))
print("Height: " + str(height))

input_bin_file_name = filename
# Change the extension  of the output file to .bmp
output_file_name = filename.rsplit( ".", 1 )[ 0 ] + ".bmp"

# Load input file
print("Read input file: " + str(input_bin_file_name))
img_vec = np.fromfile(input_bin_file_name, dtype='uint8')
img_mat = img_vec.reshape(height, width)

# Export file as BMP file
print("Write output file: " + str(output_file_name))
cv.imwrite(output_file_name, img_mat);

