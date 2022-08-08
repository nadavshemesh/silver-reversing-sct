#include "sct\structure\pattern.h"

void init_code_pattern(code_pattern* c_pattern) {
    c_pattern->asm_token_num = 0;
    c_pattern->bin_token_num = 0;
    c_pattern->bin_extra_token_num = 0;
    c_pattern->asm_var_num = 0;
    c_pattern->bin_var_num = 0;
    c_pattern->asm_tokens = NULL;
    c_pattern->bin_tokens = NULL;
    c_pattern->asm_var_pos = NULL;
    c_pattern->bin_var_pos = NULL;
    c_pattern->var_names = NULL;
    c_pattern->name = NULL;
    c_pattern->type = UNINIT_TYPE;
}

code_pattern* create_and_init_code_pattern() {
    code_pattern* c_pattern = w_malloc(sizeof(code_pattern));
    init_code_pattern(c_pattern);

    return c_pattern;
}

void init_expr_pattern(expr_pattern* e_pattern) {
    e_pattern->asm_token_num = 0;
    e_pattern->bin_token_num = 0;
    e_pattern->asm_var_num = 0;
    e_pattern->bin_var_num = 0;
    e_pattern->asm_tokens = NULL;
    e_pattern->bin_tokens = NULL;
    e_pattern->asm_var_pos = NULL;
    e_pattern->bin_var_pos = NULL;
    e_pattern->type = UNINIT_TYPE;
}

expr_pattern* create_and_init_expr_pattern() {
    expr_pattern* e_pattern = w_malloc(sizeof(expr_pattern));
    init_expr_pattern(e_pattern);

    return e_pattern;
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
    code_pattern* cp_func_call = create_and_init_code_pattern();

    cp_func_call->name = aapts("call_game_function");
    i_arr bin_tokens = { .arr = {0, 3, VAR, VAR}, .len = 4 };
    i_arr bin_var_pos = { .arr = {2, 3}, .len = 2 };
    s_arr asm_tokens = { .arr = { "call", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"func_name", "num_of_params"}, .len = 2 };
    
    init_cp(cp_func_call, FUNCTION_CALL, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_func_call;
}

code_pattern* init_cp_if() {
    code_pattern* cp_if = create_and_init_code_pattern();

    cp_if->name = aapts("if(expression) then code_block");
    cp_if->bin_extra_token_num = -1; // 0xfffffffd is extra for recognition
    i_arr bin_tokens = { .arr = {1, 0, 0xfffffffd}, .len = 3 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "if" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = { }, .len = 0 };
    
    init_cp(cp_if, IF_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_if;
}

code_pattern* init_cp_while() {
    code_pattern* cp_if = create_and_init_code_pattern();

    cp_if->name = aapts("while(expression) then code_block");
    cp_if->bin_extra_token_num = -1; // 0xfffffffd is extra for recognition
    i_arr bin_tokens = { .arr = {1, 7, 0xfffffffd}, .len = 3 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "while" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = { }, .len = 0 };
    
    init_cp(cp_if, IF_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_if;
}

code_pattern* init_cp_var() {
    code_pattern* cp_var = create_and_init_code_pattern();

    cp_var->name = aapts("var reference from data table");
    i_arr bin_tokens = { .arr = { 0, 6, VAR}, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "var" , S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = { "var_id" }, .len = 1 };
    
    init_cp(cp_var, CP_VAR_PTR, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_var;
}

code_pattern* init_cp_data_index_ptr() {
    code_pattern* cp_var = create_and_init_code_pattern();

    cp_var->name = aapts("@var_name[int index] from data table");
    i_arr bin_tokens = { .arr = { 0, 5, VAR}, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "@" , S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = { "var_id" }, .len = 1 };
    
    init_cp(cp_var, CP_DATA_INDEX_PTR, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_var;
}

code_pattern* init_cp_var_inc() {
    code_pattern* cp_inc = create_and_init_code_pattern();

    cp_inc->name = aapts("increment variable by 1");
    i_arr bin_tokens = { .arr = { 7, 0}, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "++"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = {  }, .len = 0 };
    s_arr var_names = { .arr = { }, .len = 0 };
    
    init_cp(cp_inc, VAR_INC, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_inc;
}

code_pattern* init_cp_var_dec() {
    code_pattern* cp_dec = create_and_init_code_pattern();

    cp_dec->name = aapts("decrement variable by 1");
    i_arr bin_tokens = { .arr = { 7, 1}, .len = 2 };
    i_arr bin_var_pos = { .arr = { 0 }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "--"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { 0 }, .len = 0 };
    s_arr var_names = { .arr = {}, .len = 0 };
    
    init_cp(cp_dec, VAR_DEC, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_dec;
}

code_pattern* init_cp_var_assignment() {
    code_pattern* cp_assign = create_and_init_code_pattern();

    cp_assign->name = aapts("assign var an expression.");
    i_arr bin_tokens = { .arr = { 7, 2}, .len = 2 };
    i_arr bin_var_pos = { .arr = { 0 }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "=" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { 0 }, .len = 0 };
    s_arr var_names = { .arr = {}, .len = 0 };
    
    init_cp(cp_assign, ASSIGNMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_assign;
}

code_pattern* init_cp_var_assignment_or() {
    code_pattern* cp_assign = create_and_init_code_pattern();

    cp_assign->name = aapts(" |= assign var an expression.");
    i_arr bin_tokens = { .arr = { 7, 5}, .len = 2 };
    i_arr bin_var_pos = { .arr = { 0 }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "|=" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { 0 }, .len = 0 };
    s_arr var_names = { .arr = {}, .len = 0 };
    
    init_cp(cp_assign, ASSIGNMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_assign;
}

code_pattern* init_cp_var_assignment_plus() {
    code_pattern* cp_assign = create_and_init_code_pattern();

    cp_assign->name = aapts(" += assign var an expression.");
    i_arr bin_tokens = { .arr = { 7, 3}, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "+=" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = {}, .len = 0 };
    
    init_cp(cp_assign, ASSIGNMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_assign;
}

code_pattern* init_cp_var_assignment_minus() {
    code_pattern* cp_assign = create_and_init_code_pattern();

    cp_assign->name = aapts(" -= assign var an expression.");
    i_arr bin_tokens = { .arr = { 7, 4 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "-=" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = {}, .len = 0 };
    
    init_cp(cp_assign, ASSIGNMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_assign;
}

code_pattern* init_cp_var_assignment_and() {
    code_pattern* cp_assign = create_and_init_code_pattern();

    cp_assign->name = aapts(" &= assign var an expression.");
    i_arr bin_tokens = { .arr = { 7, 6}, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "&=" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = {}, .len = 0 };
    
    init_cp(cp_assign, ASSIGNMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_assign;
}

code_pattern* init_cp_code_block() {
    code_pattern* cp_cblock = create_and_init_code_pattern();

    cp_cblock->name = aapts("code block");
    i_arr bin_tokens = { .arr = { 0xfffffffc, VAR }, .len = 2 };
    i_arr bin_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr asm_tokens = { .arr = { }, .len = 0 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = {"size_in_words"}, .len = 1 };
    
    init_cp(cp_cblock, CODE_BLOCK, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_cblock;
}

code_pattern* init_cp_script_call() {
    code_pattern* cp_scall = create_and_init_code_pattern();

    cp_scall->name = aapts("call_script");
    i_arr bin_tokens = { .arr = { 0, 1, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "execute", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"script_index"}, .len = 1 };
    
    init_cp(cp_scall, SCRIPT_CALL, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_scall;
}

code_pattern* init_cp_structure_ptr() {
    code_pattern* cp = create_and_init_code_pattern();

    cp->name = aapts("struct ptr");
    i_arr bin_tokens = { .arr = { 0, VAR, VAR, VAR, 4 }, .len = 5 };
    i_arr bin_var_pos = { .arr = { 1, 2, 3 }, .len = 3 };
    s_arr asm_tokens = { .arr = { "gamevar", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"first offset", "second offset", "third_offset"}, .len = 3 };
    
    init_cp(cp, ROOM_VAR_PTR, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp;
}

code_pattern* init_cp_switch_case() {
    code_pattern* cp_switch = create_and_init_code_pattern();

    cp_switch->name = aapts("switch_case");
    i_arr bin_tokens = { .arr = { 1, 9, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "switch" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = {"num_of_cases"}, .len = 1 };
    
    init_cp(cp_switch, SWITCH, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_switch;
}

code_pattern* init_cp_break() {
    code_pattern* cp_break = create_and_init_code_pattern();

    cp_break->name = aapts("break");
    i_arr bin_tokens = { .arr = { 1, 8 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "break" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = { }, .len = 0 };
    
    init_cp(cp_break, BREAK, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_break;
}

code_pattern* init_aid_cp_case() {
    code_pattern* cp_case = create_and_init_code_pattern();

    cp_case->name = aapts("case");
    i_arr bin_tokens = { .arr = { }, .len = 0 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "case", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = { "case_value" }, .len = 1 };
    
    init_cp(cp_case, CASE_AID, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_case;
}

code_pattern* init_aid_cp_default_case() {
    code_pattern* cp_case = create_and_init_code_pattern();

    cp_case->name = aapts("default case");
    i_arr bin_tokens = { .arr = { }, .len = 0 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "default" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = { }, .len = 0 };
    
    init_cp(cp_case, CASE_AID, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_case;
}

code_pattern* init_cp_else() {
    code_pattern* cp_else = create_and_init_code_pattern();

    cp_else->name = aapts("else_block");
    cp_else->bin_extra_token_num = -1; // 0xfffffffc is extra for recognition
    i_arr bin_tokens = { .arr = { 1, 1, 0xfffffffc }, .len = 3 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "else" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = { }, .len = 0 };
    
    init_cp(cp_else, ELSE_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_else;
}

code_pattern* init_cp_return() {
    code_pattern* cp_ret = create_and_init_code_pattern();

    cp_ret->name = aapts("exit script");
    i_arr bin_tokens = { .arr = { 1, 2 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = { "exit_script" }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };
    s_arr var_names = { .arr = { }, .len = 0 };
    
    init_cp(cp_ret, RET, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_ret;
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
    code_patterns[5] = init_cp_while();
    code_patterns[6] = init_cp_else();
    code_patterns[7] = init_cp_script_call();
    code_patterns[8] = init_cp_switch_case();
    code_patterns[9] = init_cp_break();
    code_patterns[10] = init_cp_var_assignment();
    code_patterns[11] = init_cp_var_assignment_or();
    code_patterns[12] = init_cp_var_assignment_minus();
    code_patterns[13] = init_cp_var_assignment_plus();
    code_patterns[14] = init_cp_var_assignment_and();
    code_patterns[15] = init_cp_var();
    code_patterns[16] = init_cp_structure_ptr();
    code_patterns[17] = init_cp_return();
    code_patterns[18] = init_cp_data_index_ptr();
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
    expr_func_call->name = aapts("func expr call");

    i_arr bin_tokens = { .arr = {0, 3, VAR, VAR}, .len = 4 };
    i_arr bin_var_pos = { .arr = {2, 3}, .len = 2 };
    s_arr asm_tokens = { .arr = { "call", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"func_name", "num_of_params"}, .len = 2 };
    
    init_expr(expr_func_call, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr_func_call;
}

expr_pattern* init_expr_neg_func_call() {
    expr_pattern* expr_func_call = (expr_pattern*) malloc(sizeof(code_pattern));
    expr_func_call->type = FUNCTION;
    expr_func_call->name = aapts("func expr call with negated return value");

    i_arr bin_tokens = { .arr = {0, 0x20000003, VAR, VAR}, .len = 4 };
    i_arr bin_var_pos = { .arr = {2, 3}, .len = 2 };
    s_arr asm_tokens = { .arr = { "call", "!", S_VAR }, .len = 3 };
    i_arr asm_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr var_names = { .arr = {"func_name", "num_of_params"}, .len = 2 };
    
    init_expr(expr_func_call, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr_func_call;
}

expr_pattern* init_expr_gamevar() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = GAME_VAR;
    expr->name = aapts("game variable");

    i_arr bin_tokens = { .arr = { 0, VAR, VAR, VAR, 4 }, .len = 5 };
    i_arr bin_var_pos = { .arr = { 1, 2, 3 }, .len = 3 };
    s_arr asm_tokens = { .arr = { "gamevar", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_plus_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("plus operator");

    i_arr bin_tokens = { .arr = { 3, 1 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "+"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_minus_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("minus operator");

    i_arr bin_tokens = { .arr = { 3, 2 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "-"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_times_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("multiply operator");

    i_arr bin_tokens = { .arr = { 3, 3 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "*"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_division_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("division operator");


    i_arr bin_tokens = { .arr = { 3, 4 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "/"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_modulo_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("modulo operator");

    i_arr bin_tokens = { .arr = { 3, 5 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "\%"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_eq_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("equals operator");

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
    expr->name = aapts("not equals operator");

    i_arr bin_tokens = { .arr = { 4, 3 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "!="  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_lt_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("less than operator");

    i_arr bin_tokens = { .arr = { 4, 4 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "<"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_gt_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("greater than operator");

    i_arr bin_tokens = { .arr = { 4, 5 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  ">"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_ge_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts(">= operator");

    i_arr bin_tokens = { .arr = { 4, 1 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  ">="  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_le_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("<= operator");

    i_arr bin_tokens = { .arr = { 4, 0 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "<="  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_and_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("and operator");

    i_arr bin_tokens = { .arr = { 3, 7 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "&"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_or_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = OPERATOR;
    expr->name = aapts("or operator");

    i_arr bin_tokens = { .arr = { 3, 6 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "|"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_logical_and_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = MUL_EXP_OP;
    expr->name = aapts("&& operator");

    i_arr bin_tokens = { .arr = { 6, 0 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "&&"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_logical_or_op() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = MUL_EXP_OP;
    expr->name = aapts("|| operator");

    i_arr bin_tokens = { .arr = { 6, 1 }, .len = 2 };
    i_arr bin_var_pos = { .arr = { }, .len = 0 };
    s_arr asm_tokens = { .arr = {  "||"  }, .len = 1 };
    i_arr asm_var_pos = { .arr = { }, .len = 0 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_var_ptr() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = VAR_PTR;
    expr->name = aapts("var ptr");

    i_arr bin_tokens = { .arr = { 0, 6, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "var", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_data_index_ptr() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = DATA_INDEX_PTR;
    expr->name = aapts("data index ptr");

    i_arr bin_tokens = { .arr = { 0, 5, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "@", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_func_ptr() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = FUNC_PTR;
    expr->name = aapts("function ptr");

    i_arr bin_tokens = { .arr = { 0, 8, VAR, 2 }, .len = 4 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "func", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_data_neg_index_ptr() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = DATA_INDEX_PTR;
    expr->name = aapts("data index ptr");

    i_arr bin_tokens = { .arr = { 0, 0x20000005, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "!", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_neg_var_ptr() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = VAR_PTR;
    expr->name = aapts("var ptr");

    i_arr bin_tokens = { .arr = { 0, 0x20000006, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "var", "!", S_VAR }, .len = 3 };
    i_arr asm_var_pos = { .arr = { 2 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_addr_of_var_ptr() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = ADDROF_VAR_PTR;
    expr->name = aapts("address of var ptr");

    i_arr bin_tokens = { .arr = { 0, 7, VAR }, .len = 3 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { "var", "&", S_VAR }, .len = 3 };
    i_arr asm_var_pos = { .arr = { 2 }, .len = 1 };

    init_expr(expr, bin_tokens, bin_var_pos, asm_tokens, asm_var_pos);

    return expr;
}

expr_pattern* init_expr_int() {
    expr_pattern* expr = w_malloc(sizeof(expr_pattern));
    expr->type = INTEGER;
    expr->name = aapts("integer");

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
    expr_patterns[1] = init_expr_addr_of_var_ptr();
    expr_patterns[2] = init_expr_var_ptr();
    expr_patterns[3] = init_expr_data_index_ptr();
    expr_patterns[4] = init_expr_data_neg_index_ptr();
    expr_patterns[5] = init_expr_neg_var_ptr();
    // math ops(binary)
    expr_patterns[6] = init_expr_plus_op();
    expr_patterns[7] = init_expr_minus_op();
    expr_patterns[8] = init_expr_times_op();
    expr_patterns[9] = init_expr_division_op();
    expr_patterns[10] = init_expr_modulo_op();
    // control op(binary)
    expr_patterns[11] = init_expr_eq_op();
    expr_patterns[12] = init_expr_ge_op();
    expr_patterns[13] = init_expr_gt_op();
    expr_patterns[14] = init_expr_le_op();
    expr_patterns[15] = init_expr_and_op();
    expr_patterns[16] = init_expr_or_op();
    expr_patterns[17] = init_expr_lt_op();
    expr_patterns[18] = init_expr_ne_op();
    // game
    expr_patterns[19] = init_expr_neg_func_call();
    expr_patterns[20] = init_expr_func_call();
    expr_patterns[21] = init_expr_func_ptr();
    expr_patterns[22] = init_expr_gamevar();
    // special operators
    expr_patterns[23] = init_expr_logical_and_op();
    expr_patterns[24] = init_expr_logical_or_op();
}

bool is_var_pos_expr(void* pattern, mode m, int index) {
    expr_pattern* p = (expr_pattern*) pattern;        

    int var_num = (m == MODE_BIN) ? p->bin_var_num : p->asm_var_num;
    int* var_pos = (m == MODE_BIN) ? p->bin_var_pos : p->asm_var_pos;

    // printf("vpos: %d\n", *var_pos);
    for(int k=0; k < var_num; k++) {
        // printf("var_pos: %d, index: %d\n", var_pos[k], index);
        if(var_pos[k] == index) {
            return true;
        }
    }
    return false;
}

bool is_var_pos(void* pattern, p_type pattern_type, mode m, int index) {
    code_pattern* p = (code_pattern*) pattern;        

    if(pattern_type == EXPRESSION_TYPE) {
        return is_var_pos_expr(pattern, m, index);
    }

    int var_num = (m == MODE_BIN) ? p->bin_var_num : p->asm_var_num;
    int* var_pos = (m == MODE_BIN) ? p->bin_var_pos : p->asm_var_pos;

    // printf("vpos: %d\n", *var_pos);
    for(int k=0; k < var_num; k++) {
        // printf("var_pos: %d, index: %d\n", var_pos[k], index);
        if(var_pos[k] == index) {
            return true;
        }
    }
    return false;
}
