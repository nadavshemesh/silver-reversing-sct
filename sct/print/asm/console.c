#include "sct\print\asm\console.h"

void print_bin_tokens(int* tokens, int length) {
    for(int i=0; i<length; i++) {
        printf("%08x\n", tokens[i]);
    }
}

void print_asm_token_seg(char** tokens, int from_row, int to_row, int cols) {
    char* token = *tokens;
    for(int i=from_row*cols; i < to_row*cols; i++) {
        if((tokens+i) != NULL && *(tokens+i) != 0 && abs(*(tokens+i) - token) < 1000) {
            if(i != from_row*cols && i%cols == 0) printf("\n");
            if(tokens[i] == token) { // cmp by addr
                printf(ANSI_COLOR_RED "%20.20s " ANSI_COLOR_RESET, tokens[i]);
            } else {
                printf("%20.20s ", tokens[i]);
            }
        }
    }
    printf("\n");
}

void print_token_area_details(void* token_pos, mode m) {
    if(token_pos != NULL) {
        if(m == MODE_BIN) {
            int* token = (int*) token_pos;
            print_bin_token_seg(token, -2, 2, 5);
        } else {
            char** token_pos_asm = (char**) token_pos;
            print_asm_token_seg(token_pos_asm, -3, 2, 5);
        }
    }
    exit(1);
}

void indent(int indentation_lvl) {
    for(int i=0; i < indentation_lvl; i++) {
        printf("\t");
    }
}

bool is_cp_paranth_type1(c_type type) {
    if(type == FUNCTION_CALL || type == IF_STATEMENT || type == SWITCH) {
        return true;
    }
    return false;
}

bool is_ep_paranth_type1(expr_type type) {
    if(type == FUNCTION) {
        return true;
    }
    return false;
}

bool is_cp_paranth_type2(c_type type) {
    if(type == IF_STATEMENT || type == ELSE_STATEMENT || type == SWITCH) {
        return true;
    }
    return false;
}

bool is_same_line(code_pattern* cp) {
    if(cp->type == GAME_VAR || cp->type == ROOM_VAR_PTR || cp->type == CP_VAR_PTR
         || (cp->type == FUNCTION_CALL))
        return true;
    return false;
}

bool should_indent(code_pattern* cp) {
    if(cp->type == VAR_DEC || cp->type == VAR_INC || cp->type == ASSIGNMENT)
        return false;
    return true;
}

void print_asm_expr(expression* expr) {
    int exprs_num = expr->expr_objs_len;
    for(int i=0, j=0; i < exprs_num; i++) {
        expr_obj* expr_o = &(expr->expr_objs)[i];
        expr_pattern* expr_p = expr_o->expr_p;
        
        for(int k=0, l=0; k < expr_p->asm_token_num; k++) {
            if(is_var_pos_expr(expr_p, MODE_ASM, k)) {
                printf("%s", expr_o->asm_vars[l]);
                l++;
                // if(i+1 < exprs_num) { printf(", "); }
            } else {
                if(expr_p->type == OPERATOR) printf(" ");
                printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, expr_p->asm_tokens[k]);
                if(expr_p->type == OPERATOR) printf(" ");
            }
            if(k < expr_p->asm_token_num-1) { printf(" "); }
        }
        if(expr_o->expression_node_num > 0) {
            print_asm_expression(expr_o->expression_nodes, FUNCTION_CALL, true);
        } else {
            if(expr_o->expr_p->type == FUNCTION) { printf("()"); }
        }
    }
}

void print_asm_expression(node** expression_nodes, c_type t, bool nested) {
    node* exp_node = *expression_nodes; 
    c_type type = t;
    if(is_cp_paranth_type1(type)) {
        printf("(");
    }
    while(exp_node != NULL && exp_node->item != NULL) {
        expression* obj = exp_node->item;
        print_asm_expr(obj);
        exp_node = exp_node->next;
        if(exp_node != NULL) printf(", ");
    }
    if(is_cp_paranth_type1(type)) {
        printf(")");
    }
    if(!nested) printf("\n");
}

bool is_control_statement(code_pattern* cp) {
    c_type type = cp->type;
    if(type == IF_STATEMENT || type == SWITCH || type == ELSE_STATEMENT) {
        return true;
    }
    return false;
}

void print_asm_code_obj(code_obj* co, int indentation_lvl) {
    code_pattern* cp = co->cp;

    // print code structure
    for(int i=0, j=0; i < cp->asm_token_num; i++) {
        if(is_var_pos(cp, CODE_TYPE, MODE_ASM, i)) {
            printf("%s", co->asm_vars[j]);
            if(cp->type == CP_VAR_PTR) printf(" ");
            j++;
        } else {
            if(should_indent(cp)) indent(indentation_lvl);
            printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, cp->asm_tokens[i]);
            if(i < cp->asm_token_num && !is_control_statement(cp)) printf(" ");
        }
        if(i == cp->asm_token_num-1 && co->expression_node_num == 0
         && !is_same_line(cp)) { printf("\n"); }
    }
    
    // print code exprs
    int exp_num = co->expression_node_num;
    if(exp_num > 0) {
        print_asm_expression(co->expression_nodes, co->cp->type, false);
    } else {
        if(co->cp->type == FUNCTION_CALL) { printf("()"); }
    }

    // print nested blocks
    int code_nodes_num = co->code_nodes_num;
    if(code_nodes_num > 0) {
        indentation_lvl++;
        c_type type = co->cp->type;
        if(is_cp_paranth_type2(type)) { 
            if(indentation_lvl > 0) indent(indentation_lvl-1);
            printf(" {\n");
        }
        node* code_n = *co->code_nodes;
        while(code_n != NULL && code_n->item != NULL) {
            print_asm_code_obj(code_n->item, indentation_lvl);
            code_n = code_n->next;
        }
        if(is_cp_paranth_type2(type)) { 
            if(indentation_lvl > 0) indent(indentation_lvl-1);
            printf("}\n");
        }
    }
}

void print_asm_code_objs(code_obj* co, int code_obj_num) {
    print_title("_ASM_");
    for(int i=0; i < code_obj_num; i++) {
        print_asm_code_obj(&(co)[i], 0);
    }
}

void print_asm_code_nodes(node* head) {
    node* node = head;
    if(node == NULL) return;
    while(node != NULL && node->item != NULL) {
        code_obj* co = (code_obj*) node->item;
        print_asm_code_obj(co, 0);
        node = node->next;
    }
}

void print_asm_script(script* script) {
    char script_name[256];
    sprintf(script_name, "._SCRIPT_%d", script->number);
    print_title(script_name);
    print_asm_code_nodes(*script->code_nodes);
}

void print_asm_file(sct_f* sf) {
    for(int i=0; i < sf->structure->num_of_scripts; i++) {
        print_asm_script(sf->scripts[i]);
    }
}

void print_asm_data_object(data_obj* data_o) {
    printf("%s\t", data_o->name);
    
    if(is_string((char*)data_o->data, data_o->byte_size)) {
        printf("'%s'", data_o->data);
    } else {
        int size_in_words = data_o->byte_size / 4;
        if(size_in_words > 1) printf("{ ");
        for(int i=0; i < size_in_words; i++) {
            // int num = *data_o->bin_data + i;
            int num = *((int*)(data_o->data + i*sizeof(int)));
            printf("%d", num);
            if(i < size_in_words-1) printf(", ");
        }
        if(size_in_words > 1) printf(" }");
    }
}

void print_asm_data_section(sct_f* sf) {
    print_title("._DATA");
    int size = sf->data_objs_num;
    data_obj* section = *sf->data_section;
    for(int i=0; i < size; i++) {
        data_obj data_o = section[i];
        print_asm_data_object(&data_o);
        if(i < size-1) printf("\t");
        if(i%1 == 0) printf("\n");
    }
}
