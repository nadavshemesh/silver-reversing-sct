#include "sct\structure\object.h"

void init_code_obj(code_obj* c_obj) {
    c_obj->cp = NULL;
    c_obj->hint_comment = NULL;
    c_obj->bin_vars = NULL;
    c_obj->asm_vars = NULL;
    c_obj->bin_var_num = 0;
    c_obj->asm_var_num = 0;
    c_obj->code_nodes_num = 0;
    c_obj->code_nodes = NULL;
    c_obj->expression_node_num = 0;
    c_obj->expression_nodes = NULL; 
}

code_obj* create_and_init_c_obj() {
    code_obj* c_obj = w_malloc(sizeof(code_obj));
    init_code_obj(c_obj);

    return c_obj;
}

void init_expression(expression* exp) {
    exp->expr_objs_len = 0;
    exp->expr_objs = NULL;
}

expression* create_and_init_expression() {
    expression* exp = w_malloc(sizeof(expression));
    init_expression(exp);

    return exp;
}

void init_data_obj(data_obj* d_obj) {
    d_obj->id = -1;
    d_obj->ignore = false;
    d_obj->was_renamed = false;
    d_obj->references = 0;
    d_obj->byte_size = 0;
    d_obj->name = NULL;
    d_obj->data = NULL;
}

data_obj* create_and_init_data_obj() {
    data_obj* d_obj = w_malloc(sizeof(data_obj));
    init_data_obj(d_obj);

    return d_obj;
}

void init_expr_obj(expr_obj* e_obj) {
    e_obj->expr_p = NULL;
    e_obj->bin_vars = NULL;
    e_obj->asm_vars = NULL;
    e_obj->expression_nodes = NULL;
    e_obj->data = 0;
    e_obj->expression_node_num = 0;
    e_obj->hint_comment = NULL;
}

expr_obj* create_and_init_expr_obj() {
    expr_obj* e_obj = w_malloc(sizeof(expr_obj));
    init_expr_obj(e_obj);

    return e_obj;
}

void print_data_obj(data_obj* data_o) {
    print_title("DATA_OBJ");
    if(data_o->name != NULL)
        printf("data_obj name: %s\n", data_o->name);
    printf("data_obj id: %d (Hex: %02x)\n", data_o->id, data_o->id);
    printf("references: %d\n", data_o->references);
    printf("size in bytes: %d\n", data_o->byte_size);
    printf("data: ");
    for(int i=0; i < data_o->byte_size; i++) {
        printf(" %02x ", *(data_o->data+i));
    }
    if(is_string(data_o->data, data_o->byte_size)) {
        printf("\n [ %s ] \n", data_o->data);
    } else {
        printf("[");
        for(int i=0; i < data_o->byte_size/4; i++) {
            printf(" %08x ", *((int*)(data_o->data+i*sizeof(int))));
        }
        printf("]");
        printf("\n");
    }
}

int compare_data_obj_ids(const void* a, const void* b) {
     int int_a = (((data_obj*)a)->id);
     int int_b = (((data_obj*)b)->id);

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

expr_obj* get_last_expr_obj(expression* exp) {
    if(exp->expr_objs == NULL) { print_err_and_exit("Error, get_last_expr_obj on an empty expression.", -2); }
    return &(exp->expr_objs[exp->expr_objs_len-1]);
}

void print_expr_obj(expr_obj* exp_o, bool with_pattern) {
    print_title("expr_obj");
    if(with_pattern)
        print_expr_pattern(exp_o->expr_p);

    printf("bin_vars: ");
    for(int i=0; i < exp_o->expr_p->bin_var_num; i++) { printf(" %08x ", exp_o->bin_vars[i]); }
    printf("asm_vars: ");
    for(int i=0; i < exp_o->expr_p->asm_var_num; i++) { printf(" %s ", exp_o->asm_vars[i]); }
    printf("\n");
    printf("sub expr nodes: %d\n", exp_o->expression_node_num);
    // if(exp_o->expression_node_num > 0) {
    //     node* exp_node = *exp_o->expression_nodes;
    //     while(exp_node != NULL) {
    //         print_expression(exp_node->item);
    //         exp_node = exp_node->next;
    //     }
    // }

    if(exp_o->data != NULL)
        print_data_obj(exp_o->data);
}

void print_expression(expression* exp) {
    for(int i=0; i < exp->expr_objs_len; i++) {
        expr_obj* expr_o = &(exp->expr_objs)[i];
        print_expr_obj(expr_o, true);
    }
}

void print_code_obj(code_obj* co) {
    print_title("CODE_OBJ");
    print_code_pattern(co->cp);
    printf("asm vars:");
    for(int i=0; i < co->cp->asm_var_num; i++) { 
        printf(" %s ", co->asm_vars[i]);
    }
    printf("\nbin vars:");
    for(int i=0; i < co->cp->bin_var_num; i++) { 
        printf(" %08x ", co->bin_vars[i]);
    }
    printf("\ncode nodes: %d\n", co->code_nodes_num);
    printf("expr nodes: %d\n", co->expression_node_num);
    if(co->expression_node_num > 0) {
        node* exp_node = *co->expression_nodes;
        while(exp_node != NULL) {
            print_expression(exp_node->item);
            exp_node = exp_node->next;
        }
    }
}