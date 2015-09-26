#include <stdlib.h> // EXIT_SUCCESS, etc
#include <stdio.h> // fopen(), etc
#include <stdint.h>
#include <rhash.h>
#include "hexify.h"

struct _md5_hash_t
{
    uint8_t bytes[16];
    char hex[32+1];
};

typedef struct _md5_hash_t md5_hash_t;

enum
{
    EXIT_FAILURE_fopen_failed = 10,
    EXIT_FAILURE_fread_failed,
    EXIT_FAILURE_fclose_failed,
    EXIT_FAILURE_rhash_init_failed = 20,
    EXIT_FAILURE_rhash_print_bad_length,
    EXIT_FAILURE_rhash_print_bad_length2
}; 

#define MD5_FREAD_SIZE 1024

int main(int argc, char *argv[])
{
    rhash_library_init(); /* initialize static data */

    md5_hash_t checksum;

    FILE *fp;
    {
        fp = fopen("file", "r");
        if (fp == NULL) exit(EXIT_FAILURE_fopen_failed);
        
        rhash context;
        {
            context = rhash_init(RHASH_MD5);
            if (!context) exit(EXIT_FAILURE_rhash_init_failed);
    
            char buffer[MD5_FREAD_SIZE];
            {
                while(!feof(fp))
                {
                    size_t bytes_read = fread(&buffer, sizeof(char), sizeof(buffer), fp); 
                    if (ferror(fp)) exit(EXIT_FAILURE_fread_failed);

                    if (bytes_read > 0)
                    {
                        rhash_update(context, &buffer, bytes_read);
                    }
                }
            }
            
            int result = rhash_final(context, NULL); /* finalize hash calculation */

            size_t bytes_printed;
            {
                bytes_printed = rhash_print(checksum.bytes, context, RHASH_MD5, RHPR_RAW); 
                if (bytes_printed != sizeof(checksum.bytes)) exit(EXIT_FAILURE_rhash_print_bad_length);

                bytes_printed = rhash_print(checksum.hex, context, RHASH_MD5, RHPR_HEX); 
                if (bytes_printed != sizeof(checksum.hex)-1) exit(EXIT_FAILURE_rhash_print_bad_length2);
            }
        }
        rhash_free(context);
    }
    if (fclose(fp) != 0) exit(EXIT_FAILURE_fclose_failed);

    char hex_buffer[128+1];
    hexify(checksum.bytes, sizeof(checksum.bytes), hex_buffer, sizeof(hex_buffer));
    printf("bytes: %s\n", hex_buffer);

    printf("hex: %s\n", checksum.hex);

    return EXIT_SUCCESS;
}

