#include<stdio.h>
#include<stdbool.h>

#include "sct\sct.h"
#include "sct\print\asm\console.h"
#include "sct\print\bin\console.h"

#define MAX_ASM_TOKEN_LEN 256


void build_data_section(sct_f* sf);
cp_cmp_result asm_identify_cp(char*** token_pos_ptr);
obj_and_token_ptr asm_create_code_obj(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf);
void print_asm_tokens(char** tokens, int tokens_len);
int count_next_section_tokens(sct_f* sf);
int count_expr_bin_tokens(expr_obj* expr);
int count_expression_bin_tokens(expression* exp);
int count_code_obj_bin_tokens(code_obj* co);
code_obj* asm_read_code_block(int tokens_to_read, char*** token_pos_ptr, sct_f* sf);
int count_file_sections(sct_f* sf);
void build_scripts_lables_and_order(sct_f* sf);