#include "sct\print\bin\console.h"


void print_bin_token_seg(int* tokens, int from_row, int to_row, int cols) {
    int* token = tokens;
    for(int i=from_row*cols; i < to_row*cols; i++) {
        if(i != from_row*cols && i%cols == 0) printf("\n");
        if(&(tokens[i]) == token)
            printf(ANSI_COLOR_RED "%08x " ANSI_COLOR_RESET, tokens[i]);
        else
            printf("%8.8x ", tokens[i]);
    }
    printf("\n");
}

void print_bin_switch_code_obj(code_obj* co) {
    int cases_num = co->bin_vars[0];
    int padding_to_ptrs = 260 - (cases_num*4);
    int padding_to_exp = 256 - (cases_num*4);

    // print tokens
    code_pattern* cp = co->cp;
    int bin_tokens_len = cp->bin_token_num-cp->bin_extra_token_num;
    // printf("%s (%d tokens)\n", co->cp->name, bin_tokens_len);
    for(int i=0, j=0; i < bin_tokens_len; i++) {
        if(is_var_pos(cp, CODE_TYPE, MODE_BIN, i)) {
            printf("%08x", co->bin_vars[j]);
            j++;
        } else {
            printf("%08x", cp->bin_tokens[i]);
        }
    }

    // workaround - print unknown number
    printf("%08x", -1);

    // print cases values
    for(int i=1; i < (co->bin_var_num/2); i++) {
        printf("%08x", co->bin_vars[i]);
    }

    // print padding
    for(int i=0; i < padding_to_ptrs; i++) { printf("00"); }

    // print offsets
    for(int i=(co->bin_var_num/2)+1; i < co->bin_var_num; i++) {
        printf("%08x", co->bin_vars[i]);
    }

    // print padding
    for(int i=0; i < padding_to_exp; i++) { printf("00"); }

    // print expression
    if(co->expression_node_num == 1) {
        node* exp_node = *co->expression_nodes;
        print_bin_expression(exp_node->item);
    }   
    
    // print code block
    if(co->code_nodes_num == 1) {
        node* code_node = *co->code_nodes;
        print_bin_code_obj(code_node->item);
    }   
}

void print_bin_expr_obj(expr_obj* expr) {
    expr_pattern* ep = expr->expr_p;
    for(int i=0, j=0; i < ep->bin_token_num; i++) {
        if(is_var_pos(ep, EXPRESSION_TYPE, MODE_BIN, i)) {
            printf("%08x", expr->bin_vars[j]);
            j++;
        } else {
            printf("%08x", ep->bin_tokens[i]);
        }
    }

    if(expr->expression_node_num > 0) {
        node* exp_node = *expr->expression_nodes;
        for(int i=0; i < expr->expression_node_num; i++) {
            print_bin_expression(exp_node->item);
            exp_node = exp_node->next;
        }
    }   
}

void print_bin_expression(expression* exp) {
    for(int i=0; i < exp->expr_objs_len; i++) {
        expr_obj* eo = (&exp->expr_objs)[i];
        printf("%08x%08x", 0xfffffffd, exp->expr_objs_len);
        print_bin_expr_obj(eo);
    }
}

void print_bin_code_obj(code_obj* co) {
    code_pattern* cp = co->cp;
    if(cp->type == SWITCH) {
        print_bin_switch_code_obj(co);
    } else {
        int bin_tokens_len = cp->bin_token_num-cp->bin_extra_token_num;
        // printf("%s (%d tokens)\n", co->cp->name, bin_tokens_len);
        for(int i=0, j=0; i < bin_tokens_len; i++) {
            if(is_var_pos(cp, CODE_TYPE, MODE_BIN, i)) {
                printf("%08x", co->bin_vars[j]);
                j++;
            } else {
                printf("%08x", cp->bin_tokens[i]);
            }
        }

        if(co->expression_node_num > 0) {
            node* exp_node = *co->expression_nodes;
            for(int i=0; i < co->expression_node_num; i++) {
                print_bin_expression(exp_node->item);
                exp_node = exp_node->next;
            }
        }

        if(co->code_nodes_num > 0) {
            node* code_node = *co->code_nodes;
            for(int i=0; i < co->code_nodes_num; i++) {
                print_bin_code_obj(code_node->item);
                code_node = code_node->next;
            }
        }
    }
}

void print_bin_data_obj(data_obj* data_o) {
    for(int i=0; i < data_o->byte_size; i++) {
        printf("%02x", (&data_o->data)[i]);
    }
}

void print_bin_data_section(sct_f* sf) {
    data_obj** ds = sf->data_section;
    if(ds == NULL) { print_err_and_exit("Error, data section is undefined.", -4); }

    for(int i=0; i < sf->data_objs_num; i++) {
        data_obj* data_o = ds[i];
        print_bin_data_obj(data_o);
    }
}

void print_bin_code_nodes(node* head) {
    printf("check\n");
    node* node = head;
    if(node == NULL) return;
    while(node != NULL && node->item != NULL) {
        code_obj* co = (code_obj*) node->item;
        print_bin_code_obj(co);
        node = node->next;
    }
}

void print_bin_script(script* script) {
    print_bin_code_nodes(*script->code_nodes);
}

void print_bin_file(sct_f* sf) {
    for(int i=0; i < sf->scripts_num; i++) {
        print_bin_script(sf->scripts[i]);
    }
}