#!/bin/sh
./susan input_large.pgm output_large.smoothing.pgm -s -3
./susan input_large.pgm output_large.edges.pgm -e -3
./susan input_large.pgm output_large.corners.pgm -c -3

