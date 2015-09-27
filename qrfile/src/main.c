#include <stdlib.h> // EXIT_SUCCESS, etc
#include <stdio.h> // fopen(), etc
#include <stdint.h>
#include <rhash.h>
#include "hexify.h"
#include "md5_rhash.h"
#include "fsize.h"
#include "errors.h"

struct _qrchunk_info_23L_t
{
    uint8_t document_id[16];
    uint16_t sequence_number;
    uint8_t file_data[1073];
};
typedef struct _qrchunk_info_23L_t qrchunk_info_23L_t;

union _qrchunk_23L_t
{
    qrchunk_info_23L_t info;
    uint8_t bytes[sizeof(qrchunk_info_23L_t)];
};
typedef union _qrchunk_23L_t qrchunk_23L_t;

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

        qrchunk_23L_t chunk;
        // calculate number of QR codes needed
        int chunk_size = sizeof(chunk.info.file_data);
        int num_chunks = size / chunk_size;
        if (size % chunk_size) num_chunks++;
        printf("num chunks: %i\n", num_chunks);

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

