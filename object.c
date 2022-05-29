#include "object.h"

void print_data_obj(data_obj* data_o) {
    printf("data_obj id: %d\n", data_o->id);
    printf("references: %d\n", data_o->references);
    printf("size in bytes: %d\n", data_o->byte_size);
    printf("bin_data: ");
    for(int i=0; i < data_o->byte_size/4; i++) {
        printf(" %08x ", *(data_o->bin_data+i));
    }
    printf(" [%s] ", data_o->bin_data);
    printf("\n");
}

int compare_data_obj_ids(const void* a, const void* b) {
     int int_a = (((data_obj*)a)->id);
     int int_b = (((data_obj*)b)->id);

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
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
    printf("expr nodes: %d\n", co->expression_node_num);
    if(co->expression_node_num > 0) {
        node* exp_node = *co->expression_nodes;
        while(exp_node != NULL) {
            print_expression(exp_node->item);
            exp_node = exp_node->next;
        }
    }
}