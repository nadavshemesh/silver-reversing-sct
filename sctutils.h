#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include "sct.h"
#include "sctasm.h"


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
