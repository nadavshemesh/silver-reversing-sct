#include<stdio.h>
#include<stdbool.h>

#include "sct\sct.h"
#include "sct\print\asm\console.h"
#include "sct\print\asm\file.h"

void build_data_from_link_table(sct_f* sct);
expr_obj* bin_create_expr_obj(expr_pattern* expr, int* vars, void** token_pos_ptr, sct_f* sf);
obj_and_token_ptr bin_create_code_obj(code_pattern* cp, int* vars, void** token_pos_ptr, sct_f* sf);
cp_cmp_result bin_identify_cp(void** token_pos_ptr);
void build_scripts_order(sct_f* sf);