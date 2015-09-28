#!/bin/bash

# note: you will need to run the following:
# apt-get install librhash-dev

gcc -c hexify.c
gcc -c md5_rhash.c
gcc -c fsize.c
gcc -lrhash *.o main.c
