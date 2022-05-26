#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <argp.h>

#include "../inc/input.h"
#include "../inc/split.h"

extern int errno;



int main(int argc, char** argv) {
    args_t args;
    if (!parse_args(argc, argv, &args)) {
        fprintf(stderr, "Unable to parse arguments.\n");
        return EXIT_FAILURE;
    }
    if (!validate_args(&args)) {
        fprintf(stderr, "Invalid arguments.\n");
        return EXIT_FAILURE;
    }

    split_t s;
    if (!init_splitter(&args, &s)) {
        fprintf(stderr, "Error in initializing file splitter.\n");
        return EXIT_FAILURE;
    }

    if (!split_file(&s)) {
        fprintf(stderr, "Error in splitting the file.\n");
        return EXIT_FAILURE;
    }

    destroy_splitter(&s);
    return 0;
}
