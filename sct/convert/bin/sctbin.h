#include<stdio.h>
#include<stdbool.h>

#include "sct\sct.h"
#include "sct\print\asm\console.h"

void build_data_from_link_table(sct_f* sct);
expr_obj* bin_create_expr_obj(expr_pattern* expr, void* vars, void** token_pos_ptr, sct_f* sf);
obj_and_token_ptr bin_create_code_obj(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf);