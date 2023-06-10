#include "sct\print\asm\file.h"


void f_indent(int indentation_lvl, sct_f* sf) {
    for(int i=0; i < indentation_lvl; i++) {
        fprintf(sf->out_file, "\t");
    }
}

bool f_is_cp_paranth_type1(c_type type) {
    if(type == FUNCTION_CALL || type == IF_STATEMENT || type == SWITCH) {
        return true;
    }
    return false;
}

bool f_is_ep_paranth_type1(expr_type type) {
    if(type == FUNCTION) {
        return true;
    }
    return false;
}

bool f_is_cp_paranth_type2(c_type type) {
    if(type == IF_STATEMENT || type == ELSE_STATEMENT || type == SWITCH) {
        return true;
    }
    return false;
}

bool f_is_same_line(code_pattern* cp) {
    if(cp->type == ROOM_VAR_PTR || cp->type == CP_VAR_PTR 
        || cp->type == FUNCTION_CALL) {
        return true;
    }
    return false;
}

bool should_f_indent(code_pattern* cp) {
    if(cp->type == VAR_DEC || cp->type == VAR_INC || cp->type == ASSIGNMENT)
        return false;
    return true;
}

void write_asm_expr(expression* expr, sct_f* sf) {
    int exprs_num = expr->expr_objs_len;
    for(int i=0, j=0; i < exprs_num; i++) {
        expr_obj* expr_o = &(expr->expr_objs)[i];
        expr_pattern* expr_p = expr_o->expr_p;

        switch(expr_p->type) {
            default: {
                for(int k=0, l=0; k < expr_p->asm_token_num; k++) {
                    if(is_var_pos_expr(expr_p, MODE_ASM, k)) {
                        fprintf(sf->out_file, "%s", expr_o->asm_vars[l]);
                        l++;
                    } else {
                        if(expr_p->type == OPERATOR) fprintf(sf->out_file, " ");
                        fprintf(sf->out_file, "%s", expr_p->asm_tokens[k]);
                        if(expr_p->type == OPERATOR) fprintf(sf->out_file, " ");
                    }
                    if(k < expr_p->asm_token_num-1 
                        && expr_p->type != DATA_INDEX_PTR) {
                            if(!(expr_p->type == ADDROF_VAR_PTR && k == 1))
                                fprintf(sf->out_file, " "); 
                        }
                }
                if(expr_o->hint_comment != NULL) {
                    fprintf(sf->out_file, expr_o->hint_comment);
                }
                if(expr_o->expression_node_num > 0) {
                    c_type type = FUNCTION_CALL; 
                    if(expr_p->type == DATA_INDEX_PTR) { fprintf(sf->out_file, "["); type = CP_VAR_PTR; }
                    write_asm_expression(expr_o->expression_nodes, type, true, sf);
                    if(expr_p->type == DATA_INDEX_PTR) { fprintf(sf->out_file, "]"); }
                } else {
                    if(expr_o->expr_p->type == FUNCTION) { 
                        fprintf(sf->out_file, "()");
                    }
                }
            }
        }
    }
}

void write_asm_expression(node** expression_nodes, c_type t, bool nested, sct_f* sf) {
    node* exp_node = *expression_nodes; 
    c_type type = t;
    if(f_is_cp_paranth_type1(type)) {
        fprintf(sf->out_file, "(");
    }
    while(exp_node != NULL && exp_node->item != NULL) {
        expression* obj = exp_node->item;
        write_asm_expr(obj, sf);
        exp_node = exp_node->next;
        expr_obj* last = &(obj->expr_objs[obj->expr_objs_len-1]);
        if(exp_node != NULL && last->expr_p->type != MUL_EXP_OP) fprintf(sf->out_file, ", ");
    }
    if(f_is_cp_paranth_type1(type)) {
        fprintf(sf->out_file, ")");
    }
    if(!nested) fprintf(sf->out_file, "\n");
}

bool f_is_control_statement(code_pattern* cp) {
    c_type type = cp->type;
    if(type == IF_STATEMENT || type == SWITCH || type == ELSE_STATEMENT) {
        return true;
    }
    return false;
}

bool f_is_var_type(code_pattern* cp) {
    c_type type = cp->type;
    return (type == CP_VAR_PTR || type == ROOM_VAR_PTR);
}

void write_asm_code_obj(code_obj* co, int indentation_lvl, sct_f* sf) {
    code_pattern* cp = co->cp;

    if(co->hint_comment != NULL) {
        fprintf(sf->out_file, "\n");
        if(should_f_indent(cp)) f_indent(indentation_lvl, sf);
        fprintf(sf->out_file, co->hint_comment);
        fprintf(sf->out_file, "\n");
    }
    // write code structure
    for(int i=0, j=0; i < cp->asm_token_num; i++) {
        if(is_var_pos(cp, CODE_TYPE, MODE_ASM, i)) {
            fprintf(sf->out_file, "%s", co->asm_vars[j]);
            if(f_is_var_type(cp)) fprintf(sf->out_file, " ");
            j++;
        } else {
            if(should_f_indent(cp)) f_indent(indentation_lvl, sf);
            fprintf(sf->out_file, "%s", cp->asm_tokens[i]);
            if(i < cp->asm_token_num && !f_is_control_statement(cp) 
                && cp->type != CP_DATA_INDEX_PTR) fprintf(sf->out_file, " ");
        }
        if(i == cp->asm_token_num-1 && co->expression_node_num == 0
         && !f_is_same_line(cp)) { fprintf(sf->out_file, "\n"); }
    }
    
    // write code exprs
    int exp_num = co->expression_node_num;
    bool nested = false;
    if(exp_num > 0) {
        if(cp->type == CP_DATA_INDEX_PTR) { fprintf(sf->out_file, "["); nested = true; } 
        write_asm_expression(co->expression_nodes, co->cp->type, nested, sf);
        if(cp->type == CP_DATA_INDEX_PTR) { fprintf(sf->out_file, "]"); nested = false; }
    } else {
        if(co->cp->type == FUNCTION_CALL) { 
            fprintf(sf->out_file, "()\n");
        }
    }

    // write nested blocks
    int code_nodes_num = co->code_nodes_num;
    if(code_nodes_num > 0) {
        indentation_lvl++;
        c_type type = co->cp->type;
        if(f_is_cp_paranth_type2(type)) { 
            if(indentation_lvl > 0) f_indent(indentation_lvl-1, sf);
            fprintf(sf->out_file, " {\n");
        }
        node* code_n = *co->code_nodes;
        while(code_n != NULL && code_n->item != NULL) {
            write_asm_code_obj(code_n->item, indentation_lvl, sf);
            code_n = code_n->next;
        }
        if(f_is_cp_paranth_type2(type)) { 
            if(indentation_lvl > 0) f_indent(indentation_lvl-1, sf);
            fprintf(sf->out_file, "}\n");
        }
    }
}

void write_asm_code_objs(code_obj* co, int code_obj_num, sct_f* sf) {
    for(int i=0; i < code_obj_num; i++) {
        write_asm_code_obj(&(co)[i], 0, sf);
    }
}

void write_asm_code_nodes(node* head, sct_f* sf) {
    node* node = head;
    if(node == NULL) return;
    while(node != NULL && node->item != NULL) {
        code_obj* co = (code_obj*) node->item;
        write_asm_code_obj(co, 1, sf);
        node = node->next;
    }
}

void write_asm_script(script* script, sct_f* sf) {
    char script_name[256];
    int script_number = script->number;

    if(script_number > 2) {
        sprintf(script_name, "\n._SCRIPT_%d\n", script_number);
    } else {
        switch(script_number) {
            case 0:
                sprintf(script_name, "\n._on_load\n", script_number);
                break;
            case 1:
                sprintf(script_name, "\n._infinite_loop\n", script_number);
                break;
            case 2:
                sprintf(script_name, "\n._on_exit\n", script_number);
                break;
        }
    }
    fprintf(sf->out_file, script_name);
    write_asm_code_nodes(*script->code_nodes, sf);
}

void write_asm_file(sct_f* sf) {
    write_asm_data_section(sf);
    for(int i=0; i < sf->structure->num_of_scripts; i++) {
        write_asm_script(sf->scripts[i], sf);
    }
}

void write_asm_data_object(data_obj* data_o, sct_f* sf) {
    fprintf(sf->out_file, "\t%s\t", data_o->name);
    if(data_o->type == STRING || (data_o->type == UNDEFINED_TYPE && is_string((byte*)data_o->data, data_o->byte_size))) {    
        fprintf(sf->out_file, "\"");
        for(int i=0; i < data_o->byte_size; i++) {
            char b = *((char*)(data_o->data+i)); 
            if(b == '\"') b = '\'';
            if(isprint(b) && b != '@') {
                fprintf(sf->out_file, "%c", b);
            }
        }
        fprintf(sf->out_file, "\"");
    } else {
        int size_in_words = data_o->byte_size / 4;
        if(size_in_words > 1) fprintf(sf->out_file, "{ ");
        for(int i=0; i < size_in_words; i++) {
            // int num = *data_o->bin_data + i;
            int num = *((int*)(data_o->data + i*sizeof(int)));
            if((num & 0xFF000000) == 0xC0000000) {
                int id = num & 0x00FFFFFF;
                data_obj* dao = get_data_obj_by_id(id, sf);
                fprintf(sf->out_file, "%s", dao->name);
            } else {
                fprintf(sf->out_file, "%d", num);
            }
            if(i < size_in_words-1) fprintf(sf->out_file, ", ");
        }
        if(size_in_words > 1) fprintf(sf->out_file, " }");
    }
}

void write_asm_data_section(sct_f* sf) {
    fprintf(sf->out_file, "._DATA\n");
    int size = sf->data_objs_num;
    data_obj** section = sf->data_section;
    for(int i=0; i < size; i++) {
        data_obj* data_o = section[i];

        // ignore any default zero val code ref
        if(data_o->byte_size == 4 && *((int*)data_o->data) == 0 && data_o->data_references == 0) {
            data_o->ignore = true;
        }

        if(!data_o->ignore) {
            write_asm_data_object(data_o, sf);
            if(i < size-1) fprintf(sf->out_file, "\t");
            if(i%1 == 0) fprintf(sf->out_file, "\n");
        }
    }
}

void mark_auto_gen_file(sct_f* sf) {
    fseek(sf->out_file, 0, SEEK_SET);
    fprintf(sf->out_file, 
    "/*\n* This file was auto-generated using 'silver-reversing-sct' tool\n"
    "* to better understand the scripts presented, one is encouraged to\n"
    "* view the catalogs at: \n"
    "* https://github.com/nadavshemesh/silver-reversing-sct/blob/master/docs/index.md\n*\n"
    "* Any further information and support of this project please visit:\n"
    "* https://github.com/nadavshemesh/silver-reversing-sct \n*/\n\n\n"
    );
}