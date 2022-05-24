#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "error.h"

extern int errno;

long fsize(FILE* fp) {
    long sz = -1;
    if (!fseek(fp, 0L, SEEK_END)) {
        sz = ftell(fp);
        rewind(fp);
    }
    return sz;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Must provide a filename.\n");
        exit(EXIT_FAILURE);
    }

    char* fname = argv[1];
    FILE* fp = fopen(fname, "r");
    if (!fp) {
        fprintf(stderr, "Unable to open file. Errno: %d\n Error: %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("successfully opened file with name %s\n", fname);

    /* get the size of the file */
    long sz = fsize(fp);
    if (sz == -1) {
        fprintf(stderr, "File size operation failed.\n");
        exit(EXIT_FAILURE);
    }
    printf("size of file: %ld\n", sz);

    fclose(fp);
    exit(EXIT_SUCCESS);
}
