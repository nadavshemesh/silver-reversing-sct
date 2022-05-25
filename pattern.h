#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "utils.h"

#define VAR -9
#define S_VAR "-9"
#define CODE_PATTERNS_NUM 11
#define PARAM_PATTERNS_NUM 3

typedef enum pattern_type { CODE_TYPE, PARAM_TYPE } p_type;
typedef enum code_type { IF_STATEMENT, SWITCH, FUNCTION_CALL, SCRIPT_CALL, ASSIGNMENT, VAR_INC, VAR_DEC, TIMER, CODE_BLOCK } c_type;
typedef enum param_type { INTEGER, DATA_PTR, VAR_PTR } param_type;

typedef struct code_pattern {
    c_type type;
    char* name;
    char** var_names;
    int bin_var_num;
    int* bin_var_pos;
    int bin_token_num;
    int* bin_tokens;
    int asm_var_num;
    int* asm_var_pos;
    int asm_token_num;
    char** asm_tokens;
} code_pattern;

typedef struct parameter_pattern {
    param_type type;
    int bin_var_num;
    int bin_token_num;
    int* bin_var_pos;
    int* bin_tokens;
    int asm_var_num;
    int* asm_var_pos;
    int asm_token_num;
    char** asm_tokens;
} param_pattern;

typedef struct cp_cmp_result {
    bool is_identified;
    code_pattern *match;
    void* tokens_pos;
    int var_num;
    void* vars;
} cp_cmp_result;

typedef struct pp_cmp_result {
    bool is_identified;
    param_pattern *match;
    void* tokens_pos;
    int var_num;
    void* vars;
} pp_cmp_result;


void print_code_pattern(code_pattern* cp);
void init_cp(code_pattern* cp, c_type type, i_arr bin_tokens, i_arr bin_var_pos, s_arr var_names,
                                            s_arr asm_tokens, i_arr asm_var_pos);
code_pattern* init_cp_func_call();
code_pattern* init_cp_if_game_var_int();
code_pattern* init_cp_if_var_int();
code_pattern* init_cp_if_var_var();
code_pattern* init_cp_var_inc();
code_pattern* init_cp_var_dec();
void init_code_patterns(code_pattern** code_patterns);
void free_code_patterns(code_pattern** code_patterns);
void print_param_pattern(param_pattern* pp);
param_pattern* init_pp_integer();
param_pattern* init_pp_data_ptr();
param_pattern* init_pp_var_ptr();
void init_param_patterns(param_pattern** param_patterns);