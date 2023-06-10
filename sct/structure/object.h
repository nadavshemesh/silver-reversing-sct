#ifndef FILE_OBJ_INCL
#define FILE_OBJ_INCL
#include "sct\structure\pattern.h"

typedef enum o_type { OBJ_CODE, OBJ_DATA, OBJ_EXPR } o_type;
typedef enum data_type { UNDEFINED_TYPE, STRING, NUMBER, ARRAY } data_type;
typedef enum ref_type { DATA_DATA, DATA_CODE } ref_type;

typedef struct data_object {
    int id;
    int references;
    int data_references;
    int byte_size;
    data_type type;
    bool ignore;
    bool was_renamed;
    char* name;
    byte* data;
    node** e_referees;
    node** c_referees;
    node** d_references;
} data_obj;

typedef struct data_ref {
    int id;
    ref_type type;
    int* offset;
    char* unfound_name;
    data_obj* data_parent;
    data_obj* data_item;
} data_ref;

typedef struct expression_object {
    int expression_node_num;
    char* hint_comment;
    expr_pattern* expr_p;
    int* bin_vars;
    char** asm_vars;
    data_obj* data;
    node** expression_nodes; 
} expr_obj;

typedef struct expression {
    int expr_objs_len;
    expr_obj* expr_objs;
} expression;

typedef struct code_object {
    int bin_var_num;
    int asm_var_num;
    int code_nodes_num;
    int expression_node_num;
    char* hint_comment;
    data_obj* data;
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
expr_obj* get_last_expr_obj(expression* exp);
data_ref* create_and_init_data_ref();
expr_obj* create_and_init_expr_obj();
data_obj* create_and_init_data_obj();
expression* create_and_init_expression();

#endif