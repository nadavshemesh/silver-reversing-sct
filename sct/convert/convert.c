#include "sct\convert\convert.h"

game_fun** game_functions;
code_pattern** code_patterns;
expr_pattern** expr_patterns;


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
        cp_cmp_result res = bin_identify_cp(script->script_code_ptr);
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
    fclose(file);

    return sct_file;
}

sct_f* asm_file(char* filepath) {
    backup_file_gcc(filepath);
    FILE* file = fopen(filepath, "r");

    if(file == NULL) perror("Error ");
    
    sct_f* sct_file = w_malloc(sizeof(sct_f));
    sct_file->file = file;
    
    int sections_num = count_file_sections(sct_file);
    printf("num of sections: %d\n", sections_num);
    build_data_section(sct_file);

    char*** tokens_pos_ptr;
    // for(int i=0; i < sections_num-1; i++) {
    for(int i=0; i < 1; i++) {
        char** tokens = tokenize_next_section(sct_file);
        tokens_pos_ptr = &tokens;
        cp_cmp_result res = asm_identify_cp(tokens_pos_ptr);
        if(res.is_identified) {
            char msg[256];
            sprintf(msg, "Found pattern '%s'.\n", res.match->name);
            print_success(msg);
            code_pattern* cp = res.match;
            print_code_pattern(cp);
        }
    }

    fclose(file);
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
            sct_file = asm_file(filepath);
            break;
    }

    return 0;
}
