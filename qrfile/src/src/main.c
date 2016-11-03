#include <stdint.h>
#include <stdio.h> // fopen(), etc
#include <stdlib.h> // EXIT_SUCCESS, etc
#include <string.h> // memcpy()

#include "hexify/hexify.h"
#include "rhash_md5/md5.h"

#include "errors.h"
#include "fmd5.h"
#include "fsize.h"

#define QR_23L_8BIT_SIZE 1091


// A "chunk" of a file.  Each chunk will be encoded as a QR code.
struct _qrchunk_info_23L_t
{
    // A (16-byte) MD5 hash of the the file which is being broken up into QR codes.
    uint8_t document_id[MD5_SIZE];

    // The index of this particular QR code in the sequence.  (Starts at 0).
    uint16_t sequence_index;

    // The total number of QR codes in the sequence.
    uint16_t total_chunks_count;

    // The file data captured in this chunk.
    uint8_t file_data[QR_23L_8BIT_SIZE - (MD5_SIZE + sizeof(uint16_t)*2)];
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

    // The number of (file) bytes actually stored in this QR code chunk.
    size_t used;
};
typedef struct _qrchunk_descriptor_23L_t qrchunk_descriptor_23L_t;


int make_next_chunk(FILE *fp, qrchunk_descriptor_23L_t *qr)
{
    size_t bytes_read = fread(qr->chunk.info.file_data,
                              sizeof(uint8_t), 
                              sizeof(qr->chunk.info.file_data),
                              fp);
    if (ferror(fp)) return EXIT_FAILURE_fread_failed2;
    if (bytes_read == 0) return EXIT_FAILURE_fread_returned_zero_bytes;
    qr->used = bytes_read;
    printf("read %zu bytes as chunk\n", qr->used);

    // process the chunk with libqrencode
    // FIXME

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) exit(EXIT_FAILURE_usage_no_file_argument_given);

    FILE *fp = NULL;
    {
        int result = 0;

        // open the file
        char *fpath = argv[1];
        fp = fopen(fpath, "r");
        if (fp == NULL) exit(EXIT_FAILURE_fopen_failed);
        
        // read the size of the file
        off_t size;
        result = fsize(fp, &size);
        if (result != 0) exit(result);
        printf("file size: %lli\n", size);

        // generate an md5 hash of the file
        md5_hash_t hash;
        result = fmd5(fp, &hash);
        if (result != 0) exit(result);
        rewind(fp);
        printf("md5: %s\n", hash.hex);

        qrchunk_descriptor_23L_t qr;
        memcpy(qr.chunk.info.document_id, hash.bytes, sizeof(hash.bytes));
        qr.chunk.info.sequence_index = 0;

        // calculate number of QR codes needed
        int num_chunks = size / sizeof(qr.chunk.info.file_data);
        if (size % sizeof(qr.chunk.info.file_data)) num_chunks++;
        printf("num chunks needed: %i\n", num_chunks);

        // create the chunks to send to libqrencode
        size_t bytes_remaining = size;
        while(bytes_remaining)
        {
            result = make_next_chunk(fp, &qr);
            if (result != 0) exit(result);
            
            qr.chunk.info.sequence_index++;
            bytes_remaining -= qr.used;
        }
    }
    if (fclose(fp) != 0) exit(EXIT_FAILURE_fclose_failed);

    return EXIT_SUCCESS;
}
