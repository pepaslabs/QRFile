#!/bin/bash

gcc -c hexify.c
gcc -lrhash *.o md5.c
