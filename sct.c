#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<regex.h>

#include "sct.h"
#include "fileutils.h"

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
    printf("number of scripts: %x\nmain script: %x\nscripts: %x (size:%x)\ndata section: %x (size:%x)\nlink section: %x (size: %x)\n",
                                                        s.num_of_scripts, s.main_script_off, s.script_table_off,
                                                        s.script_table_size, s.data_sec_off, s.data_sec_size, s.link_table_off,
                                                        s.link_table_size);
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

bool is_var_pos(void* pattern, p_type pattern_type, mode m, int index) {
    code_pattern* p = (code_pattern*) pattern;        

    if(pattern_type == PARAM_TYPE) {
        param_pattern* p = (param_pattern*) pattern;        
    }

    int var_num = (m == MODE_BIN) ? p->bin_var_num : p->asm_var_num;
    int* var_pos = (m == MODE_BIN) ? p->bin_var_pos : p->asm_var_pos;

    for(int k=0; k < var_num; k++) {
        // printf("var_pos: %d, index: %d\n", var_pos[k], index);
        if(var_pos[k] == index) {
            return true;
        }
    }
    return false;
}

cp_cmp_result identify_cp(void* tokens_start, mode m) {
    cp_cmp_result res = { .is_identified = false, .match = NULL};

    // compare each pattern with tokens
    for(int i=0; i < CODE_PATTERNS_NUM; i++) {
        code_pattern* cp = code_patterns[i];
        void* token;

        if(cp != NULL) {
           token = tokens_start;
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
                    if(m == MODE_BIN) { vars[var_i] = *((int*) token); }
                    else if(m == MODE_ASM) {  vars[var_i] = *((char**) token); }
                    var_i++;
                }
                tokens += 1;
                token += 1;
           }
           if(eq_tokens == eq_goal) {
                res.is_identified = true;
                res.match = cp;
                res.tokens_pos = token;
                res.vars = vars;
                return res;
           }
       }
    }
    printf("Error, code pattern not recognized at %d.\n", tokens_start);
    return res;
}

pp_cmp_result identify_pp(void* tokens_start, mode m) {
    pp_cmp_result res = { .is_identified = false, .match = NULL};

    // compare each pattern with tokens
    for(int i=0; i < PARAM_PATTERNS_NUM; i++) {
        param_pattern* pp = param_patterns[i];
        void* token;

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
                   if(cmp_token(tokens, token, m))  {
                       break; // not a match
                   }
                    eq_tokens++;
                } else {
                    if(m == MODE_BIN) { vars[var_i] = *((int*) token); }
                    else if(m == MODE_ASM) {  vars[var_i] = *((char**) token); }
                    var_i++;
                }
                tokens += 1;
                token += 1;
           }
           if(eq_tokens == eq_goal) {
                res.is_identified = true;
                res.match = pp;
                res.tokens_pos = token;
                res.vars = vars;
                return res;
           }
       }
    }
    printf("Error, parameter pattern not recognized at %d.\n", tokens_start);
    return res;
}

param_obj* create_param_obj(param_pattern* pp, int vars[]) {
    param_obj* po = w_malloc(sizeof(param_obj));


    return po;
}

code_obj* create_code_obj(code_pattern* cp, void* vars, mode m, void* tokens_pos) {
    code_obj* c_obj = w_malloc(sizeof(code_obj));

    switch(cp->type) {
        case IF:
            break;
        case SWITCH:
            break;
        case FUNCTION_CALL:
            if(m == MODE_BIN) {
                int func_num = ((int*) vars)[0];
                int params_num = ((int*) vars)[1];

                game_fun* gf = game_functions[func_num];
                if(gf == NULL) { printf("ERROR, game func %d not found.\n", func_num); exit(-1); }
                // Read params
                for(int i=0; i < params_num; i++) {
                    pp_cmp_result res = identify_pp(tokens_pos, m);
                    if(res.is_identified) {
                        // tokens_pos = res.tokens_pos;
                        param_obj* po = create_param_obj(res.match, res.vars);

                    }
                }
            }

            break;
        case SCRIPT_CALL:
            break;
        case ASSIGNMENT:
            break;
        case TIMER:
            break;
    }

    return c_obj;
}

int disasm_script(int script_offset, sct_f* sf) {
    fseek(sf->file, script_offset+8, SEEK_SET);

    int script_len;
    fread(&script_len, 4, 1, sf->file);
    script_len -= 2;
    printf("script_offset: %x, script_len: %x (%x bytes)\n", script_offset, script_len, script_len*4);

    int bin_tokens[script_len];
    fread(&bin_tokens, 4, script_len, sf->file);

    cp_cmp_result res = identify_cp(bin_tokens, MODE_BIN);
    if(res.is_identified) {
        printf("Found pattern.\n");
        print_code_pattern(res.match);
        //create_code_obj(res.match, res.vars, MODE_BIN, res.tokens_pos);
    }
    // print_tokens(&tokens[0], script_len);

    return 0;
}

int disasm_file(char* filepath) {
    backup_file_gcc(filepath);
    FILE* file = fopen(filepath, "r");
    
    sct_f* sct_file = malloc(sizeof(sct_file));
    // TODO: validate structure
    sct_file->file = file;
    sct_file->structure = form_structure(file);
    print_sct_struct(sct_file);
    
    int script1 = get_script_off(0, sct_file);
    disasm_script(script1, sct_file);
}

bool is_filepath_valid(char* filepath) {
    regex_t reg;
    regmatch_t match[1];

    if (regcomp(&reg, ".*\\.sct", REG_EXTENDED) == -1) {
        printf("ERROR, regex could not compile.");
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
        printf("ERROR, filepath is invalid.");
        return 1;
    }

    init_game_functions();
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
