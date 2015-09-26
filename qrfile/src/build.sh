#!/bin/bash

gcc -c hexify.c
gcc -c md5_rhash.c
gcc -lrhash *.o main.c
