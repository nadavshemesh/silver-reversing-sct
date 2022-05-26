#include "pattern.h"

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

code_pattern* init_cp_if_var_int() {
    code_pattern* cp_if = (code_pattern*) malloc(sizeof(code_pattern));

    cp_if->name = aapts("compare var with integer value");
    i_arr bin_tokens = { .arr = {1, 0, 0xfffffffd, 3, 0, 6, VAR, 4, VAR, VAR, VAR}, .len = 11 };
    i_arr bin_var_pos = { .arr = {6, 8, 9, 10}, .len = 4 };
    s_arr asm_tokens = { .arr = { "if", "(", "var", S_VAR, S_VAR, "int", S_VAR, ")" }, .len = 8 };
    i_arr asm_var_pos = { .arr = { 2, 3, 4 }, .len = 3 };
    s_arr var_names = { .arr = {"var_ptr", "== , >= , <=, >, <, !=", "int value"}, .len = 2 };
    
    init_cp(cp_if, IF_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_if;
}

code_pattern* init_cp_if_game_var_int() {
    code_pattern* cp_if = (code_pattern*) malloc(sizeof(code_pattern));

    cp_if->name = aapts("compare game_var with integer value");
    i_arr bin_tokens = { .arr = {1, 0, 0xfffffffd, 3, 0, 3, VAR, 4, VAR, VAR, VAR}, .len = 11 };
    i_arr bin_var_pos = { .arr = {6, 8, 9, 10}, .len = 4 };
    s_arr asm_tokens = { .arr = { "if", "(", "game_var", S_VAR, S_VAR, "int", S_VAR, ")" }, .len = 8 };
    i_arr asm_var_pos = { .arr = { 2, 3, 4 }, .len = 3 };
    s_arr var_names = { .arr = {"game_var", "== , >= , <=, >, <, !=", "int value"}, .len = 2 };
    
    init_cp(cp_if, IF_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_if;
}

code_pattern* init_cp_if_var_var() {
    code_pattern* cp_if = (code_pattern*) malloc(sizeof(code_pattern));

    cp_if->name = aapts("compare var with another var");
    i_arr bin_tokens = { .arr = {1, 0, 0xfffffffd, 3, 0, 6, VAR, 4, VAR, VAR, 0, 6, VAR}, .len = 13 };
    i_arr bin_var_pos = { .arr = {6, 8, 9, 12}, .len = 4 };
    s_arr asm_tokens = { .arr = { "if", "(", "var", S_VAR, S_VAR, "var", S_VAR, ")" }, .len = 8 };
    i_arr asm_var_pos = { .arr = { 2, 3, 4 }, .len = 3 };
    s_arr var_names = { .arr = {"var_ptr", "== , >= , <=, >, <, !=", "var_ptr"}, .len = 2 };
    
    init_cp(cp_if, IF_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_if;
}

code_pattern* init_cp_var_inc() {
    code_pattern* cp_inc = (code_pattern*) malloc(sizeof(code_pattern));

    cp_inc->name = aapts("increment variable by 1");
    i_arr bin_tokens = { .arr = { 0, 6, VAR, 7, 0}, .len = 5 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { S_VAR, "++" }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"var_ptr"}, .len = 1 };
    
    init_cp(cp_inc, VAR_INC, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_inc;
}

code_pattern* init_cp_var_dec() {
    code_pattern* cp_dec = (code_pattern*) malloc(sizeof(code_pattern));

    cp_dec->name = aapts("decrement variable by 1");
    i_arr bin_tokens = { .arr = { 0, 6, VAR, 7, 1}, .len = 5 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { S_VAR, "--" }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"var_ptr"}, .len = 1 };
    
    init_cp(cp_dec, VAR_DEC, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_dec;
}

code_pattern* init_cp_code_block() {
    code_pattern* cp_cblock = (code_pattern*) malloc(sizeof(code_pattern));

    cp_cblock->name = aapts("code block");
    i_arr bin_tokens = { .arr = { 0xfffffffc, VAR }, .len = 2 };
    i_arr bin_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "{" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"size_in_words"}, .len = 1 };
    
    init_cp(cp_cblock, CODE_BLOCK, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_cblock;
}

void init_code_patterns(code_pattern** code_patterns) {
    for(int i=0; i < CODE_PATTERNS_NUM; i++){
        code_patterns[i] = NULL;
    }
    code_pattern* cp_fc = init_cp_func_call();
    code_pattern* cp_if_int = init_cp_if_var_int();
    code_pattern* cp_if_gf_int = init_cp_if_game_var_int();
    code_pattern* cp_if_var_var = init_cp_if_var_var();
    code_pattern* cp_var_inc = init_cp_var_inc();
    code_pattern* cp_var_dec = init_cp_var_dec();
    code_pattern* cp_cblock = init_cp_code_block();

    code_patterns[0] = cp_var_inc;
    code_patterns[1] = cp_var_dec;
    code_patterns[2] = cp_if_int;
    code_patterns[3] = cp_fc;
    code_patterns[4] = cp_if_gf_int;
    code_patterns[5] = cp_if_var_var;
    code_patterns[6] = cp_cblock;
    // print_code_pattern(cp_fc);
}
void free_code_patterns(code_pattern** code_patterns) {
    for(int i=0; i<sizeof(code_patterns); i++) {
        free(code_patterns[i]);
    }
}

void print_param_pattern(param_pattern* pp) {
    printf("num_of_vars: %d - positions:", pp->bin_var_num);
    for(int i=0; i < pp->bin_var_num; i++) { printf(" %d ", pp->bin_var_pos[i]); }
    printf("\n");
    printf("bin_tokens:");
    for(int i=0; i < pp->bin_token_num; i++) { printf(" %08x ", pp->bin_tokens[i]); }
    printf("\n");
    printf("asm_tokens:");
    for(int i=0; i < pp->asm_token_num; i++) { printf(" %s ", pp->asm_tokens[i]); }
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
    pp->asm_token_num = 2;
    pp->bin_var_num = 1;
    pp->asm_var_num = 1;

    int bin_tokens[5] = { 0xfffffffd, 1, 0, 7, VAR };
    int bin_var_pos[1] = { 4 };
    char* asm_tokens[2] = { "const", S_VAR };
    int asm_var_pos[1] = { 1 };

    pp->bin_tokens = w_malloc(pp->bin_token_num*sizeof(int*));
    pp->bin_var_pos = w_malloc(pp->bin_var_num*sizeof(int*));
    pp->asm_tokens = w_malloc(pp->asm_token_num*sizeof(int*));
    pp->asm_var_pos = w_malloc(pp->asm_var_num*sizeof(int*));

    memcpy(pp->bin_tokens, bin_tokens, pp->bin_token_num*sizeof(int*));
    memcpy(pp->bin_var_pos, bin_var_pos, pp->bin_var_num*sizeof(int*));
    memcpy(pp->asm_tokens, asm_tokens, sizeof(pp->asm_tokens));
    memcpy(pp->asm_var_pos, asm_var_pos, pp->asm_var_num*sizeof(int*));

    return pp;
}

param_pattern* init_pp_var_ptr() {
    param_pattern* pp = w_malloc(sizeof(param_pattern));
    pp->type = VAR_PTR;
    pp->bin_token_num = 5;
    pp->bin_var_num = 1;
    pp->asm_token_num = 2;
    pp->asm_var_num = 1;

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

void init_param_patterns(param_pattern** param_patterns) {
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
