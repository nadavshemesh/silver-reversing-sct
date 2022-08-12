#ifndef FILE_ASM_F
#define FILE_ASM_F

#include<stdio.h>
#include<stdbool.h>

#include "sct\sct.h"


void write_asm_expr(expression* expr, sct_f* sf);
void write_asm_expression(node** expression_nodes, c_type t, bool nested, sct_f* sf);
void write_asm_code_obj(code_obj* co, int indentation_lvl, sct_f* sf);
void write_asm_code_objs(code_obj* co, int code_obj_num, sct_f* sf);
void write_asm_code_nodes(node* head, sct_f* sf);
void write_asm_script(script* script, sct_f* sf);
void write_asm_file(sct_f* sf);
void write_asm_data_object(data_obj* data_o, sct_f* sf);
void write_asm_data_section(sct_f* sf);
void mark_auto_gen_file(sct_f* sf);

#endif
