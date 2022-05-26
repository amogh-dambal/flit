#ifndef SPLIT_H
#define SPLIT_H
#pragma once


#define OUT_DIR_SIZE 8

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "input.h"

typedef struct file_splitter {
    char* out;
    FILE* in;
    FILE** fpa;
    char type; // whether we're splitting into k files or k-sized files
    long nfiles; // number of files 
    size_t buf; // size of the buffer (size of each file)
} split_t;

bool init_splitter(const args_t* const args, split_t* s);
bool split_file(split_t* s);
bool destroy_splitter(split_t* s);

#endif
