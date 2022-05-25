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

void print_param_obj(param_obj* po, bool with_pattern) {
    print_title("PARAM_OBJ");
    if(with_pattern)
        print_param_pattern(po->pp);

    printf("bin_vars: ");
    for(int i=0; i < po->pp->bin_var_num; i++) { printf(" %08x ", po->bin_vars[i]); }
    printf("asm_vars: ");
    for(int i=0; i < po->pp->asm_var_num; i++) { printf(" %s ", po->asm_vars[i]); }
    printf("\n");

    print_data_obj(po->data);
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
    printf("params num: %d\n", co->params_num);
    for(int i=0; i < co->params_num; i++) {
        param_obj* param = &(co->params)[i];
        print_param_obj(param, true);
    }
}