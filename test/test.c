#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct argument_t {
    char* fname;
    char* outdir;
    long size;
    long files;
} args_t;

void print_argst(const args_t* args) {
    printf("=======\n");
    printf("fname: %s\n", args->fname);
    printf("outdir: %s\n", args->outdir);
    printf("size: %ld\n", args->size);
    printf("files: %ld\n", args->files);
    printf("=======\n");
}

/**
 * @brief Helper functions to parse command line arguments and options
 * 
 * @param argc Value of argc from main.
 * @param argv Value of argv from main
 * @param args pointer to a args_t structure - program specific data structure
 * to parse arguments, interfaces with getopt()
 * @return true  if arguments were able to be successfully parsed without 
 * error.  
 * @return false if there was an error in parsing arguments such as, but not
 * limited to:
 * - invalid number of arguments
 * - invalid options
 * - errors from stdlib syscalls such as malloc, strlcpy,
 */
bool parse_args(const int argc, char** argv, args_t* args) {
    if (argc <= 2) {
        fprintf(stderr, "Invalid number of arguments.\n");
        fprintf(stderr, "Usage: %s [-s bytes] [-n files] [-o outdir] filename\n", argv[0]);
        return false;
    }

    extern int optind, opterr;
    extern char* optarg;

    bool parseSuccess = true;
    int c;

    opterr = 0;
    while ((c = getopt(argc, argv, "s:n:o:")) != -1) {
        switch (c) {
            case 's':
                args->size = atol(optarg);
                break;
            case 'n':
                args->files = atol(optarg);
                break;
            case 'o':
                args->outdir = optarg;
                break;
            case '?':
                parseSuccess = false;
                fprintf(stderr, "Invalid argument passed.\n");
                fprintf(stderr, "Usage: %s [-s bytes] [-n files] [-o outdir] filename\n", argv[0]);
                break;
            default:
                fprintf(stderr, "Usage: %s [-s bytes] [-n files] [-o outdir] filename\n", argv[0]);
        }
    }

    if (parseSuccess) {
        char* file = argv[optind++];
        if (optind != argc) {
            fprintf(stderr, "Invalid number of arguments.\n");
            fprintf(stderr, "Usage: %s [-s bytes] [-n files] [-o outdir] filename\n", argv[0]);
            return false;
        }
        args->fname = file;
    }

    return parseSuccess;
} 

int main(int argc, char** argv) {
    args_t args;
    bool success = parse_args(argc, argv, &args);
    if (success) {
        printf("Argument parsing successful.\n");
        print_argst(&args);
    }
    

    return 0;
}
