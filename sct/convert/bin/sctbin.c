#include "sct\convert\bin\sctbin.h"

void build_scripts_order(sct_f* sf) {
    unsigned long f_pos = ftell(sf->file);

    int scripts_num = sf->structure->num_of_scripts;
    int script_table = sf->structure->script_table_off+4;
    int scripts[scripts_num];

    fseek(sf->file, script_table, SEEK_SET);
    fread(scripts, scripts_num, sizeof(int), sf->file);

    sf->script_table = w_malloc(scripts_num*sizeof(int));
    memcpy(sf->script_table, scripts, scripts_num*sizeof(int));
    
    fseek(sf->file, f_pos, SEEK_SET);
}

void build_data_from_link_table(sct_f* sct) {
    int link_offset = (sct->structure->link_table_off)+4;
    int links_num = ((sct->structure->link_table_size)/4);
    // printf("links num: %d\n", links_num);
    int data_ref_size = 0;
    int ref_is_data[links_num];
    int ref_count[links_num];
    int refs[links_num];

    fseek(sct->file, link_offset, SEEK_SET);
    fread(&refs, 4, links_num, sct->file);

    for(int i=0; i < links_num; i++) { ref_is_data[i] = 0; ref_count[i] = 0; }
    // check and convert to data offset
    for(int i=0; i < links_num; i++) {
        // printf("type: %03x\n", refs[i] & 0xFF000000);
        if((refs[i] & 0xFF000000) == 0x40000000) {
            refs[i] &= 0x00FFFFFF;
            // printf("addr: %06x\n", refs[i]);
            ref_is_data[i]++;
        }
    } 

    for(int i=0; i < links_num; i++) { ref_count[i] = 0; } 
    // count refs to data by id
    for(int i=0, j=0; i < links_num; i++) {
        if(ref_is_data[i] > 0) {
            int in_code_offset = refs[i]*4+0x24;
            // printf("addr: %06x\n", in_code_offset);
            int data_id;
            fseek(sct->file, in_code_offset, SEEK_SET);
            fread(&data_id, 4, 1, sct->file);

            // printf("data id: %d\n", data_id);
            ref_count[data_id]++;
        }
    }

    // count distinct refs
    // for(int i=0; i < links_num; i++) { printf("ref %d: %d\n", i, ref_count[i]); } 
    for(int i=0; i < links_num; i++) { if(ref_count[i] > 0) {data_ref_size++;} } 
    // printf("data_ref_size: %08x\n", data_ref_size);

    // create data objects
    data_obj data_arr[data_ref_size];
    for(int i=0, j=0; i < links_num; i++) {
        if(ref_count[i] > 0) {
            data_arr[j].id = i;
            // printf("data id: %08x\n", i);
            data_arr[j].references = ref_count[i];
            // printf("ref count: %d\n", ref_count[i]);
            j++;
        }
    }
    // sort data_arr by id
    qsort(data_arr, data_ref_size, sizeof(data_obj), compare_data_obj_ids);

    // calculate data obj size and data
    for(int i=0, j=0; i < data_ref_size; i++, j++) {
        int data_section_offset = (sct->structure->data_sec_off) + 4;
        int data_offset = data_section_offset + data_arr[i].id*4;
        int byte_size = 0;

        if(i == data_ref_size-1) { // last item
            byte_size = ((link_offset)-data_offset);
        } else {
            byte_size = (data_arr[i+1].id - data_arr[i].id)*4;
        }

        data_arr[i].byte_size = byte_size;
        byte data[byte_size];

        fseek(sct->file, data_offset, SEEK_SET);
        fread(&data, 1, byte_size, sct->file);
        // printf("%08x\n", data[0]);

        char prefix[] = "VAR_";
        char name[sizeof(prefix)+4];
        sprintf(name, "%s%d", prefix, j);
        data_arr[i].name = aapts(name);
        // data_arr[i].bin_data = w_malloc(byte_size);
        data_arr[i].data = w_malloc(byte_size);
        memset(data_arr[i].data, 0, byte_size);
        // memset(data_arr[i].bin_data, 0, byte_size);
        // memcpy(data_arr[i].bin_data, data, byte_size);
        memcpy(data_arr[i].data, data, byte_size);
        // memcpy(data_arr[i].asm_data, asm_data, byte_size);
        // print_data_obj(&data_arr[i]);
    }

    sct->data_objs_num = data_ref_size;
    sct->data_section = w_malloc(sizeof(data_obj*));
    *sct->data_section = w_malloc(data_ref_size*sizeof(data_obj));
    memcpy(*sct->data_section, data_arr, data_ref_size*sizeof(data_obj));
    // print_data_obj((&data_objs[5]));

    // print_data_obj(&(sct->data_section)[5]);
}

bool bin_cmp_token(int* a_token_ptr, int* b_token_ptr) {
    if(*a_token_ptr == *b_token_ptr) {
        // printf("%08x == %08x\n", *a_token, *b_token);
        return true;
    }
    // printf("%08x != %08x\n", *a_token, *b_token);
    return false;
}

bool bin_is_var_pos_expr(void* pattern, int index) {
    expr_pattern* p = (expr_pattern*) pattern;        

    int var_num = p->bin_var_num;
    int* var_pos = p->bin_var_pos;

    // printf("vpos: %d\n", *var_pos);
    for(int k=0; k < var_num; k++) {
        // printf("var_pos: %d, index: %d\n", var_pos[k], index);
        if(var_pos[k] == index) {
            return true;
        }
    }
    return false;
}

bool bin_is_var_pos(void* pattern, p_type pattern_type, int index) {
    code_pattern* p = (code_pattern*) pattern;        

    if(pattern_type == EXPRESSION_TYPE) {
        return bin_is_var_pos_expr(pattern,index);
    }

    int var_num = p->bin_var_num;
    int* var_pos = p->bin_var_pos;

    // printf("vpos: %d\n", *var_pos);
    for(int k=0; k < var_num; k++) {
        // printf("var_pos: %d, index: %d\n", var_pos[k], index);
        if(var_pos[k] == index) {
            return true;
        }
    }
    return false;
}

cp_cmp_result bin_identify_cp(void** token_pos_ptr) {
    cp_cmp_result res = { .is_identified = false, .match = NULL};
    int* token;

    // compare each pattern with tokens
    for(int i=0; i < CODE_PATTERNS_NUM; i++) {
        code_pattern* cp = code_patterns[i];
        token = *token_pos_ptr;

        if(cp != NULL) {
           int token_num = cp->bin_token_num;
           int var_num = cp->bin_var_num;
           int* tokens = cp->bin_tokens;
           int vars[var_num];
           int var_i = 0;
           int eq_goal = token_num - var_num;
           int eq_tokens = 0;
           // compare const tokens
           for(int j=0; j < token_num; j++) {
               if(!bin_is_var_pos(cp, CODE_TYPE, j)) {
                   if(!bin_cmp_token(tokens, token))  {
                       break; // not a match
                   }
                    eq_tokens++;
                } else {
                    printf("var found: %08x\n", *((int*)token));
                    vars[var_i] = *((int*) token);
                    var_i++;
                }
                tokens++;
                token++;
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
    print_token_area_details(token, MODE_BIN);

    return res;
}

expr_cmp_result bin_identify_expr(void** token_pos_ptr) {
    expr_cmp_result res = { .is_identified = false, .match = NULL};
    int* token;

    // compare each pattern with tokens
    for(int i=0; i < EXPR_PATTERNS_NUM; i++) {
        expr_pattern* expr = expr_patterns[i];

        if(expr != NULL) {
           token = *token_pos_ptr;
           int token_num = expr->bin_token_num;
           int var_num = expr->bin_var_num;
           int* tokens = expr->bin_tokens;
           int vars[var_num];
           
           int var_i = 0;
           int eq_goal = token_num - var_num;
           int eq_tokens = 0;
           // compare const tokens
           for(int j=0; j < token_num; j++) {
               if(!bin_is_var_pos(expr, EXPRESSION_TYPE, j)) {
                   if(!bin_cmp_token(tokens, token))  {
                       break; // not a match
                   }
                    eq_tokens++;
                } else {
                    vars[var_i] = *((int*) token);
                    var_i++;
                }
                tokens++;
                token++;
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
    print_token_area_details(token, MODE_BIN);
    return res;
}

int bin_read_expression_size(void** tokens_pos_ptr, bool with_prologue) { 
        int* token = *tokens_pos_ptr; 
        if(!with_prologue || *token == 0xfffffffd) {

            int size = (with_prologue) ? *(token+1) : *token;
            if(size > 0) {
                *tokens_pos_ptr += (with_prologue) ? 8 : 4;
                return size;
            }
            print_err("Error, No expr obj declaration and size.", -2);
            print_token_area_details(*tokens_pos_ptr, MODE_BIN);
        } 

        print_err("Error, No expr obj declaration and size.", -2);
        print_token_area_details(*tokens_pos_ptr, MODE_BIN);
}

expression* bin_read_expression(void** tokens_pos_ptr, bool with_prologue, sct_f* sf) {
    expression* exp = w_malloc(sizeof(expression));
    int exprs_num = bin_read_expression_size(tokens_pos_ptr, with_prologue);
    printf("expr size: %d\n", exprs_num);
    expr_obj exprs[exprs_num];
    // Read exprs
    for(int i=0; i < exprs_num; i++) {
        expr_cmp_result res = bin_identify_expr(tokens_pos_ptr);
        if(res.is_identified) {
            char msg[256];
            sprintf(msg, "Found expr pattern '%s'.", res.match->name);
            print_success(msg);
            *tokens_pos_ptr = res.tokens_pos;
            expr_obj* eo = bin_create_expr_obj(res.match, res.vars, tokens_pos_ptr, sf);
            exprs[i] = *eo;
        } else { print_err_and_exit("unidentified expression.", -4); }
    }
    exp->expr_objs_len = exprs_num;
    exp->expr_objs = w_malloc(exprs_num*sizeof(expr_obj));
    memcpy(exp->expr_objs, exprs, exprs_num*sizeof(expr_obj));

    return exp;
}

expr_obj* bin_read_function_call_expr(expr_pattern* expr_p, void* vars, void** tokens_pos_ptr, sct_f* sf) {
    expr_obj* e_obj = create_and_init_expr_obj();

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
        exp = bin_read_expression(tokens_pos_ptr, true, sf);
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

    return e_obj;
}

expr_obj* bin_create_expr_obj(expr_pattern* expr, void* vars, void** token_pos_ptr, sct_f* sf) {
    expr_obj* eo = create_and_init_expr_obj();

    int bin_var_num = expr->bin_var_num;
    int asm_var_num = expr->asm_var_num;
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
            data->data = w_malloc(sizeof(int));
            // data->bin_data = w_malloc(sizeof(int*));
            // data->asm_data = w_malloc(sizeof(char*));
            memcpy(data->name, &c, sizeof(data->name));
            memcpy(data->data, i, sizeof(data->data));
            // memcpy(data->asm_data, &c, sizeof(data->asm_data));
            // memcpy(data->bin_data, i, sizeof(data->bin_data));
            break;

        case DATA_PTR:
            data = get_data_obj_by_id(data_var, sf);
            break;

        case VAR_PTR:
            data = get_data_obj_by_id(data_var, sf);
            break;
        case FUNCTION:
            expr_obj* exp_func = bin_read_function_call_expr(expr, vars, token_pos_ptr, sf);
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

    return eo;
}

code_obj* bin_read_code_block(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

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
        cp_cmp_result res = bin_identify_cp(token_pos_ptr);
        if(res.is_identified) {
            char msg[256];
            sprintf(msg, "Found pattern '%s'.\n", res.match->name);
            print_success(msg);
            obj_and_token_ptr oatp = bin_create_code_obj(res.match, res.vars, token_pos_ptr, sf);
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

    return c_obj;
}

code_obj* bin_read_code_block_cases(code_pattern* cp, void* vars, int* cases, int* case_ptrs,
         int cases_num, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

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
    sprintf(num_s, "%d", cases[case_index]);
    char* asm_vars[1] = { aapts(num_s) };
    current_case->asm_vars = w_malloc((current_case->cp->asm_var_num)*sizeof(char*));
    memcpy(current_case->asm_vars, &asm_vars, sizeof(current_case->asm_vars));

    current_case->code_nodes = w_malloc(sizeof(node*));
    *current_case->code_nodes = w_malloc(sizeof(node));
    // for(int i=0; i < cases_num; i++) {
    //     printf("case addr: %08x\n", case_ptrs[i]);
    // }

    int case_tokens_num = (case_ptrs[case_index+1]-case_ptrs[case_index])/4;
    printf("case tokens num: %d\n", case_tokens_num);
    while(tokens_read < size_in_words) {
        printf("Block tokens read: %d\n", tokens_read);
        cp_cmp_result res = bin_identify_cp(token_pos_ptr);
        if(res.is_identified) {
            char msg[256];
            sprintf(msg, "Found pattern '%s'.\n", res.match->name);
            print_success(msg);
            obj_and_token_ptr oatp = bin_create_code_obj(res.match, res.vars, token_pos_ptr, sf);
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

            printf("tokens read: %d\n", tokens_read);
            // After break add case code_obj to main block
            if((tokens_read == case_tokens_num) && case_index < cases_num) {
                printf("case %d\n", cases[case_index]);
                node* code_node = create_node(current_case);
                if(c_obj->code_nodes_num == 0){
                    *c_obj->code_nodes = code_node;
                } else {
                    insert_node(c_obj->code_nodes, create_node(current_case));
                }
                c_obj->code_nodes_num++;

                // init current case (next case)
                case_index++;
                if(case_index < cases_num-1) {
                    case_tokens_num += (case_ptrs[case_index+1]-case_ptrs[case_index])/4;
                } else {
                    case_tokens_num = size_in_words;
                }
                printf("case tokens num: %d\n", case_tokens_num);
                current_case = create_and_init_c_obj();
                current_case->cp = case_cp;

                sprintf(num_s, "%d", cases[case_index]);
                printf("next case %d\n", cases[case_index]);
                char* asm_vars[1] = { aapts(num_s) };
                current_case->asm_vars = w_malloc((current_case->cp->asm_var_num)*sizeof(char**));
                memcpy(current_case->asm_vars, asm_vars, sizeof(current_case->asm_vars));
                
                current_case->code_nodes = w_malloc(sizeof(node*));
                *current_case->code_nodes = w_malloc(sizeof(node));
            }
        }
    }

    return c_obj;
}

code_obj* bin_read_function_call(code_pattern* cp, void* vars, void** tokens_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

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
        exp = bin_read_expression(tokens_pos_ptr, true, sf);
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

    return c_obj;
}

code_obj* bin_read_if_statement(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    expression* exp = bin_read_expression(token_pos_ptr, false, sf);
    c_obj->cp = cp;

    code_obj* code_block_obj;
    cp_cmp_result res = bin_identify_cp(token_pos_ptr);
    if(res.is_identified && res.match->type == CODE_BLOCK) {
        *token_pos_ptr = res.tokens_pos;
        obj_and_token_ptr oatp = bin_create_code_obj(res.match, res.vars, token_pos_ptr, sf);
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

    return c_obj;
}

code_obj* bin_read_else_statement(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;

    code_obj* code_block_obj;
    *token_pos_ptr -= 4; // move back before -0xfffffffc
    cp_cmp_result res = bin_identify_cp(token_pos_ptr);
    if(res.is_identified && res.match->type == CODE_BLOCK) {
        *token_pos_ptr = res.tokens_pos;
        obj_and_token_ptr oatp = bin_create_code_obj(res.match, res.vars, token_pos_ptr, sf);
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

    return c_obj;
}

code_obj* bin_read_assignment(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    expression* exp = bin_read_expression(token_pos_ptr, true, sf);
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

    return c_obj;
}

char* determine_room_var_ptr(int first_offset, int second_offset) {
    game_var* gv = get_game_var_by_offsets(first_offset, second_offset);
    if(gv != NULL) {
        return gv->name;
    }
    char str[50];
    sprintf(str, "game_var_%02x_%02x", first_offset, second_offset);
    return aapts(str);
}

code_obj* bin_read_room_var_ptr(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;

    int first_offset = (((int*)vars))[0];
    int second_offset = ((int*)(vars))[1];
    char* asm_vars[1] = { determine_room_var_ptr(first_offset, second_offset) };
    int bin_vars[2] = { first_offset, second_offset };

    c_obj->asm_vars = w_malloc(cp->asm_var_num*sizeof(char*));
    c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));

    memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
    memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

    return c_obj;
}

code_obj* bin_read_var_ptr(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

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

    return c_obj;
}

code_obj* bin_read_script_call(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

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

    return c_obj;
}

code_obj* bin_read_switch_case(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = w_malloc(sizeof(code_obj));

    c_obj->cp = cp;
    int cases_num = ((int*)vars)[0];
    if(cases_num <= 0) print_err_and_exit("Error, switch without cases.", -2);
    int padding_from_vars_to_block = 0x80;
    int padding_from_vars_to_ptrs = 0x40;

    // read cases values
    int* token_pos = *token_pos_ptr;
    token_pos += 1;
    int cases[cases_num];
    int case_ptrs[cases_num];
    memcpy(cases, token_pos, cases_num*sizeof(int));

    // read case addrs
    token_pos += padding_from_vars_to_ptrs;
    memcpy(case_ptrs, token_pos, cases_num*sizeof(int));
    // for(int i=0; i < cases_num; i++) {
    //     printf("case addr: %08x\n", case_ptrs[i]);
    // }

    // read expression
    token_pos += padding_from_vars_to_block-padding_from_vars_to_ptrs;
    // token_pos += padding_to_block;
    *token_pos_ptr = token_pos;
    expression* exp = bin_read_expression(token_pos_ptr, true, sf);
    c_obj->expression_node_num = 1;
    node** exp_nodes = w_malloc(sizeof(node*));
    *exp_nodes = w_malloc(sizeof(node));
    node* new_node = create_node(exp);
    *exp_nodes = new_node;
    c_obj->expression_nodes = exp_nodes;

    char* asm_vars[cases_num];
    for(int i=0; i < cases_num; i++) {
        char str[10];
        sprintf(str, "%d", cases[i]);
        asm_vars[i] = str;
    }

    // read code block
    code_obj* code_block_obj;
    cp_cmp_result res = bin_identify_cp(token_pos_ptr);
    if(res.is_identified && res.match->type == CODE_BLOCK) {
        *token_pos_ptr = res.tokens_pos;
        code_obj* c_obj;
        code_block_obj = bin_read_code_block_cases(res.match, res.vars, cases, case_ptrs, cases_num, token_pos_ptr, sf);
    } else {
        print_err_and_exit("Error, no code_block after switch-case.", -2);
    }

    node* c_block_node = create_node(code_block_obj);
    c_obj->code_nodes = w_malloc(sizeof(node*));
    *c_obj->code_nodes = w_malloc(sizeof(node));
    *c_obj->code_nodes = c_block_node;
    // print_code_obj(c_block_node->item);
    c_obj->code_nodes_num = 1;

    return c_obj;
}

code_obj* bin_read_default(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = w_malloc(sizeof(code_obj));

    c_obj->cp = cp;

    c_obj->code_nodes_num = 0;
    c_obj->code_nodes = NULL;
    c_obj->asm_vars = NULL;
    c_obj->bin_vars = NULL;

    c_obj->expression_node_num = 0;
    c_obj->expression_nodes = NULL;

    return c_obj;
}

obj_and_token_ptr bin_create_code_obj(code_pattern* cp, void* vars, void** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj;

    switch(cp->type) {
        case CODE_BLOCK:
            c_obj = bin_read_code_block(cp, vars, token_pos_ptr, sf);
            break;
        case IF_STATEMENT:
            c_obj = bin_read_if_statement(cp, vars, token_pos_ptr, sf);
            break;
        case ELSE_STATEMENT:
            c_obj = bin_read_else_statement(cp, vars, token_pos_ptr, sf);
            break;
        case SWITCH:
            c_obj = bin_read_switch_case(cp, vars, token_pos_ptr, sf);
            break;
        case FUNCTION_CALL:
            c_obj = bin_read_function_call(cp, vars, token_pos_ptr, sf);
            break;
        case SCRIPT_CALL:
            c_obj = bin_read_script_call(cp, vars, token_pos_ptr, sf);
            break;
        case ROOM_VAR_PTR:
            c_obj = bin_read_room_var_ptr(cp, vars, token_pos_ptr, sf);
            break;
        case CP_VAR_PTR:
            c_obj = bin_read_var_ptr(cp, vars, token_pos_ptr, sf);
            break;
        case ASSIGNMENT:
            c_obj = bin_read_assignment(cp, vars, token_pos_ptr, sf);
            break;
        default:
            c_obj = bin_read_default(cp, vars, token_pos_ptr, sf);
    }

    void* token_ptr = *token_pos_ptr;
    obj_and_token_ptr oatp = { .obj = c_obj, .type = OBJ_CODE, .token_ptr = token_ptr };
    return oatp;
}
