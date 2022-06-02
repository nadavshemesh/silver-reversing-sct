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

sct_s* form_structure(FILE* sctfile);
void print_sct_struct(sct_f* sf);
void print_asm_script(script* script);
void print_asm_code_obj(code_obj* co, int indentation_lvl);
void print_asm_code_objs(code_obj* co, int code_obj_num);
int get_script_off(int script_num, sct_f* sf);
void print_tokens(int* bin_tokens, int length);
sct_f* disasm_file(char* filepath);
script* disasm_script(int script_offset, sct_f* sf);
bool is_filepath_valid(char* filepath);
void build_data_from_link_table(sct_f* sct);
data_obj* get_data_obj_by_id(int id, sct_f* sf);
cp_cmp_result identify_cp(void** token_pos_ptr, mode m);
expr_cmp_result identify_expr(void** token_pos_ptr, mode m);
expr_obj* create_expr_obj(expr_pattern* expr, void* vars, mode m, void** token_pos_ptr, sct_f* sf);
code_obj* read_code_block(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf);
code_obj* read_function_call(code_pattern* cp, void* vars, mode m, void** tokens_pos_ptr, sct_f* sf);
obj_and_token_ptr create_code_obj(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf);
void print_asm_expression(node** expression_nodes, c_type t, bool nested);
