#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "utils.h"

#define VAR -9
#define S_VAR "-9"
#define CODE_PATTERNS_NUM 14
#define EXPR_PATTERNS_NUM 18
#define UNINIT_TYPE 111

typedef enum pattern_type { CODE_TYPE, EXPRESSION_TYPE } p_type; // needed?
typedef enum code_type { EXPRESSION, IF_STATEMENT, ELSE_STATEMENT, SWITCH, BREAK, FUNCTION_CALL, SCRIPT_CALL, 
            ASSIGNMENT, VAR_INC, VAR_DEC, CODE_BLOCK, CODE_BLOCK_CASES, CASE_AID, ROOM_VAR_PTR } c_type;
typedef enum expr_type { OPERATOR, INTEGER, DATA_PTR, VAR_PTR, FUNCTION, GAME_VAR } expr_type;
typedef enum operator_type { BINARY_OP, UNARY_OP} operator_type;

typedef struct code_pattern {
    c_type type;
    int asm_var_num;
    int bin_var_num;
    int bin_token_num;
    int asm_token_num;
    char* name;
    char** var_names;
    int* bin_var_pos;
    int* bin_tokens;
    int* asm_var_pos;
    char** asm_tokens;
} code_pattern;

typedef struct expr_pattern {
    expr_type type;
    int bin_var_num;
    int bin_token_num;
    int asm_var_num;
    int asm_token_num;
    int* bin_var_pos;
    int* bin_tokens;
    int* asm_var_pos;
    char** asm_tokens;
} expr_pattern;

typedef struct cp_cmp_result {
    bool is_identified;
    int var_num;
    code_pattern *match;
    void* tokens_pos;
    void* vars;
} cp_cmp_result;

typedef struct expr_cmp_result {
    bool is_identified;
    int var_num;
    expr_pattern *match;
    void* tokens_pos;
    void* vars;
} expr_cmp_result;


void print_code_pattern(code_pattern* cp);
void init_cp(code_pattern* cp, c_type type, i_arr bin_tokens, i_arr bin_var_pos, s_arr var_names,
                                            s_arr asm_tokens, i_arr asm_var_pos);
void init_code_patterns(code_pattern** code_patterns);
void free_code_patterns(code_pattern** code_patterns);
void print_expr_pattern(expr_pattern* exp);
void init_expr_patterns(expr_pattern** expr_patterns);
void init_code_pattern(code_pattern* c_pattern);
code_pattern* create_and_init_code_pattern();
void init_expr_pattern(expr_pattern* e_pattern);
expr_pattern* create_and_init_expr_pattern();
code_pattern* init_aid_cp_case();