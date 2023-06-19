#include "sct\convert\convert.h"
#include <unistd.h>

const char* TEMP_GF_FILE = "./gf.txt";
const char* TEMP_GF_NAME = "gf.txt";
game_var** game_vars;
game_fun** game_functions;
code_pattern** code_patterns;
expr_pattern** expr_patterns;
catalog* enemy_cat;
catalog* handle_cat;
catalog* item_cat;
catalog* sound_cat;


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

void write_game_functions_ref_file(char* gf_name) {
    char* dir = "./docs/catalog/func_refs/";
    char* filename = gf_name;
    char ext[] = ".md";

    int str_path_size = strlen(dir)+strlen(filename)+strlen(ext)+1;
    char fullpath[str_path_size];
    snprintf(fullpath, str_path_size, "%s%s%s", dir, filename, ext);

    FILE* f = fopen(fullpath, "wb");
    FILE* gf_f = fopen(TEMP_GF_FILE, "rb");

    char err[256], err2[256];
    sprintf(err, "Error, file %s, could not be opened.", fullpath);
    sprintf(err2, "Error, file %s, could not be opened.", TEMP_GF_FILE);
    if(f == NULL) print_err_and_exit(err, -4);
    if(gf_f == NULL) print_err_and_exit(err2, -4);

    fprintf(f, "# Function: %s \n", gf_name);
    fprintf(f, "### References in the original script files\n");
    fprintf(f, "\n#\n\n");
    fprintf(f, "| File | Line Number | Reference code |\n");
    fprintf(f, "| --- | --- | --- |\n");

    while(!feof(gf_f)) {
        char filepath[4096];
        char filename[1024];
        char line_num[1024];
        char code[2048];

        // get file name
        bool stop = false;
        for(int i=0, j=0; !stop && i<1024; ++i) {
            char c = getc(gf_f);
            if(c == 0) c = getc(gf_f);
            if(c == ':' || c == '\t') stop = true;
            if(!stop && is_printable_ascii(c) && c !='\n') {filename[j] = c; j++;} else {filename[j] = 0;}
        }
        if(!stop) break;

        // get line number
        stop = false;
        for(int i=0, j=0; !stop && i<1024; ++i) {
            char c = getc(gf_f);
            if(c == 0) c = getc(gf_f);
            if(c == ':' || c == '\t') stop = true;
            if(!stop && is_printable_ascii(c) && c !='\n') {line_num[j] = c; j++;} else {line_num[j] = 0;}
        }
        if(!stop) break;

        // get line of code
        stop = false;
        bool start = false;
        for(int i=0, j=0; !stop && i<2048; ++i) {
            char c = getc(gf_f);
            if(c == '|') {code[j]='\\';j++;}
            if(c == '\n') {stop = true; code[j] = 0;}
            else if(c == '\t') start = true;
            else if(start && is_printable_ascii(c)) {code[j] = c;j++;}
        }
        sprintf(filepath, "../../../out/%s#L%s", filename, line_num);
        fprintf(f, "| [%s](%s) | %s | %s |\n", filename, filepath, line_num, code);
    }

    fclose(gf_f);
    fclose(f);
}
void write_game_functions_file() {
    char* dir = "./docs/catalog/";
    char* filename = "functions";
    char ext[] = ".md";

    int str_path_size = strlen(dir)+strlen(filename)+strlen(ext)+1;
    char fullpath[str_path_size];
    snprintf(fullpath, str_path_size, "%s%s%s", dir, filename, ext);

    FILE* f = fopen(fullpath, "wb");

    char err[256];
    sprintf(err, "Error, file %s, could not be opened.", fullpath);
    if(f == NULL) print_err_and_exit(err, -4);

    int counter = 0;
    for(int i=0; i<GAME_FUNCTIONS_NUM; ++i) {
        game_fun* gf = game_functions[i];
        if(gf->desc != NULL) counter++;
    }

    fprintf(f, "# Game Functions (Incomplete, %d/%d)\n", counter, GAME_FUNCTIONS_NUM);
    fprintf(f, "### Description\n");
    fprintf(f, "The game function calls are the main ingredient in the scripts, thats how we actually use the game's inner logic.\n \
        The important thing to remember is that these functions take arguments that are essential for the function to work properly.\n \
        If one of the arguments is wrong the game will produce an unexpected result or most likely will just crash.\n");
    fprintf(f, "\n#\n\n");
    fprintf(f, "*Note: the information provided here can be very inaccurate or even wrong, \
        these are based on experiments and guesswork. If you notice a mistake, please contact me to correct it.*\n");
    fprintf(f, "| #ID | String ID | N.o Arguments | Arguments | References |\n");
    fprintf(f, "| --- | --- | --- | --- | --- |\n");
    for(int i=0; i<GAME_FUNCTIONS_NUM; ++i) {
        game_fun* gf = game_functions[i];
        if(gf->desc != NULL) {
            char win_cmd[256];
            chdir("./out");
            sprintf(win_cmd, "findstr /n '%s(' * > %s", gf->name, "../gf.txt");
            system(win_cmd);
            chdir("..");
            FILE* gf_f = fopen(TEMP_GF_FILE, "rb");
            int lines = count_lines(gf_f);

            char* ref_dir = "./func_refs/";

            int ref_path_size = strlen(ref_dir)+strlen(gf->name)+strlen(ext)+1;
            char refpath[ref_path_size];
            snprintf(refpath, ref_path_size, "%s%s%s", ref_dir, gf->name, ext);

            fprintf(f, "| %d | [%s](%s) | %d | %s | %d |\n", gf->id, gf->name, refpath, gf->params, gf->desc, lines);
            write_game_functions_ref_file(gf->name);
        }
    }

    fprintf(f, "\n#\n\n");
    fprintf(f, "# Unnamed functions (yet to be identified and cataloged)\n");
    fprintf(f, "| #ID | String ID | N.o Arguments | Arguments | References |\n");
    fprintf(f, "| --- | --- | --- | --- | --- |\n");
    for(int i=0; i<GAME_FUNCTIONS_NUM; ++i) {
        game_fun* gf = game_functions[i];
        if(gf->desc == NULL) {
            char win_cmd[256];
            chdir("./out");
            sprintf(win_cmd, "findstr /n '%s(' * > %s", gf->name, "../gf.txt");
            system(win_cmd);
            chdir("..");
            FILE* gf_f = fopen(TEMP_GF_FILE, "rb");
            int lines = count_lines(gf_f);

            char* ref_dir = "./func_refs/";

            int ref_path_size = strlen(ref_dir)+strlen(gf->name)+strlen(ext)+1;
            char refpath[ref_path_size];
            snprintf(refpath, ref_path_size, "%s%s%s", ref_dir, gf->name, ext);

            fprintf(f, "| %d | [%s](%s) | - | - | %d |\n", gf->id, gf->name, refpath, lines);
            write_game_functions_ref_file(gf->name);
        }
    }

    // remove gf.txt
    remove(TEMP_GF_FILE);

    char msg[256];
    sprintf(msg, "successfully written to %s.", fullpath);
    print_success(msg);

    fclose(f);
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

    if(write_to_out_dir) {
        mark_auto_gen_file(sf);
    }
    write_asm_file(sf);

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
    game_vars = w_malloc(GAME_VARS_NUM*sizeof(game_var*));
    game_functions = w_malloc(GAME_FUNCTIONS_NUM*sizeof(game_fun*));
    code_patterns = w_malloc(CODE_PATTERNS_NUM*sizeof(code_pattern*));
    expr_patterns = w_malloc(EXPR_PATTERNS_NUM*sizeof(code_pattern*));
    init_game_vars(game_vars);
    init_game_functions(game_functions);
    init_code_patterns(code_patterns);
    init_expr_patterns(expr_patterns);

    bool save_to_out = false;
    for(int i=0; i < argc; i++) {
        char* arg = argv[i];
        if(strs_identical(arg, "-s")) {
            save_to_out = true;
        }
        if(strs_identical(arg, "-g")) {
            write_game_functions_file();
            exit(0);
        }
    }

    if(argc < 3) {
        printf(ANSI_COLOR_YELLOW "Usage: [operation (compile/ decompile)] [file path]\n" ANSI_COLOR_RESET);
        return 1;
    }

    char* filepath = argv[2];
    if(!is_filepath_valid(filepath)) {
        print_err_and_exit("ERROR, filepath is invalid.", -1);
        return 1;
    }

    // catalogs
    enemy_cat = w_malloc(sizeof(catalog*));
    handle_cat = w_malloc(sizeof(catalog*));
    item_cat = w_malloc(sizeof(catalog*));
    sound_cat = w_malloc(sizeof(catalog*));
    init_enemies_cat(enemy_cat);
    init_char_handle_cat(handle_cat);
    init_item_cat(item_cat);
    init_sound_cat(sound_cat);

    int op = 0;
    char* arg1 = argv[1];
    if(is_string(arg1, strlen(arg1))) {
        if(strs_identical(arg1, "compile")) {
            op = 1;
        } else if(strs_identical(arg1, "decompile")) {
            op = 0;
        } else {
            printf(ANSI_COLOR_YELLOW "Usage: [operation (compile/ decompile)] [file path]\nUnknown command %s.\n" ANSI_COLOR_RESET, arg1);
            exit(-1);
        }
    } else {
        op = atoi(argv[1]);
    }
    sct_f* sct_file;
    char* ext;
    switch(op) {
        case 0:
            ext = getFilenameExt(filepath);
            if(!(strs_identical(ext, ".sct") || strs_identical(ext, ".SCT"))) {
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
            if(!(strs_identical(ext, ".tsct") || strs_identical(ext, ".TSCT"))) {
                print_err_and_exit("Wrong extension. should be a .tsct file", -4);
            }

            sct_file = asm_file(filepath);
            write_sct_bin_file(filepath, sct_file);
            break;
    }

    return 0;
}
