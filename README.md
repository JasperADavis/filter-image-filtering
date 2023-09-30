# Filter - Image Filtering Tool

## Overview

This is a CLI program written in C that takes an existing image `INFILE`, a `flag` representing a filter to be applied to said image, and outputs the end result to a specified file `OUTFILE`.


## Instructions
This program is launched from the command-line:
`./filter [flag] INFILE OUTFILE`

Available flags:
- `b`: blur image
- `g`: grayscale
- `r`: reflect the image
- `s`: sepia
