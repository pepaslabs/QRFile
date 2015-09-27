#include <stdint.h>

#define MD5_SIZE 16
#define MD5_HEX_SIZE (MD5_SIZE*2)
#define MD5_HEX_BUFF_SIZE (MD5_HEX_SIZE+1)

struct _md5_hash_t
{
    uint8_t bytes[MD5_SIZE];
    char hex[MD5_HEX_BUFF_SIZE];
};

typedef struct _md5_hash_t md5_hash_t;

#define MD5_FREAD_SIZE 4096
