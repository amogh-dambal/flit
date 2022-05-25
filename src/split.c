#include "../inc/split.h"
/**
 * @brief Returns the size of the file 
 * pointed to by FP. On error, returns -1.
 * 
 * @param fp : pointer to the file 
 * @return long : size of the file in bytes
 */

long fsize(FILE* fp) {
    long sz = -1;
    if (!fseek(fp, 0L, SEEK_END)) {
        sz = ftell(fp);
        rewind(fp);
    }
    return sz;
}


bool init_splitter(const args_t* const args, split_t* s) {
    s->fpa = NULL;
    s->in = fopen(args->fname, "r");
    if (!s->in) {
        fprintf(stderr, "Error in opening provided file.\n");
        return false;
    }

    // either we split on the size of the output file
    // or the number of output files.
    s->type = (args->size != -1) ? 's' : 'n';
    long val = (args->size != -1) ? args->size : args->files;

    size_t fsz = fsize(s->in);
    if (fsz <= 0) {
        fprintf(stderr, "File size computation error.\n");
        fclose(s->in);
        return false;
    }

    if (s->type == 's') {
        s->buf = val;
        // if the size of each subfile is not evenly divisible
        // by total size, we need one more file for the remaining
        // bytes. we coerce the mod operation into boolean so 
        // if there is any remainder, we add 1
        s->nfiles = (fsz / s->buf) + !!(fsz % s->buf);
    }
    else {
        s->nfiles = val;
        // add modulus so we can avoid data loss by fitting
        // all bytes into all available files
        s->buf = (fsz / s->nfiles) + (fsz % s->nfiles);
    }
    return true;
}

bool split_file(split_t* s);
bool destroy_splitter(split_t* s);
bool split_file_by_bytes(const args_t* const args, split_t* s); 
bool split_file_by_num(const args_t* const args, split_t* s);