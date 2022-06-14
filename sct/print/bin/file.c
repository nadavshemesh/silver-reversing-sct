#include "sct\print\bin\file.h"

void write_sct_header(sct_f* sf) {
    int magic = 0;
    int first_script = 0x20;
    int link_end = sf->structure->link_table_off + sf->structure->link_table_size*4;
    fwrite(&magic, sizeof(int), 1, sf->out_file);
    fwrite(&(sf->scripts_num), sizeof(int), 1, sf->out_file);
    fwrite(&first_script, sizeof(int), 1, sf->out_file);
    fwrite(&(sf->structure->script_table_off), sizeof(int), 1, sf->out_file);
    fwrite(&(sf->structure->data_sec_size), sizeof(int), 1, sf->out_file);
    fwrite(&(sf->structure->data_sec_off), sizeof(int), 1, sf->out_file);
    fwrite(&(sf->structure->link_table_size), sizeof(int), 1, sf->out_file);
    fwrite(&(sf->structure->link_table_off), sizeof(int), 1, sf->out_file);
    fwrite(&(link_end), sizeof(int), 1, sf->out_file);
}

void write_bin_switch_code_obj(code_obj* co, sct_f* sf) {
    int cases_num = co->bin_vars[0];
    int padding_to_ptrs = 0x41 - (cases_num);
    int padding_to_exp = 0x40 - (cases_num);

    // print tokens
    code_pattern* cp = co->cp;
    int bin_tokens_len = cp->bin_token_num-cp->bin_extra_token_num;
    // printf("%s (%d tokens)\n", co->cp->name, bin_tokens_len);
    for(int i=0, j=0; i < bin_tokens_len; i++) {
        if(is_var_pos(cp, CODE_TYPE, MODE_BIN, i)) {
            // printf("%08x", co->bin_vars[j]);
            fwrite(&(co->bin_vars[j]), sizeof(int), 1, sf->out_file);
            j++;
        } else {
            // printf("%08x", cp->bin_tokens[i]);
            fwrite(&(cp->bin_tokens[i]), sizeof(int), 1, sf->out_file);
        }
    }

    // workaround - print unknown number
    int workaround = -1;
    fwrite(&(workaround), sizeof(int), 1, sf->out_file);

    // print cases values
    for(int i=1; i < (co->bin_var_num/2); i++) {
        // printf("%08x", co->bin_vars[i]);
        fwrite(&(co->bin_vars[i]), sizeof(int), 1, sf->out_file);
    }

    // print padding
    for(int i=0; i < padding_to_ptrs; i++) { 
        // printf("00"); 
        int padding = 0;
        fwrite(&(padding), sizeof(int), 1, sf->out_file);
    }

    // print offsets
    for(int i=(co->bin_var_num/2)+1; i < co->bin_var_num; i++) {
        // printf("%08x", co->bin_vars[i]);
        fwrite(&(co->bin_vars[i]), sizeof(int), 1, sf->out_file);
    }

    // print padding
    for(int i=0; i < padding_to_exp; i++) {
    //  printf("00"); 
        int padding = 0;
        fwrite(&(padding), sizeof(int), 1, sf->out_file);
    }

    // print expression
    if(co->expression_node_num == 1) {
        node* exp_node = *co->expression_nodes;
        write_bin_expression(exp_node->item, sf);
    }   
    
    // print code block
    if(co->code_nodes_num == 1) {
        node* code_node = *co->code_nodes;
        write_bin_code_obj(code_node->item, sf);
    }   
}

void write_bin_expr_obj(expr_obj* expr, sct_f* sf) {
    expr_pattern* ep = expr->expr_p;
    for(int i=0, j=0; i < ep->bin_token_num; i++) {
        if(is_var_pos(ep, EXPRESSION_TYPE, MODE_BIN, i)) {
            // printf("%08x", expr->bin_vars[j]);
            fwrite(&(expr->bin_vars[j]), sizeof(int), 1, sf->out_file);
            j++;
        } else {
            // printf("%08x", ep->bin_tokens[i]);
            fwrite(&(ep->bin_tokens[i]), sizeof(int), 1, sf->out_file);
        }
    }

    if(expr->expression_node_num > 0) {
        node* exp_node = *expr->expression_nodes;
        for(int i=0; i < expr->expression_node_num; i++) {
            write_bin_expression(exp_node->item, sf);
            exp_node = exp_node->next;
        }
    }   
}

void write_bin_expression(expression* exp, sct_f* sf) {
    int prologue[2] = { 0xfffffffd, exp->expr_objs_len };
    fwrite(prologue, 2*sizeof(int), 1, sf->out_file);
    for(int i=0; i < exp->expr_objs_len; i++) {
        expr_obj* eo = exp->expr_objs+i;
        // printf("%08x%08x", 0xfffffffd, exp->expr_objs_len);
        write_bin_expr_obj(eo, sf);
    }
}

void write_bin_code_obj(code_obj* co, sct_f* sf) {
    code_pattern* cp = co->cp;
    if(cp->type == SWITCH) {
        write_bin_switch_code_obj(co, sf);
    } else {
        int bin_tokens_len = cp->bin_token_num+cp->bin_extra_token_num;
        // printf("%s (%d tokens)\n", co->cp->name, bin_tokens_len);
        for(int i=0, j=0; i < bin_tokens_len; i++) {
            if(is_var_pos(cp, CODE_TYPE, MODE_BIN, i)) {
                // printf("%08x", co->bin_vars[j]);
                fwrite(&(co->bin_vars[j]), sizeof(int), 1, sf->out_file);
                j++;
            } else {
                // printf("%08x", cp->bin_tokens[i]);
                fwrite(&(cp->bin_tokens[i]), sizeof(int), 1, sf->out_file);
            }
        }

        if(co->expression_node_num > 0) {
            node* exp_node = *co->expression_nodes;
            for(int i=0; i < co->expression_node_num; i++) {
                write_bin_expression(exp_node->item, sf);
                exp_node = exp_node->next;
            }
        }

        if(co->code_nodes_num > 0) {
            node* code_node = *co->code_nodes;
            for(int i=0; i < co->code_nodes_num; i++) {
                write_bin_code_obj(code_node->item, sf);
                code_node = code_node->next;
            }
        }
    }
}

void write_bin_data_obj(data_obj* data_o, sct_f* sf) {
    fwrite(data_o->data, data_o->byte_size, 1, sf->out_file);
}

void write_bin_data_section(sct_f* sf) {
    data_obj** ds = sf->data_section;

    for(int i=0; i < sf->data_objs_num; i++) {
        data_obj* data_o = ds[i];
        write_bin_data_obj(data_o, sf);
    }
}

void write_bin_link_table(sct_f* sf) {
    node* link = *sf->data_link_table;
    while(link != NULL) {
        fwrite(link->item, sizeof(int), 1, sf->out_file);
        link = link->next;
    }
}

void write_bin_code_nodes(node* head, sct_f* sf) {
    node* node = head;
    if(node == NULL) return;
    while(node != NULL && node->item != NULL) {
        code_obj* co = (code_obj*) node->item;
        write_bin_code_obj(co, sf);
        node = node->next;
    }
}

void write_bin_script(script* script, sct_f* sf) {
    write_bin_code_nodes(*script->code_nodes, sf);
}

void write_bin_code_section(sct_f* sf) {
    for(int i=0; i < sf->scripts_num; i++) {
    // for(int i=0; i < 1; i++) {
        write_bin_script(sf->scripts[i], sf);
    }
}

void write_bin_script_table(sct_f* sf) {
    fwrite(sf->script_table, sf->structure->script_table_size, 1, sf->out_file);
}