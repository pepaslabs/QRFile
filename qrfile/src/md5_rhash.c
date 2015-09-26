#include <stdlib.h> // EXIT_SUCCESS, etc
#include <stdint.h>
#include <rhash.h>
#include "hexify.h"
#include "md5_rhash.h"
#include "errors.h"

/* md5_rhash()
 * - fp should be a FILE* which has already been fopen()'ed
 * - returns 0 on success
 */
int md5_rhash(FILE *fp, md5_hash_t *md5)
{
    rhash_library_init(); /* initialize static data */

    rhash context;
    {
        context = rhash_init(RHASH_MD5);
        if (!context) return EXIT_FAILURE_rhash_init_failed;
    
        char buffer[MD5_FREAD_SIZE];
        {
            while(!feof(fp))
            {
                size_t bytes_read = fread(&buffer, sizeof(char), sizeof(buffer), fp); 
                if (ferror(fp)) return EXIT_FAILURE_fread_failed;

                if (bytes_read > 0)
                {
                    rhash_update(context, &buffer, bytes_read);
                }
            }
        }
            
        int result = rhash_final(context, NULL); /* finalize hash calculation */
        // FIXME check this result

        size_t bytes_printed;
        {
            bytes_printed = rhash_print(md5->bytes, context, RHASH_MD5, RHPR_RAW); 
            if (bytes_printed != sizeof(md5->bytes)) return EXIT_FAILURE_rhash_print_bad_length;

            bytes_printed = rhash_print(md5->hex, context, RHASH_MD5, RHPR_HEX); 
            if (bytes_printed != sizeof(md5->hex)-1) return EXIT_FAILURE_rhash_print_bad_length2;
        }
    }
    rhash_free(context);

    return 0;
}
