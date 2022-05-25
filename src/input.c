#include "../inc/input.h"

/**
 * @brief Helper function to print the args_t data structure. Helpful for 
 * debugging, not used in production code. 
 * 
 * @param args Pointer to the args_t struct. 
 */
void print_argst(const args_t* const args) {
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
    
    // initialize args because arguments are optional
    args->size = -1;
    args->files = -1;
    args->outdir = NULL;
    args->fname = NULL;
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

bool validate_filename(const char* filename) {
    // TODO: implement actual filename validation logic
    // when we figure out exactly what kinds of files we 
    // can and cannot support
    return true;
}

bool validate_size(const long size) {
    // TODO: add more file size validation rules as we figure out split limits
    return size != 0;
}

bool validate_files(const long files) {
    // TODO: add more file number validation rules as we figure out 
    // split limits. May want to cap the number of small files for 
    // performance's sake. Definitely want to have 1B file size minimum.
    return files != 0;
}


bool validate_args(const args_t* const args) {
    // essentially a logical XOR, since either MUST be true. if both
    // or neither are true then we have a problem
    bool filesOrSize = (args->size == -1) != (args->files == -1);
    return 
        validate_filename(args->fname) &&
        validate_size(args->size) &&
        validate_files(args->files) &&
        filesOrSize;
}