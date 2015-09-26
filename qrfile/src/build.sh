#!/bin/bash

gcc -c hexify.c
gcc -c md5_rhash.c
gcc -c fsize.c
gcc -lrhash *.o main.c
