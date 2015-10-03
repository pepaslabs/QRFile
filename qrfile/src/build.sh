#!/bin/bash

# build dependencies:
#
# debian users:
# apt-get install librhash-dev
#
# OS X / homebrew users:
# brew install rhash
# export C_INCLUDE_PATH=/usr/local/include
# export LIBRARY_PATH=/usr/local/lib
# export LD_LIBRARY_PATH=/usr/local/lib

gcc -lrhash *.c
