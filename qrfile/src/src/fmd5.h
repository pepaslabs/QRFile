#ifndef FMD5_H
#define FMD5_H

#include <stdio.h> // FILE*

#include "md5_hash_t.h"

/* fmd5(): calculate the MD5 hash of a file
 * - fp should be a FILE* which has already been fopen()'ed
 * - returns 0 on success
 */
int fmd5(FILE *fp, md5_hash_t *out_md5);

#endif // FMD5_H
