#include "hexify.h"

int hexify(uint8_t *in, size_t in_size, char *out, size_t out_size)
{
    // originally inspired by http://stackoverflow.com/a/12839870/558735
 
    if (in_size == 0 || out_size == 0) return 0;

    char map[16+1] = "0123456789abcdef";

    int bytes_written = 0;
    size_t i = 0;
    while(i < in_size && (i*2 + (2+1)) <= out_size)
    {
        uint8_t high_nibble = (in[i] & 0xF0) >> 4;
        *out = map[high_nibble];
        out++;

        uint8_t low_nibble = in[i] & 0x0F;
        *out = map[low_nibble];
        out++;

        bytes_written += 2;
        i++;
    }
    *out = '\0';

    return bytes_written;
}
