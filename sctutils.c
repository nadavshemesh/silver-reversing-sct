#include "sctutils.h"

game_fun** game_functions;
code_pattern** code_patterns;
expr_pattern** expr_patterns;

void print_bin_tokens(int* tokens, int length) {
    for(int i=0; i<length; i++) {
        printf("%08x\n", tokens[i]);
    }
}

void print_bin_token_seg(int* tokens, int from_row, int to_row, int cols) {
    int* token = tokens;
    for(int i=from_row*cols; i < to_row*cols; i++) {
        if(i != from_row*cols && i%cols == 0) printf("\n");
        if(&(tokens[i]) == token)
            printf(ANSI_COLOR_RED "%08x " ANSI_COLOR_RESET, tokens[i]);
        else
            printf("%8.8x ", tokens[i]);
    }
    printf("\n");
}

bool cmp_token(void* a_token_ptr, void* b_token_ptr, mode m) {
    if(m == MODE_BIN) {
        int* a_token = (int*) a_token_ptr;
        int* b_token = (int*) b_token_ptr;
        if(*a_token == *b_token) {
            // printf("%08x == %08x\n", *a_token, *b_token);
            return true;
        }

        // printf("%08x != %08x\n", *a_token, *b_token);
        return false;
    }
    char** a_token = (char**) a_token_ptr;
    char** b_token = (char**) b_token_ptr;
    bool res = strcmp(*a_token, *b_token);
    
    if(res) {
        // printf("%s != %s\n", *a_token, *b_token);
        return true;
    }
    // printf("%s != %s\n", *a_token, *b_token);
    return false;
}

void print_token_area_details(void* token_pos, mode m) {
    if(token_pos != NULL) {
        if(m == MODE_BIN) {
            int* token = (int*) token_pos;
            print_bin_token_seg(token, -2, 2, 5);
        } else {
            char** token = (char**) token_pos;
            printf("[ %s %s %s %s ]\n", token-2, token-1, token, token+1);
        }
    }
    exit(1);
}

cp_cmp_result identify_cp(void** token_pos_ptr, mode m) {
    cp_cmp_result res = { .is_identified = false, .match = NULL};
    void* token;

    // compare each pattern with tokens
    for(int i=0; i < CODE_PATTERNS_NUM; i++) {
        code_pattern* cp = code_patterns[i];
        token = *token_pos_ptr;

        if(cp != NULL) {
           int token_num = (m == MODE_BIN) ? cp->bin_token_num : cp->asm_token_num;
           int var_num = (m == MODE_BIN) ? cp->bin_var_num : cp->asm_var_num;
           void* tokens = (m == MODE_BIN) ? (void*) cp->bin_tokens : (void*) cp->asm_tokens;
           int vars[var_num];
           if(m == MODE_ASM) { char* vars[var_num]; }
           int var_i = 0;
           int eq_goal = token_num - var_num;
           int eq_tokens = 0;
           // compare const tokens
           for(int j=0; j < token_num; j++) {
               if(!is_var_pos(cp, CODE_TYPE, m, j)) {
                   if(!cmp_token(tokens, token, m))  {
                       break; // not a match
                   }
                    eq_tokens++;
                } else {
                    if(m == MODE_BIN) { printf("var found: %08x\n", *((int*)token)); vars[var_i] = *((int*) token); }
                    else if(m == MODE_ASM) {  vars[var_i] = *((char**) tokens); }
                    var_i++;
                }
                tokens += sizeof(int);
                token += sizeof(int);
           }
           if(eq_tokens == eq_goal) {
                res.is_identified = true;
                res.match = cp;
                res.tokens_pos = token;
                *token_pos_ptr = token;
                res.vars = w_malloc(var_num*sizeof(int));
                memcpy(res.vars, vars, var_num*sizeof(int));
                return res;
           }
       }
    }
    print_err("Error, code pattern not recognized.", 2);
    print_token_area_details(token, m);

    return res;
}

expr_cmp_result identify_expr(void** token_pos_ptr, mode m) {
    expr_cmp_result res = { .is_identified = false, .match = NULL};
    void* token;

    // compare each pattern with tokens
    for(int i=0; i < EXPR_PATTERNS_NUM; i++) {
        expr_pattern* expr = expr_patterns[i];

        if(expr != NULL) {
           token = *token_pos_ptr;
           int token_num = (m == MODE_BIN) ? expr->bin_token_num : expr->asm_token_num;
           int var_num = (m == MODE_BIN) ? expr->bin_var_num : expr->asm_var_num;
           void* tokens = (m == MODE_BIN) ? (void*) expr->bin_tokens : (void*) expr->asm_tokens;
           int vars[var_num];
           if(m == MODE_ASM) { char* vars[var_num]; }
           
           int var_i = 0;
           int eq_goal = token_num - var_num;
           int eq_tokens = 0;
           // compare const tokens
           for(int j=0; j < token_num; j++) {
               if(!is_var_pos(expr, EXPRESSION_TYPE, m, j)) {
                   if(!cmp_token(tokens, token, m))  {
                       break; // not a match
                   }
                    eq_tokens++;
                } else {
                    if(m == MODE_BIN) { vars[var_i] = *((int*) token); }
                    else if(m == MODE_ASM) {  vars[var_i] = *((char**) token); }
                    var_i++;
                }
                tokens += sizeof(int);
                token += sizeof(int);
           }
           if(eq_tokens == eq_goal) {
                res.is_identified = true;
                res.match = expr;
                res.tokens_pos = token;
                *token_pos_ptr = token;
                res.vars = w_malloc(var_num*sizeof(int));
                memcpy(res.vars, vars, var_num*sizeof(int));
                return res;
           }
       }
    }
    print_err("Error, expression not recognized.", 2);
    print_token_area_details(token, m);
    return res;
}

int read_expression_size(void** tokens_pos_ptr, mode m, bool with_prologue) { 
    if(m == MODE_BIN) {
        int* token = *tokens_pos_ptr; 
        if(!with_prologue || *token == 0xfffffffd) {
            int size = (with_prologue) ? *(token+1) : *token;
            if(size > 0) {
                *tokens_pos_ptr += (with_prologue) ? 8 : 4;
                return size;
            }
            print_err("Error, No expr obj declaration and size.", -2);
            print_token_area_details(*tokens_pos_ptr, m);
        } 

        print_err("Error, No expr obj declaration and size.", -2);
        print_token_area_details(*tokens_pos_ptr, m);
    }
}

expression* read_expression(void** tokens_pos_ptr, mode m, bool with_prologue, sct_f* sf) {
    expression* exp = w_malloc(sizeof(expression));

    if(m == MODE_BIN) {
        int exprs_num = read_expression_size(tokens_pos_ptr, m, with_prologue);
        printf("expr size: %d\n", exprs_num);
        expr_obj exprs[exprs_num];
        // Read exprs
        for(int i=0; i < exprs_num; i++) {
            expr_cmp_result res = identify_expr(tokens_pos_ptr, m);
            if(res.is_identified) {
                char msg[256];
                sprintf(msg, "Found expr pattern '%s'.", res.match->name);
                print_success(msg);
                *tokens_pos_ptr = res.tokens_pos;
                expr_obj* eo = create_expr_obj(res.match, res.vars, m, tokens_pos_ptr, sf);
                exprs[i] = *eo;
            } else { print_err_and_exit("unidentified expression.", -4); }
        }
        exp->expr_objs_len = exprs_num;
        exp->expr_objs = w_malloc(exprs_num*sizeof(expr_obj));
        memcpy(exp->expr_objs, exprs, exprs_num*sizeof(expr_obj));
    }

    return exp;
}

expr_obj* read_function_call_expr(expr_pattern* expr_p, void* vars, mode m, void** tokens_pos_ptr, sct_f* sf) {
    expr_obj* e_obj = create_and_init_expr_obj();

    if(m == MODE_BIN) {
        int func_num = ((int*) vars)[0];
        int params_num = ((int*) vars)[1];
        printf("game_func: %x, params_num(exprs): %d\n", func_num, params_num);

        game_fun* gf = game_functions[func_num];
        if(gf == NULL) { 
            char err[256];
            sprintf(err, "ERROR, game func %d not found.\n", func_num);
            print_err(err, -3);
        }
        int exp_nodes_len = 0;
        node** exp_nodes = w_malloc(sizeof(node*));
        *exp_nodes = w_malloc(sizeof(node));
        for(int i=0; i < params_num; i++) {
            expression* exp;
            exp = read_expression(tokens_pos_ptr, m, true, sf);
            node* new_node = create_node(exp);
            if(exp_nodes_len == 0) {
                *exp_nodes = new_node;
            } else {
                insert_node(exp_nodes, new_node);
            }
            exp_nodes_len++;
        }

        e_obj->expression_nodes = w_malloc(sizeof(node*));
        *e_obj->expression_nodes = w_malloc(sizeof(node));
        e_obj->expression_node_num = exp_nodes_len;
        e_obj->expression_nodes = exp_nodes;
        // print_expression((*exp_nodes)->item);

        char* asm_vars = { gf->name };
        e_obj->expr_p = expr_p;
        e_obj->bin_vars = w_malloc(expr_p->bin_var_num*sizeof(int));
        e_obj->asm_vars = w_malloc(expr_p->asm_var_num*MAX_FUNC_NAME);
        memcpy(e_obj->bin_vars, vars, sizeof(e_obj->bin_vars));
        memcpy(e_obj->asm_vars, &asm_vars, sizeof(e_obj->asm_vars));
    }

    return e_obj;
}

expr_obj* create_expr_obj(expr_pattern* expr, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    expr_obj* eo = create_and_init_expr_obj();

    int bin_var_num = expr->bin_var_num;
    int asm_var_num = expr->asm_var_num;
    if(m == MODE_BIN) {
        int data_var = (vars == NULL) ? 0 : ((int*)vars)[0];
        data_obj* data;

        eo->bin_vars = w_malloc(bin_var_num*sizeof(int*));
        memcpy(eo->bin_vars, vars, bin_var_num*sizeof(int*));

        switch(expr->type) {
            case INTEGER:
                char c[10];
                int i[1] = { data_var };
                sprintf(c, "%d", data_var);
                data = w_malloc(sizeof(data_obj));
                data->name = w_malloc(10);
                data->byte_size = 4;
                data->references = 0;
                data->bin_data = w_malloc(sizeof(int*));
                data->asm_data = w_malloc(sizeof(char*));
                memcpy(data->name, &c, sizeof(data->name));
                memcpy(data->asm_data, &c, sizeof(data->asm_data));
                memcpy(data->bin_data, i, sizeof(data->bin_data));
                break;

            case DATA_PTR:
                data = get_data_obj_by_id(data_var, sf);
                break;

            case VAR_PTR:
                data = get_data_obj_by_id(data_var, sf);
                break;
            case FUNCTION:
                expr_obj* exp_func = read_function_call_expr(expr, vars, m, token_pos_ptr, sf);
                return exp_func;
                break;
        
        default:
            data = NULL;
        }

        char* name = (data != NULL) ? data->name : "";
        char* asm_vars = { name };
        eo->asm_vars = w_malloc(asm_var_num*sizeof(char*));
        memcpy(eo->asm_vars, &asm_vars, asm_var_num*sizeof(int*));

        eo->expr_p = expr;
        eo->data = data;
    }

    return eo;
}

code_obj* read_code_block(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        c_obj->cp = cp;
        c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));
        c_obj->code_nodes = w_malloc(sizeof(node*));
        *c_obj->code_nodes = w_malloc(sizeof(node));

        int size_in_words = ((int*)vars)[0]-2;
        int bin_vars[1] = { size_in_words };
        memcpy(c_obj->bin_vars, bin_vars, cp->bin_var_num*sizeof(int));

        int* token_ptr = *token_pos_ptr;
        int* block_token_pos_ptr = *token_pos_ptr;
        int tokens_read = (block_token_pos_ptr - token_ptr);
        while(tokens_read < size_in_words) {
            printf("Block tokens read: %d\n", tokens_read);
            cp_cmp_result res = identify_cp(token_pos_ptr, MODE_BIN);
            if(res.is_identified) {
                char msg[256];
                sprintf(msg, "Found pattern '%s'.\n", res.match->name);
                print_success(msg);
                obj_and_token_ptr oatp = create_code_obj(res.match, res.vars, MODE_BIN, token_pos_ptr, sf);
                if(oatp.type == OBJ_CODE) {
                    code_obj* co = (code_obj*) oatp.obj;
                    // print_code_obj(co);
                    // print_asm_code_obj(co);
                    node* code_node = create_node(co);
                    if(c_obj->code_nodes_num == 0){
                        *c_obj->code_nodes = code_node;
                    } else {
                        insert_node(c_obj->code_nodes, create_node(co));
                    }
                    c_obj->code_nodes_num++;
                }
                *token_pos_ptr = oatp.token_ptr;
                block_token_pos_ptr = *token_pos_ptr;
                tokens_read = (block_token_pos_ptr - token_ptr);
            }
        }
    }

    return c_obj;
}

code_obj* read_code_block_cases(code_pattern* cp, void* vars, int* cases, int cases_num, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        c_obj->cp = cp;
        c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));
        c_obj->code_nodes = w_malloc(sizeof(node*));
        *c_obj->code_nodes = w_malloc(sizeof(node));

        int size_in_words = ((int*)vars)[0]-2;
        int bin_vars[1] = { size_in_words };
        memcpy(c_obj->bin_vars, bin_vars, cp->bin_var_num*sizeof(int));

        int* token_ptr = *token_pos_ptr;
        int* block_token_pos_ptr = *token_pos_ptr;
        int tokens_read = (block_token_pos_ptr - token_ptr);

        int case_index = 0;
        // init current case (first case)
        code_obj* current_case = create_and_init_c_obj();
        code_pattern* case_cp = init_aid_cp_case();
        current_case->cp = case_cp;

        char num_s[30];
        sprintf(num_s, " %d", cases[case_index]);
        char* asm_vars[1] = { aapts(num_s) };
        current_case->asm_vars = w_malloc((current_case->cp->asm_var_num)*sizeof(char*));
        memcpy(current_case->asm_vars, &asm_vars, sizeof(current_case->asm_vars));

        current_case->code_nodes = w_malloc(sizeof(node*));
        *current_case->code_nodes = w_malloc(sizeof(node));

        while(tokens_read < size_in_words) {
            printf("Block tokens read: %d\n", tokens_read);
            cp_cmp_result res = identify_cp(token_pos_ptr, MODE_BIN);
            if(res.is_identified) {
                char msg[256];
                sprintf(msg, "Found pattern '%s'.\n", res.match->name);
                print_success(msg);
                obj_and_token_ptr oatp = create_code_obj(res.match, res.vars, MODE_BIN, token_pos_ptr, sf);
                if(oatp.type == OBJ_CODE) {
                    code_obj* co = (code_obj*) oatp.obj;
                    // print_code_obj(co);
                    // print_asm_code_obj(co);
                    node* code_node = create_node(co);
                    if(current_case->code_nodes_num == 0){
                        *current_case->code_nodes = code_node;
                    } else {
                        insert_node(current_case->code_nodes, create_node(co));
                    }
                    current_case->code_nodes_num++;
                }
                *token_pos_ptr = oatp.token_ptr;
                block_token_pos_ptr = *token_pos_ptr;
                tokens_read = (block_token_pos_ptr - token_ptr);
                // After break add case code_obj to main block
                if(res.match->type == BREAK && case_index < cases_num) {
                    node* code_node = create_node(current_case);
                    if(c_obj->code_nodes_num == 0){
                        *c_obj->code_nodes = code_node;
                    } else {
                        insert_node(c_obj->code_nodes, create_node(current_case));
                    }
                    c_obj->code_nodes_num++;

                    // init current case (next case)
                    case_index++;
                    current_case = create_and_init_c_obj();
                    current_case->cp = case_cp;

                    sprintf(num_s, " %d", cases[case_index]);
                    char* asm_vars[1] = { aapts(num_s) };
                    current_case->asm_vars = w_malloc((current_case->cp->asm_var_num)*sizeof(char**));
                    memcpy(current_case->asm_vars, asm_vars, sizeof(current_case->asm_vars));
                    
                    current_case->code_nodes = w_malloc(sizeof(node*));
                    *current_case->code_nodes = w_malloc(sizeof(node));
                }
            }
        }
    }

    return c_obj;
}


code_obj* read_function_call(code_pattern* cp, void* vars, mode m, void** tokens_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        int func_num = ((int*) vars)[0];
        int params_num = ((int*) vars)[1];
        printf("game_func: %x, params_num(exprs): %d\n", func_num, params_num);

        game_fun* gf = game_functions[func_num];
        if(gf == NULL) { 
            char err[256];
            sprintf(err, "ERROR, game func %d not found.\n", func_num);
            print_err(err, -3);
        }
        int exp_nodes_len = 0;
        node** exp_nodes = w_malloc(sizeof(node*));
        *exp_nodes = w_malloc(sizeof(node));
        for(int i=0; i < params_num; i++) {
            expression* exp;
            exp = read_expression(tokens_pos_ptr, m, true, sf);
            node* new_node = create_node(exp);
            if(exp_nodes_len == 0) {
                *exp_nodes = new_node;
            } else {
                insert_node(exp_nodes, new_node);
            }
            exp_nodes_len++;
        }

        c_obj->expression_node_num = exp_nodes_len;
        c_obj->expression_nodes = exp_nodes;
        // print_expression((*exp_nodes)->item);

        char* asm_vars = { gf->name };
        c_obj->cp = cp;
        c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));
        c_obj->asm_vars = w_malloc(cp->asm_var_num*MAX_FUNC_NAME);
        memcpy(c_obj->bin_vars, vars, sizeof(c_obj->bin_vars));
        memcpy(c_obj->asm_vars, &asm_vars, sizeof(c_obj->asm_vars));
    }

    return c_obj;
}

code_obj* read_if_statement(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        expression* exp = read_expression(token_pos_ptr, m, false, sf);
        c_obj->cp = cp;

        code_obj* code_block_obj;
        cp_cmp_result res = identify_cp(token_pos_ptr, m);
        if(res.is_identified && res.match->type == CODE_BLOCK) {
            *token_pos_ptr = res.tokens_pos;
            obj_and_token_ptr oatp = create_code_obj(res.match, res.vars, m, token_pos_ptr, sf);
            code_block_obj = oatp.obj;
            *token_pos_ptr = oatp.token_ptr;
        } else {
            print_err_and_exit("Error, no code_block after if statement.", -2);
        }

        node* c_block_node = create_node(code_block_obj);
        c_obj->code_nodes = w_malloc(sizeof(node*));
        *c_obj->code_nodes = w_malloc(sizeof(node));
        *c_obj->code_nodes = c_block_node;
        // print_code_obj(code_block_obj);
        // print_code_obj(c_block_node->item);
        c_obj->code_nodes_num = 1;

        c_obj->asm_vars = NULL;
        c_obj->bin_vars = NULL;

        node** exp_nodes = w_malloc(sizeof(node*));
        *exp_nodes = w_malloc(sizeof(node));
        node* new_node = create_node(exp);
        *exp_nodes = new_node;
        c_obj->expression_nodes = exp_nodes;
        c_obj->expression_node_num = 1;
    }

    return c_obj;
}

code_obj* read_else_statement(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        c_obj->cp = cp;

        code_obj* code_block_obj;
        *token_pos_ptr -= 4; // move back before -0xfffffffc
        cp_cmp_result res = identify_cp(token_pos_ptr, m);
        if(res.is_identified && res.match->type == CODE_BLOCK) {
            *token_pos_ptr = res.tokens_pos;
            obj_and_token_ptr oatp = create_code_obj(res.match, res.vars, m, token_pos_ptr, sf);
            code_block_obj = oatp.obj;
            *token_pos_ptr = oatp.token_ptr;
        } else {
            print_err_and_exit("Error, no code_block after if statement.", -2);
        }

        node* c_block_node = create_node(code_block_obj);
        c_obj->code_nodes = w_malloc(sizeof(node*));
        *c_obj->code_nodes = w_malloc(sizeof(node));
        *c_obj->code_nodes = c_block_node;
        // print_code_obj(code_block_obj);
        // print_code_obj(c_block_node->item);
        c_obj->code_nodes_num = 1;

        c_obj->asm_vars = NULL;
        c_obj->bin_vars = NULL;
    }

    return c_obj;
}

code_obj* read_assignment(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        expression* exp = read_expression(token_pos_ptr, m, true, sf);
        c_obj->cp = cp;

        c_obj->code_nodes_num = 0;
        c_obj->code_nodes = NULL;
        c_obj->asm_vars = NULL;
        c_obj->bin_vars = NULL;

        node** exp_nodes = w_malloc(sizeof(node*));
        *exp_nodes = w_malloc(sizeof(node));
        node* new_node = create_node(exp);
        *exp_nodes = new_node;
        c_obj->expression_nodes = exp_nodes;
        c_obj->expression_node_num = 1;
    }

    return c_obj;
}

char* determine_room_var_ptr(int first_offset, int second_offset) {
        switch(first_offset) {
            case 9: // ptr to room flags
                switch(second_offset) {
                    case 0x08:  // Room state
                        return aapts("room_state");
                    break;
                    case 0x2c:  // Room cleared flag
                        return aapts("is_room_cleared");
                    break;
                
                default:
                    break;
                }
                break;
            // case 00 - ptr to last game object 
            // cases 00-07 - ptr to last game object data attribute
            // case 10 - ptr to last game object data 
        }
        char str[50];
        sprintf(str, "game_var_%02x_%02x", first_offset, second_offset);
        return aapts(str);
}

code_obj* read_room_var_ptr(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        c_obj->cp = cp;

        int first_offset = (((int*)vars))[0];
        int second_offset = ((int*)(vars))[1];
        char* asm_vars[1] = { determine_room_var_ptr(first_offset, second_offset) };
        int bin_vars[2] = { first_offset, second_offset };

        c_obj->asm_vars = w_malloc(cp->asm_var_num*sizeof(char*));
        c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));

        memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
        memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));
    }

    return c_obj;
}

code_obj* read_var_ptr(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        c_obj->cp = cp;
                
        int data_var = (((int*)vars))[0];
        data_obj* data;
        data = get_data_obj_by_id(data_var, sf);
        char* name = (data != NULL) ? data->name : "";
        char* asm_vars = { name };
        int bin_vars[1] = { data_var };
        c_obj->asm_vars = w_malloc(cp->asm_var_num*sizeof(char*));
        c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));

        c_obj->data = data;
        memcpy(c_obj->asm_vars, &asm_vars, sizeof(c_obj->asm_vars));
        memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));
    }

    return c_obj;
}

code_obj* read_script_call(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    if(m == MODE_BIN) {
        c_obj->cp = cp;
        int script_number = ((int*)vars)[0];
        char name[MAX_FUNC_NAME];
        sprintf(name, "SCRIPT_%d", script_number);

        int bin_var_num = cp->bin_var_num;
        int asm_var_num = cp->asm_var_num;
        char* asm_vars[1] = { aapts(name) };
        int bin_vars[1] = { script_number };
        c_obj->asm_vars = w_malloc(asm_var_num*sizeof(char*));
        c_obj->bin_vars = w_malloc(bin_var_num*sizeof(int));

        c_obj->code_nodes_num = 0;
        c_obj->code_nodes = NULL;
        
        memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
        memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

        c_obj->expression_node_num = 0;
        c_obj->expression_nodes = NULL;
    }

    return c_obj;
}

code_obj* read_switch_case(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = w_malloc(sizeof(code_obj));

    if(m == MODE_BIN) {
        c_obj->cp = cp;
        int cases_num = ((int*)vars)[0];
        if(cases_num <= 0) print_err_and_exit("Error, switch without cases.", -2);
        int padding_to_block_ptrs = 69;
        int padding_to_block = 129;

        // read cases values
        int* token_pos = *token_pos_ptr;
        token_pos += 1;
        int cases[cases_num];
        int case_ptrs[cases_num];
        memcpy(cases, token_pos, cases_num*sizeof(int));

        // for(int i=0; i < cases_num; i++) {
        //     printf("%d ", cases[i]);
        // }
        // printf("\n");

        // read case blocks (no need in bin_mode)
        // token_pos += padding_to_block_ptrs-cases_num;

        // read expression
        // token_pos += (padding_to_block-cases_num);
        token_pos += padding_to_block;
        // print_token_area_details(token_pos, m);
        *token_pos_ptr = token_pos;
        expression* exp = read_expression(token_pos_ptr, m, false, sf);
        c_obj->expression_node_num = 1;
        node** exp_nodes = w_malloc(sizeof(node*));
        *exp_nodes = w_malloc(sizeof(node));
        node* new_node = create_node(exp);
        *exp_nodes = new_node;
        c_obj->expression_nodes = exp_nodes;

        // copy bin & asm vars (cases values)
        // int bin_var_num = cases_num;
        // int asm_var_num = cases_num;
        // cp->bin_var_num = cases_num;
        char* asm_vars[cases_num];
        for(int i=0; i < cases_num; i++) {
            char str[10];
            sprintf(str, "%d", cases[i]);
            asm_vars[i] = str;
        }
        // c_obj->asm_vars = w_malloc(asm_var_num*sizeof(char*));
        // c_obj->bin_vars = w_malloc(cases_num*sizeof(int));

        // memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
        // memcpy(c_obj->bin_vars, cases, cases_num*sizeof(int));
        // for(int i=0; i < cases_num; i++) {
        //     printf("%d ", (c_obj->bin_vars)[i]);
        // }
        // printf("\n");

        // read code block
        code_obj* code_block_obj;
        cp_cmp_result res = identify_cp(token_pos_ptr, m);
        if(res.is_identified && res.match->type == CODE_BLOCK) {
            *token_pos_ptr = res.tokens_pos;
            code_obj* c_obj;
            code_block_obj = read_code_block_cases(res.match, res.vars, cases, cases_num, m, token_pos_ptr, sf);
        } else {
            print_err_and_exit("Error, no code_block after switch-case.", -2);
        }

        node* c_block_node = create_node(code_block_obj);
        c_obj->code_nodes = w_malloc(sizeof(node*));
        *c_obj->code_nodes = w_malloc(sizeof(node));
        *c_obj->code_nodes = c_block_node;
        // print_code_obj(c_block_node->item);
        c_obj->code_nodes_num = 1;

    }

    return c_obj;
}

code_obj* read_default(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = w_malloc(sizeof(code_obj));

    if(m == MODE_BIN) {
        c_obj->cp = cp;

        c_obj->code_nodes_num = 0;
        c_obj->code_nodes = NULL;
        c_obj->asm_vars = NULL;
        c_obj->bin_vars = NULL;

        c_obj->expression_node_num = 0;
        c_obj->expression_nodes = NULL;
    }

    return c_obj;
}

obj_and_token_ptr create_code_obj(code_pattern* cp, void* vars, mode m, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj;

    switch(cp->type) {
        case CODE_BLOCK:
            c_obj = read_code_block(cp, vars, m, token_pos_ptr, sf);
            break;
        case IF_STATEMENT:
            c_obj = read_if_statement(cp, vars, m, token_pos_ptr, sf);
            break;
        case ELSE_STATEMENT:
            c_obj = read_else_statement(cp, vars, m, token_pos_ptr, sf);
            break;
        case SWITCH:
            c_obj = read_switch_case(cp, vars, m, token_pos_ptr, sf);
            break;
        case FUNCTION_CALL:
            c_obj = read_function_call(cp, vars, m, token_pos_ptr, sf);
            break;
        case SCRIPT_CALL:
            c_obj = read_script_call(cp, vars, m, token_pos_ptr, sf);
            break;
        case ROOM_VAR_PTR:
            c_obj = read_room_var_ptr(cp, vars, m, token_pos_ptr, sf);
            break;
        case CP_VAR_PTR:
            c_obj = read_var_ptr(cp, vars, m, token_pos_ptr, sf);
            break;
        case ASSIGNMENT:
            c_obj = read_assignment(cp, vars, m, token_pos_ptr, sf);
            break;
        default:
            c_obj = read_default(cp, vars, m, token_pos_ptr, sf);
    }

    void* token_ptr = *token_pos_ptr;
    obj_and_token_ptr oatp = { .obj = c_obj, .type = OBJ_CODE, .token_ptr = token_ptr };
    return oatp;
}

script* disasm_script(int script_num, sct_f* sf) {
    int script_offset = get_script_off(script_num, sf);
    fseek(sf->file, script_offset+8, SEEK_SET);

    script* script = w_malloc(sizeof(script));
    script->number = script_num;
    script->code_nodes_num = 0;
    script->code_nodes = w_malloc(sizeof(node*));
    *script->code_nodes = w_malloc(sizeof(node));
    printf("node ptr: %08x\n", script->code_nodes);
    script->script_code_ptr = w_malloc(sizeof(void*));

    int script_len;
    fread(&script_len, 4, 1, sf->file);
    script_len -= 2;
    script->size_in_words = script_len;
    printf("script_offset: %x, script_len: %x (%x bytes)\n", script_offset, script_len, script_len*4);

    int bin_tokens[script_len];
    *script->script_code_ptr = bin_tokens;
    fread(&bin_tokens, 4, script_len, sf->file);

    int* token_ptr = *(script->script_code_ptr);
    int tokens_read = token_ptr - bin_tokens;
    while(tokens_read < script_len) {
        printf("Tokens read: %d\n", tokens_read);
        cp_cmp_result res = identify_cp(script->script_code_ptr, MODE_BIN);
        if(res.is_identified) {
            char msg[256];
            sprintf(msg, "Found pattern '%s'.\n", res.match->name);
            print_success(msg);
            obj_and_token_ptr oatp = create_code_obj(res.match, res.vars, MODE_BIN, script->script_code_ptr, sf);
            if(oatp.type == OBJ_CODE) {
                code_obj* co = (code_obj*) oatp.obj;
                // print_code_obj(co);
                // print_asm_code_obj(co);

                node* code_node = create_node(co);
                if(script->code_nodes_num == 0) {
                    *(script->code_nodes) = code_node;
                } else {
                    insert_node(script->code_nodes, code_node);
                }
                // print_code_obj((*script->code_nodes)->item);
                script->code_nodes_num++;
                // print_asm_script(script);
            }
            *script->script_code_ptr = oatp.token_ptr;
            token_ptr = *script->script_code_ptr;
            tokens_read = token_ptr - bin_tokens;
        }
    }

    return script;
}

sct_f* disasm_file(char* filepath) {
    backup_file_gcc(filepath);
    FILE* file = fopen(filepath, "r");
    
    sct_f* sct_file = malloc(sizeof(sct_f));

    // TODO: validate structure
    sct_file->file = file;
    sct_file->structure = form_structure(file);
    
    // disassemble file's data section
    build_data_from_link_table(sct_file);

    // disassemble file's scripts
    int scripts_num = sct_file->structure->num_of_scripts;
    sct_file->scripts = w_malloc(sizeof(script*));
    *sct_file->scripts = w_malloc(scripts_num*sizeof(script));
    for(int i=0; i < scripts_num; i++) {
        script* s = disasm_script(i, sct_file);
        sct_file->scripts[i] = s;
    }

    return sct_file;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("Usage: [op] [file]\n");
        return 1;
    }

    char* filepath = argv[2];
    if(!is_filepath_valid(filepath)) {
        print_err_and_exit("ERROR, filepath is invalid.", -1);
        return 1;
    }

    game_functions = w_malloc(GAME_FUNCTIONS_NUM*sizeof(game_fun*));
    code_patterns = w_malloc(CODE_PATTERNS_NUM*sizeof(code_pattern*));
    expr_patterns = w_malloc(EXPR_PATTERNS_NUM*sizeof(code_pattern*));
    init_game_functions(game_functions);
    init_code_patterns(code_patterns);
    init_expr_patterns(expr_patterns);

    int op = atoi(argv[1]);
    sct_f* sct_file;
    switch(op) {
        case 0:
            sct_file = disasm_file(filepath);
            print_asm_data_section(sct_file);
            print_asm_file(sct_file);
            break;

        case 1:
            break;
    }

    return 0;
}
