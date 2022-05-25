#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Data structure to help in parsing arguments.
 * Contains all the possible command line options in one
 * group.
 * char* fname (required): filename to split
 * char* outdir (optional): name of output directory to write resultant files
 * long size: size of each individual splitfile
 * long files: number of total splitfiles
 * 
 * Only one of SIZE and FILES is allowed, if both are present that is an 
 * argument validation error.
 * 
 * OUTDIR is not required to be empty.
 * 
 * FNAME must be one of the valid, supported file types. 
 */
typedef struct argument_t {
    char* fname;
    char* outdir;
    long size;
    long files;
} args_t;

void print_args(const args_t* args);
bool parse_args(const int argc, char** argv, args_t* args);
bool validate_args(const args_t* const args);