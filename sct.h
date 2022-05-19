#include<stdio.h>
#include<stdlib.h>

#include "utils.h"

#define VAR -9
#define S_VAR "-9"
#define CODE_PATTERNS_NUM 6
#define PARAM_PATTERNS_NUM 3
#define GAME_FUNCTIONS_NUM  0x134

const int MAX_FILENAME = 64;
const int MAX_PATH = 512;
const int MAX_DIR_PATH = 216;
const int MAX_VAR_NAME = 20;
const int MAX_FUNC_NAME = 256;
const int MAX_TOKENS_PER_EXPRESSION = 50;

typedef enum mode { MODE_BIN, MODE_ASM } mode;
typedef enum pattern_type { CODE_TYPE, PARAM_TYPE } p_type;
typedef enum code_type { IF, SWITCH, FUNCTION_CALL, SCRIPT_CALL, ASSIGNMENT, TIMER } c_type;
typedef enum param_type { INTEGER, DATA_PTR, VAR_PTR } param_type;

typedef struct game_function {
    int id;
    int params;
    char* desc;
    char* name;
} game_fun;

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

typedef struct cp_cmp_result {
    bool is_identified;
    code_pattern *match;
    void* tokens_pos;
    int var_num;
    void* vars;
} cp_cmp_result;

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

typedef struct parameter_object {
    param_type type;
    int bin_var_num;
    int bin_token_num;
    int* bin_var_pos;
    int* bin_tokens;
    int asm_var_num;
    int* asm_var_pos;
    int asm_token_num;
    char** asm_tokens;
} param_obj;

typedef struct pp_cmp_result {
    bool is_identified;
    param_pattern *match;
    void* tokens_pos;
    int var_num;
    void* vars;
} pp_cmp_result;

/*  code_obj is like code_pattern but with 
    variables encoded within the expression */
typedef struct code_object {
    c_type type;
    char** var_names;
    int bin_var_num;
    int* bin_var_pos;
    int bin_token_num;
    int* bin_tokens;
    int asm_var_num;
    int* asm_var_pos;
    int asm_token_num;
    char** asm_tokens;
    int params_num;
    param_obj* params;
} code_obj;


game_fun* game_functions[GAME_FUNCTIONS_NUM];
code_pattern* code_patterns[CODE_PATTERNS_NUM];
param_pattern* param_patterns[PARAM_PATTERNS_NUM];

typedef unsigned char BYTE;

typedef struct sct_struct {
    int num_of_scripts;
    int main_script_off;
    int script_table_off;
    int script_table_size;
    int data_sec_off;
    int data_sec_size;
    int link_table_off;
    int link_table_size;
} sct_s;

typedef struct sct_file {
    sct_s* structure;
    FILE* file;
} sct_f;

sct_s* form_structure(FILE* sctfile);
void print_sct_struct(sct_f* sf);
int get_script_off(int script_num, sct_f* sf);
void print_tokens(int* bin_tokens, int length);
int disasm_file(char* filepath);
int disasm_script(int script_offset, sct_f* sf);
bool is_filepath_valid(char* filepath);





char* c_itoa(int num, char *str)
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}

void print_code_pattern(code_pattern* cp) {
    printf("cp_name: %s\n", cp->name);
    printf("bin_vars: %d - positions:", cp->bin_var_num);
    for(int i=0; i < cp->bin_var_num; i++) { printf(" %d ", cp->bin_var_pos[i]); }
    printf("\n");
    printf("asm_vars: %d - positions:", cp->asm_var_num);
    for(int i=0; i < cp->asm_var_num; i++) { printf(" %d ", cp->asm_var_pos[i]); }
    printf("\nvar_names:");
    for(int i=0; i < cp->bin_var_num; i++) { printf(" %s ", cp->var_names[i]); }
    printf("\n");
    printf("bin_tokens:");
    for(int i=0; i < cp->bin_token_num; i++) { printf(" %08x ", cp->bin_tokens[i]); }
    printf("\n");
    printf("asm_tokens:");
    for(int i=0; i < cp->asm_token_num; i++) { printf(" %s ", cp->asm_tokens[i]); }
    printf("\n");
}

void init_cp(code_pattern* cp, c_type type, i_arr bin_tokens, i_arr bin_var_pos, s_arr var_names,
                                            s_arr asm_tokens, i_arr asm_var_pos) {
    cp->bin_tokens = w_malloc(bin_tokens.len*sizeof(int*));
    cp->bin_var_pos = w_malloc(bin_var_pos.len*sizeof(int*));
    cp->asm_tokens = w_malloc(asm_tokens.len*sizeof(char*));
    cp->asm_var_pos = w_malloc(asm_var_pos.len*sizeof(int*));
    cp->var_names = w_malloc(var_names.len*sizeof(char*));

    cp->type = type;
    cp->bin_token_num = bin_tokens.len;
    cp->asm_token_num = asm_tokens.len;
    cp->bin_var_num = bin_var_pos.len;
    cp->asm_var_num = asm_var_pos.len;

    memcpy(cp->bin_tokens, bin_tokens.arr, cp->bin_token_num*sizeof(int*));
    memcpy(cp->bin_var_pos, bin_var_pos.arr, cp->bin_var_num*sizeof(int*));
    memcpy(cp->asm_tokens, asm_tokens.arr, cp->asm_token_num*sizeof(char*));
    memcpy(cp->asm_var_pos, asm_var_pos.arr, cp->asm_var_num*sizeof(int*));
    for(int i=0; i < var_names.len; i++) {
        cp->var_names[i] = aapts(var_names.arr[i]);
    }
}

code_pattern* init_cp_func_call() {
    code_pattern* cp_func_call = (code_pattern*) malloc(sizeof(code_pattern));

    cp_func_call->name = aapts("call_game_function");
    i_arr bin_tokens = { .arr = {0, 3, VAR, VAR}, .len = 4 };
    i_arr bin_var_pos = { .arr = {2, 3}, .len = 2 };
    s_arr asm_tokens = { .arr = { "call", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"func_name", "num_of_params"}, .len = 2 };
    
    init_cp(cp_func_call, FUNCTION_CALL, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_func_call;
}
void init_code_patterns() {
    for(int i=0; i < CODE_PATTERNS_NUM; i++){
        code_patterns[i] = NULL;
    }
    code_pattern* cp_fc = init_cp_func_call();

    code_patterns[3] = cp_fc;
    // print_code_pattern(cp_fc);
}
void free_code_patterns() {
    for(int i=0; i<sizeof(code_patterns); i++) {
        free(code_patterns[i]);
    }
}

void print_param_pattern(param_pattern* pp) {
    printf("num_of_vars: %d - positions:", pp->bin_var_num);
    for(int i=0; i < pp->bin_var_num; i++) { printf(" %d ", pp->bin_var_pos[i]); }
    printf("\n");
    printf("pp_tokens:");
    for(int i=0; i < pp->bin_token_num; i++) { printf(" %08x ", pp->bin_tokens[i]); }
    printf("\n");
}

param_pattern* init_pp_integer() {
    param_pattern* pp = w_malloc(sizeof(param_pattern));
    pp->type = INTEGER;
    pp->bin_token_num = 4;
    pp->bin_var_num = 1;
    pp->asm_token_num = 2;
    pp->asm_var_num = 1;

    int bin_tokens[4] = { 0xfffffffd, 1, 2, VAR };
    int bin_var_pos[1] = { 3 };
    char* asm_tokens[2] = { "int", S_VAR };
    int asm_var_pos[1] = { 1 };

    pp->bin_tokens = w_malloc(pp->bin_token_num*sizeof(int*));
    pp->bin_var_pos = w_malloc(pp->bin_var_num*sizeof(int*));
    pp->asm_tokens = w_malloc(pp->asm_token_num*sizeof(char*));
    pp->asm_var_pos = w_malloc(pp->asm_var_num*sizeof(int*));

    memcpy(pp->bin_tokens, bin_tokens, pp->bin_token_num*sizeof(int*));
    memcpy(pp->bin_var_pos, bin_var_pos, pp->bin_var_num*sizeof(int*));
    memcpy(pp->asm_tokens, asm_tokens, pp->asm_token_num*sizeof(char*));
    memcpy(pp->asm_var_pos, asm_var_pos, pp->asm_var_num*sizeof(int*));

    return pp;
}

param_pattern* init_pp_data_ptr() {
    param_pattern* pp = w_malloc(sizeof(param_pattern));
    pp->type = DATA_PTR;
    pp->bin_token_num = 5;
    pp->bin_var_num = 1;

    int bin_tokens[5] = { 0xfffffffd, 1, 0, 7, VAR };
    int bin_var_pos[1] = { 4 };
    char* asm_tokens[2] = { "const", S_VAR };
    int asm_var_pos[1] = { 1 };

    pp->bin_tokens = w_malloc(pp->bin_token_num*sizeof(int*));
    pp->bin_var_pos = w_malloc(pp->bin_var_num*sizeof(int*));
    pp->asm_tokens = w_malloc(pp->asm_token_num*sizeof(char*));
    pp->asm_var_pos = w_malloc(pp->asm_var_num*sizeof(int*));

    memcpy(pp->bin_tokens, bin_tokens, pp->bin_token_num*sizeof(int*));
    memcpy(pp->bin_var_pos, bin_var_pos, pp->bin_var_num*sizeof(int*));
    memcpy(pp->asm_tokens, asm_tokens, pp->asm_token_num*sizeof(char*));
    memcpy(pp->asm_var_pos, asm_var_pos, pp->asm_var_num*sizeof(int*));

    return pp;
}

param_pattern* init_pp_var_ptr() {
    param_pattern* pp = w_malloc(sizeof(param_pattern));
    pp->type = VAR_PTR;
    pp->bin_token_num = 5;
    pp->bin_var_num = 1;

    int bin_tokens[5] = { 0xfffffffd, 1, 0, 6, VAR };
    int bin_var_pos[1] = { 4 };
    char* asm_tokens[2] = { "var", S_VAR };
    int asm_var_pos[1] = { 1 };

    pp->bin_tokens = w_malloc(pp->bin_token_num*sizeof(int*));
    pp->bin_var_pos = w_malloc(pp->bin_var_num*sizeof(int*));
    pp->asm_tokens = w_malloc(pp->asm_token_num*sizeof(char*));
    pp->asm_var_pos = w_malloc(pp->asm_var_num*sizeof(int*));

    memcpy(pp->bin_tokens, bin_tokens, pp->bin_token_num*sizeof(int*));
    memcpy(pp->bin_var_pos, bin_var_pos, pp->bin_var_num*sizeof(int*));
    memcpy(pp->asm_tokens, asm_tokens, pp->asm_token_num*sizeof(char*));
    memcpy(pp->asm_var_pos, asm_var_pos, pp->asm_var_num*sizeof(int*));

    return pp;
}

void init_param_patterns() {
    for(int i=0; i < PARAM_PATTERNS_NUM; i++){
        param_patterns[i] = NULL;
    }

    param_pattern* pp_int = init_pp_integer();
    param_pattern* pp_data_ptr = init_pp_data_ptr();
    param_pattern* pp_var_ptr = init_pp_var_ptr();

    param_patterns[0] = pp_int;
    param_patterns[1] = pp_data_ptr;
    param_patterns[2] = pp_var_ptr;
    // print_param_pattern(pp_int);
}

void print_game_function(game_fun gf) {
    printf("func_name: %s\n", gf.name);
    printf("func_params: %d\n", gf.params);
    printf("func_description: %s\n", gf.desc);
}

void print_game_function_i(int num) {
    game_fun* gf = game_functions[num];
    if(gf == NULL) { printf("error - game_functions uninitialized.\n"); }
    else print_game_function(*gf);
}

void init_game_functions() {
    for(int i=0; i<sizeof(game_functions)/8; i++) {
        game_fun* gf = w_malloc(sizeof(game_fun));
        gf->id = i;
        char* name = w_malloc(MAX_FUNC_NAME);
        char* num = w_malloc(4);
        char* prefix = "func_";

        c_itoa(i,num);
        strncat(name, prefix, strlen(prefix));
        strcat(name, num);
        gf->name = name;
        game_functions[i] = gf;
    }

    game_functions[0x97]->name = aapts("create_enemy");
    game_functions[0x97]->params = 4;
    game_functions[0x97]->desc = aapts("(type, id, position_ptr, -)");
    game_functions[0x06]->name = aapts("put_item_in_char_inv");
    game_functions[0x06]->params = 4;
    game_functions[0x06]->desc = aapts("(is_equiped, is_dropped, item, character)");
}
void free_game_functions() {
    for(int i=0; i<sizeof(game_functions); i++) {
        free(game_functions[i]);
    }
}