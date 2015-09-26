#include <stdlib.h> // EXIT_SUCCESS, etc
#include <stdio.h> // fopen(), etc
#include <stdint.h>
#include <rhash.h>
#include "hexify.h"
#include "md5_rhash.h"
#include "errors.h"

int main(int argc, char *argv[])
{
    md5_hash_t checksum;

    FILE *fp;
    {
        fp = fopen("file", "r");
        if (fp == NULL) exit(EXIT_FAILURE_fopen_failed);
        
        int result = md5_rhash(fp, &checksum);
        if (result != 0) exit(result);
    }
    if (fclose(fp) != 0) exit(EXIT_FAILURE_fclose_failed);

    char hex_buffer[128+1];
    hexify(checksum.bytes, sizeof(checksum.bytes), hex_buffer, sizeof(hex_buffer));
    printf("bytes: %s\n", hex_buffer);

    printf("hex: %s\n", checksum.hex);

    return EXIT_SUCCESS;
}

