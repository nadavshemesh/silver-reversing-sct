#ifndef FILE_SCT_INCL
#define FILE_SCT_INCL

#include<stdio.h>
#include<stdlib.h>
#include<regex.h>

#include "fileutils.h"
#include "gamefunc.h"
#include "object.h"

#define MAX_VAR_NAME 20;
#define MAX_TOKENS_PER_EXPRESSION 50;

typedef struct script {
    int number;
    int size_in_words;
    int code_nodes_num;
    char** name;
    void** script_code_ptr;
    node** code_nodes;
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
    int code_objs_size;
    int data_objs_size;
    FILE* file;
    sct_s* structure;
    script** scripts;
    data_obj** data_section;
} sct_f;


bool is_filepath_valid(char* filepath);
sct_s* form_structure(FILE* sctfile);
int get_script_off(int script_num, sct_f* sf);
void print_sct_struct(sct_f* sf);
void print_script(script* script);
void print_data_section(sct_f* sf);
data_obj* inefficient_search_data_id(int id, sct_f* sf);
data_obj* get_data_obj_by_id(int id, sct_f* sf);


#endif /* !FILE_SCT_INCL */