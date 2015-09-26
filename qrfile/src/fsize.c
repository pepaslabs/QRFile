#include "fsize.h"
#include "errors.h"
#include <sys/stat.h> // struct stat

int fsize(FILE *fp, off_t *size)
{
    struct stat stat_buff;

    int fd = fileno(fp);
    if (fd == -1) return EXIT_FAILURE_fileno_failed;

    int result = fstat(fd, &stat_buff); 
    if (result != 0) return EXIT_FAILURE_fstat_failed;

    *size = stat_buff.st_size;
    return 0;
}
