#include <stdint.h>
#include <stdlib.h> // EXIT_SUCCESS, etc

#include "hexify/hexify.h"
#include "rhash_md5/md5.h"

#include "fmd5.h"
#include "errors.h"

/* fmd5(): calculate the MD5 hash of a file
 * - fp should be a FILE* which has already been fopen()'ed
 * - returns 0 on success
 */
int fmd5(FILE *fp, md5_hash_t *out_md5)
{
    md5_ctx context;
    {
        rhash_md5_init(&context);

        unsigned char buffer[MD5_FREAD_SIZE];
        {
            while(!feof(fp))
            {
                size_t bytes_read = fread(&buffer, sizeof(char), sizeof(buffer), fp); 
                if (ferror(fp)) return EXIT_FAILURE_fread_failed;

                if (bytes_read > 0)
                {
                    rhash_md5_update(&context, (const unsigned char *)&buffer, bytes_read);
                }
            }
        }

        rhash_md5_final(&context, out_md5->bytes);

        hexify(out_md5->bytes, sizeof(out_md5->bytes), out_md5->hex, sizeof(out_md5->hex));
    }

    return 0;
}
