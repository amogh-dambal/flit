#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "input.h"

typedef struct file_splitter {
    FILE* in;
    FILE** fpa;
    char type;
    long val;
    size_t buf;
} split_t;

bool init_splitter(const args_t* const args, split_t* s);
bool split_file(split_t* s);
bool destroy_splitter(split_t* s);
bool split_file_by_bytes(const args_t* const args, split_t* s); 
bool split_file_by_num(const args_t* const args, split_t* s);