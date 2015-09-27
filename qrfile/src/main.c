#include <stdlib.h> // EXIT_SUCCESS, etc
#include <stdio.h> // fopen(), etc
#include <stdint.h>
#include <rhash.h>
#include "hexify.h"
#include "md5_rhash.h"
#include "fsize.h"
#include "errors.h"
#include <string.h> // memcpy()

#define QR_23L_8BIT_SIZE 1091

struct _qrchunk_info_23L_t
{
    uint8_t document_id[MD5_SIZE];
    uint16_t sequence_number;
    uint8_t file_data[QR_23L_8BIT_SIZE - (MD5_SIZE + sizeof(uint16_t))];
};
typedef struct _qrchunk_info_23L_t qrchunk_info_23L_t;

union _qrchunk_23L_t
{
    qrchunk_info_23L_t info;
    uint8_t bytes[sizeof(qrchunk_info_23L_t)];
};
typedef union _qrchunk_23L_t qrchunk_23L_t;

struct _qrchunk_descriptor_23L_t
{
    qrchunk_23L_t chunk;
    size_t used;
};
typedef struct _qrchunk_descriptor_23L_t qrchunk_descriptor_23L_t;

int make_next_chunk(FILE *fp, qrchunk_descriptor_23L_t *qr)
{
    size_t bytes_read = fread(qr->chunk.info.file_data, sizeof(uint8_t), sizeof(qr->chunk.info.file_data), fp);
    if (ferror(fp)) return EXIT_FAILURE_fread_failed2;
    if (bytes_read == 0) return EXIT_FAILURE_fread_returned_zero_bytes;
    qr->used = bytes_read;
    printf("read %i bytes as chunk\n", qr->used);

    // process the chunk with libqrencode
    // FIXME

    return 0;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    {
        int result = 0;

        // open the file
        char *fpath = "file";
        fp = fopen(fpath, "r");
        if (fp == NULL) exit(EXIT_FAILURE_fopen_failed);
        
        // read the size of the file
        off_t size;
        result = fsize(fp, &size);
        if (result != 0) exit(result);
        printf("file size: %i\n", size);

        // generate an md5 hash of the file
        md5_hash_t hash;
        result = md5_rhash(fp, &hash);
        if (result != 0) exit(result);
        rewind(fp);
        printf("md5: %s\n", hash.hex);

        qrchunk_descriptor_23L_t qr;
        memcpy(qr.chunk.info.document_id, hash.bytes, sizeof(hash.bytes));
        qr.chunk.info.sequence_number = 0;

        // calculate number of QR codes needed
        int num_chunks = size / sizeof(qr.chunk.info.file_data);
        if (size % sizeof(qr.chunk.info.file_data)) num_chunks++;
        printf("num chunks: %i\n", num_chunks);

        // create the chunks to send to libqrencode
        size_t bytes_remaining = size;
        while(bytes_remaining)
        {
            result = make_next_chunk(fp, &qr);
            if (result != 0) exit(result);
            
            qr.chunk.info.sequence_number++;
            bytes_remaining -= qr.used;
        }
    }
    if (fclose(fp) != 0) exit(EXIT_FAILURE_fclose_failed);

    return EXIT_SUCCESS;
}
