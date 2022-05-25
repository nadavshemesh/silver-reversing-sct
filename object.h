#include "pattern.h"

typedef enum o_type { OBJ_CODE, OBJ_DATA, OBJ_PARAM } o_type;
typedef struct data_object {
    int id;
    char* name;
    int references;
    int byte_size;
    int* bin_data;
    char** asm_data;
} data_obj;

typedef struct parameter_object {
    param_pattern* pp;
    int* bin_vars;
    char** asm_vars;
    data_obj* data;
} param_obj;

typedef struct code_object {
    code_pattern* cp;
    int* bin_vars;
    char** asm_vars;
    int params_num;
    param_obj* params;
    struct code_object* nested_code_objs;
} code_obj;

typedef struct obj_and_token_ptr {
    void* obj;
    o_type type;
    void* token_ptr;
} obj_and_token_ptr;


void print_data_obj(data_obj* data_o);
int compare_data_obj_ids(const void* a, const void* b);
void print_param_obj(param_obj* po, bool with_pattern);
void print_code_obj(code_obj* co);