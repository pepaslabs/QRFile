#include <stdio.h> // FILE*
#include "md5_hash_t.h"

/* md5_rhash()
 * - fp should be a FILE* which has already been fopen()'ed
 * - returns 0 on success
 */
int md5_rhash(FILE *fp, md5_hash_t *md5);
