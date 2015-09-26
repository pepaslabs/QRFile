#include <stdlib.h> // EXIT_SUCCESS, etc
#include <stdio.h> // fopen(), etc
#include <stdint.h>
#include <rhash.h>
#include "hexify.h"
#include "md5_rhash.h"
#include "fsize.h"
#include "errors.h"


int main(int argc, char *argv[])
{
    char *fpath = "file";

    md5_hash_t hash;

    FILE *fp;
    {
        // open the file
        fp = fopen(fpath, "r");
        if (fp == NULL) exit(EXIT_FAILURE_fopen_failed);
        
        // generate an md5 hash of the file
        int result = md5_rhash(fp, &hash);
        if (result != 0) exit(result);

        // read the size of the file
        off_t size;
        result = fsize(fp, &size);
        if (result != 0) exit(result);
        printf("file size: %i\n", size);

        // calculate number of QR codes needed
        int chunk_size = 1091 - 16 - 2;
        int num_chunks = size / chunk_size;
        if (size % chunk_size) num_chunks++;

        // create the chunks to send to libqrencode
        rewind(fp);

    }
    if (fclose(fp) != 0) exit(EXIT_FAILURE_fclose_failed);

    char hex_buffer[32+1];
    hexify(hash.bytes, sizeof(hash.bytes), hex_buffer, sizeof(hex_buffer));
    printf("bytes: %s\n", hex_buffer);

    printf("hex: %s\n", hash.hex);

    return EXIT_SUCCESS;
}

