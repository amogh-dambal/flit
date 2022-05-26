#include <stdio.h>
#include <stdlib.h>
#include "../inc/input.h"
#include <sys/stat.h>
#include <string.h>

int main(int argc, char** argv) {

    const char* src = "test";
    char dest[100];

    size_t n = strlcpy(dest, src, 5);
    
    printf("arg: %d\n", 5);
    printf("Copied %u bytes into %s from %s\n", n, dest, src);
    return 0;
}