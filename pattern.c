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
    s_arr asm_tokens = { .arr = { "call ", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"func_name", "num_of_params"}, .len = 2 };
    
    init_cp(cp_func_call, FUNCTION_CALL, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_func_call;
}

code_pattern* init_cp_if() {
    code_pattern* cp_if = (code_pattern*) malloc(sizeof(code_pattern));

    cp_if->name = aapts("if(expression) then code_block");
    i_arr bin_tokens = { .arr = {1, 0, 0xfffffffd}, .len = 3 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "if" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = { }, .len = 0 };
    
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
    s_arr asm_tokens = { .arr = { "{", S_VAR, "}" }, .len = 3 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"size_in_words"}, .len = 1 };
    
    init_cp(cp_cblock, CODE_BLOCK, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_cblock;
}

code_pattern* init_cp_script_call() {
    code_pattern* cp_cblock = (code_pattern*) malloc(sizeof(code_pattern));

    cp_cblock->name = aapts("call_script");
    i_arr bin_tokens = { .arr = { 0, 1, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "execute", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"script id"}, .len = 1 };
    
    init_cp(cp_cblock, SCRIPT_CALL, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_cblock;
}

void init_code_patterns(code_pattern** code_patterns) {
    for(int i=0; i < CODE_PATTERNS_NUM; i++){
        code_patterns[i] = NULL;
    }

    code_patterns[0] = init_cp_var_inc();
    code_patterns[1] = init_cp_var_dec();
    code_patterns[2] = init_cp_func_call();
    code_patterns[3] = init_cp_code_block();
    code_patterns[4] = init_cp_if();
    // print_code_pattern(cp_fc);
}
void free_code_patterns(code_pattern** code_patterns) {
    for(int i=0; i<sizeof(code_patterns); i++) {
        free(code_patterns[i]);
    }
}

void print_expr_pattern(expr_pattern* expr) {
    printf("num_of_vars: %d - positions:", expr->bin_var_num);
    for(int i=0; i < expr->bin_var_num; i++) { printf(" %d ", expr->bin_var_pos[i]); }
    printf("\n");
    printf("bin_tokens:");
    for(int i=0; i < expr->bin_token_num; i++) { printf(" %08x ", expr->bin_tokens[i]); }
    printf("\n");
    printf("asm_tokens:");
    for(int i=0; i < expr->asm_token_num; i++) { printf(" %s ", expr->asm_tokens[i]); }
    printf("\n");
}

void init_expr(expr_pattern* expr, i_arr bin_tokens, i_arr bin_var_pos, s_arr asm_tokens, i_arr asm_var_pos) {
    expr->bin_token_num = bin_tokens.len;
    expr->bin_var_num = bin_var_pos.len;
    expr->asm_token_num = asm_tokens.len;
    expr->asm_var_num = asm_var_pos.len;

    expr->bin_tokens = w_malloc(expr->bin_token_num*sizeof(int*));
    expr->bin_var_pos = w_malloc(expr->bin_var_num*sizeof(int*));
    expr->asm_tokens = w_malloc(expr->asm_token_num*sizeof(char*));
    expr->asm_var_pos = w_malloc(expr->asm_var_num*sizeof(int*));

    memcpy(expr->bin_tokens, bin_tokens.arr, expr->bin_token_num*sizeof(int*));
    memcpy(expr->bin_var_pos, bin_var_pos.arr, expr->bin_var_num*sizeof(int*));
    memcpy(expr->asm_tokens, asm_tokens.arr, expr->asm_token_num*sizeof(char*));
    memcpy(expr->asm_var_pos, asm_var_pos.arr, expr->asm_var_num*sizeof(int*));
}

expr_pattern* init_expr_func_call() {
    expr_pattern* expr_func_call = (expr_pattern*) malloc(sizeof(code_pattern));
    expr_func_call->type = FUNCTION;

    i_arr bin_tokens = { .arr = {0, 3, VAR, VAR}, .len = 4 };
    i_arr bin_var_pos = { .arr = {2, 3}, .len = 2 };
    s_arr asm_tokens = { .arr = { "call ", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"func_name", "num_of_params"}, .len = 2 };
    
    init_expr(expr_func_call, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr_func_call;
}

expr_pattern* init_expr_room_timer() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = GAME_VAR;

    i_arr bin_tokens = { .arr = { 0, 2, 0, 0x5b, 4 }, .len = 5 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "ROOM_TIMER" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_room_cleared() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = GAME_VAR;

    i_arr bin_tokens = { .arr = { 0, 4, 9, 0x2c, 4 }, .len = 5 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "IS_ROOM_CLEARED" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_room_state() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = GAME_VAR;

    i_arr bin_tokens = { .arr = { 0, 4, 9, 8, 4 }, .len = 5 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "ROOM_STATE" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_plus_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 3, 1 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "+" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_minus_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 3, 2 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "-" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_times_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 3, 3 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "*" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_division_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 3, 4 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "/" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_modulo_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 3, 5 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "\%" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_eq_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 4, 2 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "==" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_ne_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 4, 3 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "!=" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_lt_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 4, 4 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "<" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_gt_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 4, 5 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { ">" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_ge_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 4, 1 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { ">=" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_le_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;

    i_arr bin_tokens = { .arr = { 4, 0 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "<=" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_var_ptr() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = VAR_PTR;

    i_arr bin_tokens = { .arr = { 0, 6, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "var", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_data_ptr() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = DATA_PTR;

    i_arr bin_tokens = { .arr = { 0, 7, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "const", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_int() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = INTEGER;

    i_arr bin_tokens = { .arr = { 2, VAR }, .len = 2 };
    i_arr bin_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "int", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

void init_expr_patterns(expr_pattern** expr_patterns) {
    for(int i=0; i < EXPR_PATTERNS_NUM; i++){
        expr_patterns[i] = NULL;
    }

    // data types
    expr_patterns[0] = init_expr_int();
    expr_patterns[1] = init_expr_data_ptr();
    expr_patterns[2] = init_expr_var_ptr();
    // math ops
    expr_patterns[3] = init_expr_plus_op();
    expr_patterns[4] = init_expr_minus_op();
    expr_patterns[5] = init_expr_times_op();
    expr_patterns[6] = init_expr_division_op();
    expr_patterns[7] = init_expr_modulo_op();
    // control op
    expr_patterns[8] = init_expr_eq_op();
    expr_patterns[9] = init_expr_ge_op();
    expr_patterns[10] = init_expr_gt_op();
    expr_patterns[11] = init_expr_le_op();
    expr_patterns[12] = init_expr_lt_op();
    expr_patterns[13] = init_expr_ne_op();
    // game
    expr_patterns[14] = init_expr_room_cleared();
    expr_patterns[15] = init_expr_room_state();
    expr_patterns[16] = init_expr_room_timer();
    expr_patterns[17] = init_expr_func_call();
}
