#include <sys/stat.h> // mkdir()
#include <time.h>


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

/**
 * @brief Generate a random N-digit hexadecimal string and 
 * store it in the provided buffer BUF. Note that BUF must
 * have space for n + 1 bytes to include the null terminating
 * byte. 
 * 
 * @param buf : output buffer
 * @param n : number of desired characters in output string.
 */
void gen_hex_str(char* buf, size_t n) {
    srand(time(NULL));
    
    const char* const hex_digits = "0123456789ABCDEF";
    int i;
    for (i = 0; i < n; i++) {
        buf[i] = hex_digits[rand() % 16];
    }
    buf[i++] = '\0';
}

/**
 * @brief Generate a temporary filename for the Ith output files
 * being split by the splitter S and store it in the file buffer FBUF. 
 * Note that the file buffer must have space for the terminating null-byte. 
 * 
 * @param fbuf : file buffer. 
 * @param i : index of temp file
 * @param s : split_t object which handles all splitting logic 
 * 
 * @return true: if file name generation was successful, false otherwise.
 * will only be false on an snprintf() failure, so this should usually
 * work.
 */
bool gen_tmp_fname(char* fbuf, int i, const split_t* const s) {
    int w = snprintf(fbuf, 100, "%s/part-%d.spf", s->out, i);
    return w > 0 && w < 100;
}

bool init_splitter(const args_t* const args, split_t* s) {
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

    s->fpa = malloc(s->nfiles * sizeof(FILE*));
    if (!s->fpa) {
        fprintf(stderr, "Malloc of file pointer array failed.\n");
        fclose(s->in);
        return false;
    }
    for (int i = 0; i < s->nfiles; i++) {
        s->fpa[i] = NULL;
    }

    if (args->outdir) {
        size_t n = strlen(args->outdir);
        s->out = malloc((n + 1) * sizeof(char));
        size_t cpd = strlcpy(s->out, args->outdir, (n + 1));
        if (cpd != n) {
            fprintf(stderr, "Error in strlcpy when configuring write directory.");
            free(s->out);
            free(s->fpa);
            fclose(s->in);
            return false;
        }
        
    }
    else {
        s->out = malloc(OUT_DIR_SIZE + 1 * sizeof(char));
        gen_hex_str(s->out, OUT_DIR_SIZE);
    }

    if (mkdir(s->out, 0777)) {
        fprintf(stderr, "Unable to open write directory.\n");
        free(s->fpa);
        free(s->out);
        fclose(s->in);
        return false;
    }
    return true;
}

bool split_file(split_t* s) {
    char fnbuf[100];
    char data[s->buf];
    int i = 0;

    errno = 0;
    while (i < s->nfiles && !feof(s->in)) {
        size_t br = fread(data, s->buf, 1, s->in);
        if (ferror(s->in) || br != 1) {
            fprintf(stderr, "File read error.\n");
            destroy_splitter(s);
            return false;
        }

        if (!gen_tmp_fname(fnbuf, i, s) || ferror(s->in)) {
            fprintf(stderr, "File write configuration error.\n");
            destroy_splitter(s);
            return false;
        }
        s->fpa[i] = fopen(fnbuf, "a");
        if (!s->fpa[i]) {
            fprintf(stderr, "Unable to open temporary file %s\n.", fnbuf);
            fprintf(stderr, "Errno: %d\n", errno);
            destroy_splitter(s);
            return false;
        }

        size_t bw = fwrite(data, s->buf, 1, s->fpa[i]);
        if (ferror(s->in) || bw != 1) {
            fprintf(stderr, "File write error.\n");
            destroy_splitter(s);
            return false;
        }

        if (fclose(s->fpa[i])) {
            fprintf(stderr, "File close error.\n");
            destroy_splitter(s);
            return false;
        }
        // don't know if we have to clear the buffers, 
        // but i'm just being cautious especially w/ the
        // snprintf() call. if it affects performance 
        // and/or is unnecessary, i'll take it out
        memset(fnbuf, 0, 100);
        memset(data, 0, s->buf);
    }
}
bool destroy_splitter(split_t* s) {
    fclose(s->in);
    free(s->out);

    for (int i = 0; i < s->nfiles && s->fpa[i]; i++) {
        fclose(s->fpa[i]);
    }
    free(s->fpa);
}
