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
    int bin_tokens_len = cp->bin_token_num-cp->bin_extra_token_num;
    printf("%s (%d tokens)\n", co->cp->name, bin_tokens_len);
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