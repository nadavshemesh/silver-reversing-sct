#include "sct\convert\convert.h"

game_fun** game_functions;
code_pattern** code_patterns;
expr_pattern** expr_patterns;

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
            obj_and_token_ptr oatp = bin_create_code_obj(res.match, res.vars, script->script_code_ptr, sf);
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