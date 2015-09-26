#include <stdint.h>

struct _md5_hash_t
{
    uint8_t bytes[16];
    char hex[32+1];
};

typedef struct _md5_hash_t md5_hash_t;

#define MD5_FREAD_SIZE 4096
