#include<stdio.h>
#include<stdbool.h>

#include "sct.h"


void indent(int indentation_lvl);
bool is_cp_paranth_type1(c_type type);
bool is_ep_paranth_type1(expr_type type);
bool is_cp_paranth_type2(c_type type);
bool is_same_line(code_pattern* cp);
bool should_indent(code_pattern* cp);
void print_asm_expr(expression* expr);
void print_asm_expression(node** expression_nodes, c_type t, bool nested);
void print_asm_code_obj(code_obj* co, int indentation_lvl);
void print_asm_code_objs(code_obj* co, int code_obj_num);
void print_asm_code_nodes(node* head);
void print_asm_script(script* script);
void print_asm_file(sct_f* sf);
void print_asm_data_object(data_obj* data_o);
void print_asm_data_section(sct_f* sf);