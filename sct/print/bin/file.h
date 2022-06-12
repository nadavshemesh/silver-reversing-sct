
#ifndef FILE_BIN_F
#define FILE_BIN_F

#include<stdio.h>
#include<stdbool.h>

#include "sct\sct.h"


void write_sct_header(sct_f* sf);
void write_bin_code_obj(code_obj* co, sct_f* sf);
void write_bin_expression(expression* exp, sct_f* sf);
void write_bin_expr_obj(expr_obj* expr, sct_f* sf);
void write_bin_switch_code_obj(code_obj* co, sct_f* sf);
void write_bin_code_section(sct_f* sf);
void write_bin_data_section(sct_f* sf);
void write_bin_script_table(sct_f* sf);
void write_bin_link_table(sct_f* sf);

#endif