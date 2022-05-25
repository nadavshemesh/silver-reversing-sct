#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<regex.h>

#include "sct.h"

game_fun** game_functions;
const bool debug = false;
long read_offset = 0;

sct_s* form_structure(FILE* sctfile) {
    sct_s* structure = malloc(sizeof(sct_s));
    fseek(sctfile, 0x4, SEEK_SET);
    fread(&(structure->num_of_scripts), 1, 4, sctfile);

    fseek(sctfile, 0x8, SEEK_SET);
    fread(&(structure->main_script_off), 1, 4, sctfile);

    fseek(sctfile, 0xc, SEEK_SET);
    fread(&(structure->script_table_off), 1, 4, sctfile);
    structure->script_table_size = structure->num_of_scripts*4;

    fseek(sctfile, 0x10, SEEK_SET);
    fread(&(structure->data_sec_size), 1, 4, sctfile);

    fseek(sctfile, 0x14, SEEK_SET);
    fread(&(structure->data_sec_off), 1, 4, sctfile);

    fseek(sctfile, 0x18, SEEK_SET);
    fread(&(structure->link_table_size), 1, 4, sctfile);
    structure->link_table_size *= 4;

    fseek(sctfile, 0x1c, SEEK_SET);
    fread(&(structure->link_table_off), 1, 4, sctfile);

    return structure;
}

void print_sct_struct(sct_f* sf) {
    sct_s s = *(sf->structure);
    print_title("SCT_INFO");
    printf(ANSI_COLOR_BLUE);
    printf("number of scripts: %x\nmain script: %x\nscripts: %x (size:%x)\ndata section: %x (size:%x)\nlink section: %x (size: %x)\n",
                                                        s.num_of_scripts, s.main_script_off, s.script_table_off,
                                                        s.script_table_size, s.data_sec_off, s.data_sec_size, s.link_table_off,
                                                        s.link_table_size);
    printf(ANSI_COLOR_RESET);
    }

int get_script_off(int script_num, sct_f* sf) {
    sct_s s = *(sf->structure);
    if(script_num >= s.num_of_scripts) {
        printf("script #%d does'nt exist, there are %d scripts.\n", script_num, s.num_of_scripts);
        return -2;
    }
    int script_offset = -1;
    int table_offset = (s.script_table_off+(script_num+1)*4); // (+1 for consistency) zero indexed
    fseek(sf->file, table_offset, SEEK_SET);
    fread(&script_offset, 4, 1, sf->file);

    if(script_offset < 0) {
        printf("could not figure out script #%d offset.\n", script_num);
        return -1;
    }
    return script_offset*4+0x20;
}

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

bool is_var_pos_pp(void* pattern, mode m, int index) {
    param_pattern* p = (param_pattern*) pattern;        

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

    if(pattern_type == PARAM_TYPE) {
        return is_var_pos_pp(pattern, m, index);
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

void print_token_area_details(void* token_pos, mode m) {
    if(token_pos != NULL) {
        if(m == MODE_BIN) {
            int* token = (int*) token_pos;
            print_bin_token_seg(token, -2, 2, 5);
            // printf(ANSI_COLOR_YELLOW "[ %08x %08x %08x %08x %08x %08x ]" ANSI_COLOR_RESET "\n", \
            //  token[-2], token[-1], token[0], token[1], token[2], token[3]);
            // printf(ANSI_COLOR_RED "  % 9c % 9c ~~~~~~~~ % 8c  " ANSI_COLOR_RESET "\n", NULL, NULL, NULL);
        } else {
            char** token = (char**) token_pos;
            printf("[ %s %s %s %s ]\n", token-2, token-1, token, token+1);
        }
    }
    exit(1);
}

cp_cmp_result identify_cp(void* tokens_start, mode m) {
    cp_cmp_result res = { .is_identified = false, .match = NULL};
    void* token;

    // compare each pattern with tokens
    for(int i=0; i < CODE_PATTERNS_NUM; i++) {
        code_pattern* cp = code_patterns[i];
        token = tokens_start;

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

pp_cmp_result identify_pp(void* tokens_start, mode m) {
    pp_cmp_result res = { .is_identified = false, .match = NULL};
    void* token;

    // compare each pattern with tokens
    for(int i=0; i < PARAM_PATTERNS_NUM; i++) {
        param_pattern* pp = param_patterns[i];

        if(pp != NULL) {
           token = tokens_start;
           int token_num = (m == MODE_BIN) ? pp->bin_token_num : pp->asm_token_num;
           int var_num = (m == MODE_BIN) ? pp->bin_var_num : pp->asm_var_num;
           void* tokens = (m == MODE_BIN) ? (void*) pp->bin_tokens : (void*) pp->asm_tokens;
           int vars[var_num];
           if(m == MODE_ASM) { char* vars[var_num]; }
           
           int var_i = 0;
           int eq_goal = token_num - var_num;
           int eq_tokens = 0;
           // compare const tokens
           for(int j=0; j < token_num; j++) {
               if(!is_var_pos(pp, PARAM_TYPE, m, j)) {
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
                res.match = pp;
                res.tokens_pos = token;
                res.vars = w_malloc(var_num*sizeof(int));
                memcpy(res.vars, vars, var_num*sizeof(int));
                return res;
           }
       }
    }
    print_err("Error, param pattern not recognized.", 2);
    print_token_area_details(token, m);
    return res;
}


param_obj* create_param_obj(param_pattern* pp, void* vars, mode m, sct_f* sf) {
    param_obj* po = w_malloc(sizeof(param_obj));
    int bin_var_num = pp->bin_var_num;
    int asm_var_num = pp->asm_var_num;
    if(m == MODE_BIN) {
        int data_var = ((int*)vars)[0];
        data_obj* data;

        po->bin_vars = w_malloc(bin_var_num*sizeof(int*));
        memcpy(po->bin_vars, vars, bin_var_num*sizeof(int*));

        switch(pp->type) {
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
        
        default:
            break;
        }

        char* asm_vars = { data->name };
        po->asm_vars = w_malloc(asm_var_num*sizeof(char*));
        memcpy(po->asm_vars, &asm_vars, asm_var_num*sizeof(int*));

        po->pp = pp;
        po->data = data;
    }

    return po;
}

code_obj* read_code_block(code_pattern* cp, void* vars, mode m, void* tokens_pos, sct_f* sf) {
    code_obj* c_obj = w_malloc(sizeof(code_obj));
    if(m == MODE_BIN) {

    }

    return c_obj;
}

code_obj* read_function_call(code_pattern* cp, void* vars, mode m, void** tokens_pos_ptr, sct_f* sf) {
    code_obj* c_obj = w_malloc(sizeof(code_obj));

    if(m == MODE_BIN) {
        int func_num = ((int*) vars)[0];
        int params_num = ((int*) vars)[1];
        printf("game_func: %x, params_num: %d\n", func_num, params_num);

        game_fun* gf = game_functions[func_num];
        if(gf == NULL) { 
            char err[256];
            sprintf(err, "ERROR, game func %d not found.\n", func_num);
            print_err(err, -3);
        }
        char* asm_vars = { gf->name };
        c_obj->cp = cp;
        c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));
        c_obj->asm_vars = w_malloc(cp->asm_var_num*MAX_FUNC_NAME);
        memcpy(c_obj->bin_vars, vars, sizeof(c_obj->bin_vars));
        memcpy(c_obj->asm_vars, &asm_vars, sizeof(c_obj->asm_vars));
        param_obj params[params_num];
        // Read params
        for(int i=0; i < params_num; i++) {
            pp_cmp_result res = identify_pp(*tokens_pos_ptr, m);
            if(res.is_identified) {
                *tokens_pos_ptr = res.tokens_pos;
                param_obj* po = create_param_obj(res.match, res.vars, m, sf);
                params[i] = *po;
            } else { print_err_and_exit("unidentified param.", -4); }
        }
        c_obj->params_num = params_num;
        c_obj->params = w_malloc(params_num*sizeof(param_obj));
        memcpy(c_obj->params, params, params_num*sizeof(param_obj));
    }

    return c_obj;
}

obj_and_token_ptr create_code_obj(code_pattern* cp, void* vars, mode m, void* tokens_pos, sct_f* sf) {
    code_obj* c_obj;

    switch(cp->type) {
        case CODE_BLOCK:
            
            break;
        case IF_STATEMENT:
            break;
        case SWITCH:
            break;
        case FUNCTION_CALL:
            c_obj = read_function_call(cp, vars, m, &tokens_pos, sf);
            break;
        case SCRIPT_CALL:
            break;
        case ASSIGNMENT:
            break;
        case TIMER:
            break;
    }

    obj_and_token_ptr oatp = { .obj = c_obj, .type = OBJ_CODE, .token_ptr = tokens_pos };
    return oatp;
}

void print_asm_code_obj(code_obj* co) {
    code_pattern* cp = co->cp;
    // print code structure
    for(int i=0, j=0; i < cp->asm_token_num; i++) {
        if(is_var_pos(cp, CODE_TYPE, MODE_ASM, i)) {
            printf("%s", co->asm_vars[j]);
            j++;
        } else {
            printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, cp->asm_tokens[i]);
        }
        if(i+1 != cp->asm_token_num) { printf(" "); }
    }
    // print params
    if(co->params_num > 0) {
        printf("(");
        for(int i=0; i < co->params_num; i++) {
            param_obj* po = &(co->params)[i];
            param_pattern* pp = po->pp;
            
            for(int k=0, l=0; k < pp->asm_token_num; k++) {
                if(is_var_pos_pp(pp, MODE_ASM, k)) {
                    printf("%s", po->asm_vars[l]);
                    l++;
                    if(i+1 < co->params_num) { printf(", "); }
                } else {
                    printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, pp->asm_tokens[k]);
                }
                if(k+1 != pp->asm_token_num) { printf(" "); }
            }
        }
        printf(")\n");
    }
}

void print_asm_code_objs(code_obj* co, int code_obj_num) {
    print_title("_ASM_");
    for(int i=0; i < code_obj_num; i++) {
        print_asm_code_obj(&(co)[i]);
    }
}

void print_asm_script(script* script) {
    print_asm_code_objs(script->code_objs, script->code_obj_num);
}

int disasm_script(int script_offset, sct_f* sf) {
    fseek(sf->file, script_offset+8, SEEK_SET);

    script* script = w_malloc(sizeof(script));
    int script_len;
    fread(&script_len, 4, 1, sf->file);
    script_len -= 2;
    script->size_in_words = script_len;
    printf("script_offset: %x, script_len: %x (%x bytes)\n", script_offset, script_len, script_len*4);

    int bin_tokens[script_len];
    script->script_code_ptr = w_malloc(sizeof(void*));
    *script->script_code_ptr = bin_tokens;
    fread(&bin_tokens, 4, script_len, sf->file);

    while(*script->script_code_ptr != NULL) {
        cp_cmp_result res = identify_cp(*script->script_code_ptr, MODE_BIN);
        if(res.is_identified) {
            char msg[256];
            sprintf(msg, "Found pattern '%s'.\n", res.match->name);
            print_success(msg);
            obj_and_token_ptr oatp = create_code_obj(res.match, res.vars, MODE_BIN, res.tokens_pos, sf);
            if(oatp.type == OBJ_CODE) {
                code_obj* co = (code_obj*) oatp.obj;
                print_code_obj(co);
                print_asm_code_obj(co);
            }
            *script->script_code_ptr = oatp.token_ptr;
        }
    }
    // print_tokens(&tokens[0], script_len);

    return 0;
}

int disasm_file(char* filepath) {
    backup_file_gcc(filepath);
    FILE* file = fopen(filepath, "r");
    
    sct_f* sct_file = malloc(sizeof(sct_f));
    // TODO: validate structure
    sct_file->file = file;
    sct_file->structure = form_structure(file);
    print_sct_struct(sct_file);
    
    build_data_from_link_table(sct_file);

    int script1 = get_script_off(0, sct_file);
    disasm_script(script1, sct_file);
}

bool is_filepath_valid(char* filepath) {
    regex_t reg;
    regmatch_t match[1];

    if (regcomp(&reg, ".*\\.sct", REG_EXTENDED) == -1) {
        print_err_and_exit("ERROR, regex could not compile.", -1);
        return false;
    }

    if(regexec(&reg, filepath, 1, match, REG_EXTENDED) == 0) {
        return true;
    } else {
        return false;
    }
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
    init_game_functions(game_functions);
    init_code_patterns();
    init_param_patterns();

    int op = atoi(argv[1]);
    switch(op) {
        case 0:
            disasm_file(filepath);
            break;

        case 1:
            break;
    }

    return 0;
}
