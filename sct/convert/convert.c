#include "sct\convert\convert.h"

game_var** game_vars;
game_fun** game_functions;
code_pattern** code_patterns;
expr_pattern** expr_patterns;
catalog* enemy_cat;
catalog* handle_cat;


script* disasm_script(int script_num, sct_f* sf) {
    int script_offset = get_script_off(script_num, sf);
    // int script_offset = get_script_offset_sorted(script_num, sf);
    int script_original_offset = get_script_offset(script_num, sf); 
    // printf("of: %08x\n", script_original_offset);
    // exit(0);
    fseek(sf->file, script_offset+8, SEEK_SET);

    script* script = w_malloc(sizeof(script));
    script->number = get_script_num_by_offset(script_offset, sf);
    char script_name[256];
    sprintf(script_name, "._SCRIPT_%d", script->number);
    script->name = w_malloc(sizeof(char*));
    memcpy(script->name, script_name, strlen(script_name)+1);
    script->code_nodes_num = 0;
    script->code_nodes = w_malloc(sizeof(node*));
    *script->code_nodes = w_malloc(sizeof(node));
    // printf("node ptr: %08x\n", script->code_nodes);
    script->script_code_ptr = w_malloc(sizeof(void*));

    int script_len;
    fread(&script_len, 4, 1, sf->file);
    script_len -= 2;
    script->size_in_words = script_len;
    // printf("script_offset: %x, script_len: %x (%x bytes)\n", script_offset, script_len, script_len*4);

    int bin_tokens[script_len];
    *script->script_code_ptr = bin_tokens;
    fread(&bin_tokens, 4, script_len, sf->file);

    int* token_ptr = *(script->script_code_ptr);
    int tokens_read = token_ptr - bin_tokens;
    while(tokens_read < script_len) {
        // printf("Tokens read: %d\n", tokens_read);
        cp_cmp_result res = bin_identify_cp(script->script_code_ptr);
        if(res.is_identified) {
            char msg[256];
            sprintf(msg, "Found pattern '%s'.\n", res.match->name);
            // print_success(msg);
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
    print_sct_struct(sct_file);
    
    // disassemble file's data section
    build_scripts_order(sct_file);
    build_data_from_link_table(sct_file);

    // disassemble file's scripts
    int scripts_num = sct_file->structure->num_of_scripts;
    sct_file->scripts = w_malloc(scripts_num*sizeof(script*));
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
    sct_file->structure = w_malloc(sizeof(sct_s));
    sct_file->structure->code_section_word_counter = 0;
    sct_file->structure->link_table_size = 0;
    sct_file->data_link_table = w_malloc(sizeof(node*));
    
    int sections_num = count_file_sections(sct_file);
    printf("num of sections: %d\n", sections_num);
    sct_file->scripts_num = sections_num-1;
    sct_file->script_table = w_malloc(sct_file->scripts_num*sizeof(int));

    build_scripts_lables_and_order(sct_file);
    build_data_section(sct_file);

    // sct_file->scripts = w_malloc(sizeof(script*));
    sct_file->scripts = w_malloc((sections_num-1)*sizeof(script*));

    char*** tokens_pos_ptr;
    for(int i=0; i < sct_file->scripts_num; i++) {
        script* sc = w_malloc(sizeof(script));
        sc->code_nodes = w_malloc(sizeof(node*));
        sc->code_nodes_num = 0;
        sc->number = i;
        sc->name = aapts(get_script_label_by_id(i, sct_file));

        sct_file->script_table[i] = get_sct_code_word_count(sct_file);
        char** tokens = tokenize_next_section(sct_file);
        int block_tokens_size = count_tokens(tokens);
        tokens_pos_ptr = &tokens;

        code_obj* co = asm_read_code_block(block_tokens_size, tokens_pos_ptr, sct_file);
        node* code_node = create_node(co);
        if(sc->code_nodes_num == 0) {
            *sc->code_nodes = code_node;
        } else {
            insert_node(sc->code_nodes, code_node);
        }

        sct_file->scripts[i] = sc;

        free(tokens);
    }

    // printf("data section size: %08x\n", sct_file->structure->data_sec_size);
    // print_script_table_offsets(sct_file);
    sct_file->structure->num_of_scripts = sct_file->scripts_num;
    sct_file->structure->script_table_off = 0x20 + sct_file->structure->code_section_word_counter*4;
    sct_file->structure->script_table_size = sct_file->scripts_num*4;
    sct_file->structure->data_sec_off = sct_file->structure->script_table_off + sct_file->structure->script_table_size;
    sct_file->structure->link_table_off = sct_file->structure->data_sec_off + sct_file->structure->data_sec_size;
    print_sct_struct(sct_file);

    fclose(file);
    return sct_file;
}

void write_tsct_asm_file(char* filepath, bool write_to_out_dir, sct_f* sf) {
    char* dir = (write_to_out_dir) ? aapts("./out/") : getDir(filepath);
    char* filename = getFilenameNoExt(filepath);
    char ext[] = ".tsct";

    int str_path_size = strlen(dir)+strlen(filename)+strlen(ext)+1;
    char fullpath[str_path_size];
    snprintf(fullpath, str_path_size, "%s%s%s", dir, filename, ext);

    FILE* f = fopen(fullpath, "wb");

    char err[256];
    sprintf(err, "Error, file %s, could not be opened.", fullpath);
    if(f == NULL) print_err_and_exit(err, -4);

    sf->out_file = w_malloc(sizeof(FILE));
    sf->out_file = f;

    write_asm_file(sf);
    if(write_to_out_dir) {
        mark_auto_gen_file(sf);
    }

    char msg[256];
    sprintf(msg, "successfully written to %s.", fullpath);
    print_success(msg);

    fclose(f);
}

void write_sct_bin_file(char* filepath, sct_f* sf) {
    char* dir = getDir(filepath);
    char* filename = getFilenameNoExt(filepath);
    char ext[] = ".sct";

    int str_path_size = strlen(dir)+strlen(filename)+strlen(ext)+1;
    char fullpath[str_path_size];
    snprintf(fullpath, str_path_size, "%s%s%s", dir, filename, ext);

    FILE* f = fopen(fullpath, "wb");

    char err[256];
    sprintf(err, "Error, file %s, could not be opened.", fullpath);
    if(f == NULL) print_err_and_exit(err, -4);

    sf->out_file = w_malloc(sizeof(FILE));
    sf->out_file = f;

    write_sct_header(sf);
    write_bin_code_section(sf);
    write_bin_script_table(sf);
    write_bin_data_section(sf);
    write_bin_link_table(sf);

    char msg[256];
    sprintf(msg, "successfully written to %s.", fullpath);
    print_success(msg);
    fclose(f);
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

    game_vars = w_malloc(GAME_VARS_NUM*sizeof(game_var*));
    game_functions = w_malloc(GAME_FUNCTIONS_NUM*sizeof(game_fun*));
    code_patterns = w_malloc(CODE_PATTERNS_NUM*sizeof(code_pattern*));
    expr_patterns = w_malloc(EXPR_PATTERNS_NUM*sizeof(code_pattern*));
    init_game_vars(game_vars);
    init_game_functions(game_functions);
    init_code_patterns(code_patterns);
    init_expr_patterns(expr_patterns);

    // catalogs
    enemy_cat = w_malloc(sizeof(catalog*));
    handle_cat = w_malloc(sizeof(catalog*));
    init_enemies_cat(enemy_cat);
    init_char_handle_cat(handle_cat);

    bool save_to_out = false;
    for(int i=0; i < argc; i++) {
        char* arg = argv[i];
        if(strs_identical(arg, "-s")) {
            save_to_out = true;
        }
    }

    int op = atoi(argv[1]);
    sct_f* sct_file;
    char* ext;
    switch(op) {
        case 0:
            ext = getFilenameExt(filepath);
            if(!strs_identical(ext, ".sct")) {
                print_err_and_exit("Wrong extension. should be an .sct file", -2);
            }

            sct_file = disasm_file(filepath);
            write_tsct_asm_file(filepath, false, sct_file);
            // write to ./out/
            if(save_to_out) {
                write_tsct_asm_file(filepath, true, sct_file);
            }
            break;

        case 1:
            ext = getFilenameExt(filepath);
            if(!strs_identical(ext, ".tsct")) {
                print_err_and_exit("Wrong extension. should be a .tsct file", -4);
            }

            sct_file = asm_file(filepath);
            write_sct_bin_file(filepath, sct_file);
            break;
    }

    return 0;
}
