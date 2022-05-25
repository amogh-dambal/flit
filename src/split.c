#include "../inc/split.h"


bool init_splitter(const args_t* const args, split_t* s) {
    s->inf = args->fname;
}


bool split_file(split_t* s);
bool destroy_splitter(split_t* s);
bool split_file_by_bytes(const args_t* const args, split_t* s); 
bool split_file_by_num(const args_t* const args, split_t* s);