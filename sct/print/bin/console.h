#ifndef FILE_BIN_CONS
#define FILE_BIN_CONS

#include<stdio.h>
#include<stdbool.h>

#include "sct\sct.h"


void print_bin_code_obj(code_obj* co);
void print_bin_expression(expression* exp);
void print_bin_expr_obj(expr_obj* expr);
void print_bin_switch_code_obj(code_obj* co);
void print_bin_file(sct_f* sf);
void print_bin_data_section(sct_f* sf);

#endif