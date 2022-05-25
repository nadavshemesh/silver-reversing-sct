#include<stdio.h>
#include<stdlib.h>

#include "fileutils.h"
#include "gamefunc.h"
#include "object.h"

const int MAX_VAR_NAME = 20;
const int MAX_TOKENS_PER_EXPRESSION = 50;

typedef enum mode { MODE_BIN, MODE_ASM } mode;

// typedef struct code_block {
//     int size_in_words;
//     code_obj* code_objs;
// } code_block;

typedef struct script {
    int size_in_words;
    void** script_code_ptr;
    int code_obj_num;
    code_obj* code_objs;
    // code_block* code_blocks;
} script;

typedef struct sct_struct {
    int num_of_scripts;
    int main_script_off;
    int script_table_off;
    int script_table_size;
    int data_sec_off;
    int data_sec_size;
    int link_table_off;
    int link_table_size;
} sct_s;

typedef struct sct_file {
    FILE* file;
    sct_s* structure;
    int code_objs_size;
    script* scripts;
    int data_objs_size;
    data_obj* data_section;
} sct_f;

sct_s* form_structure(FILE* sctfile);
void print_sct_struct(sct_f* sf);
int get_script_off(int script_num, sct_f* sf);
void print_tokens(int* bin_tokens, int length);
int disasm_file(char* filepath);
int disasm_script(int script_offset, sct_f* sf);
bool is_filepath_valid(char* filepath);
void build_data_from_link_table(sct_f* sct);
data_obj* get_data_obj_by_id(int id, sct_f* sf);
