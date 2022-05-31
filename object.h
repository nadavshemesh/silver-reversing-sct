#include "pattern.h"

typedef enum o_type { OBJ_CODE, OBJ_DATA, OBJ_EXPR } o_type;
typedef struct data_object {
    int id;
    int references;
    int byte_size;
    char* name;
    int* bin_data;
    char** asm_data;
} data_obj;

typedef struct expression_object {
    expr_pattern* expr_p;
    int* bin_vars;
    char** asm_vars;
    data_obj* data;
} expr_obj;

typedef struct expression {
    int expr_objs_len;
    expr_obj* expr_objs;
} expression;

typedef struct code_object {
    int code_nodes_num;
    int expression_node_num;
    code_pattern* cp;
    int* bin_vars;
    char** asm_vars;
    node** expression_nodes; 
    node** code_nodes; 
} code_obj;

typedef struct obj_and_token_ptr {
    o_type type;
    void* obj;
    void* token_ptr;
} obj_and_token_ptr;


void print_data_obj(data_obj* data_o);
int compare_data_obj_ids(const void* a, const void* b);
void print_expr_obj(expr_obj* exp_o, bool with_pattern);
void print_expression(expression* exp);
void print_code_obj(code_obj* co);
void init_code_obj(code_obj* c_obj);
code_obj* create_and_init_c_obj();
void init_expr_obj(expr_obj* e_obj);
expr_obj* create_and_init_expr_obj();