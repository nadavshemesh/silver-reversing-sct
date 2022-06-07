#include "sct\sct.h"

bool is_filepath_valid(char* filepath) {
    regex_t reg;
    regmatch_t match[1];

    if (regcomp(&reg, ".*\\.t?sct", REG_EXTENDED) == -1) {
        print_err_and_exit("ERROR, regex could not compile.", -1);
        return false;
    }

    if(regexec(&reg, filepath, 1, match, REG_EXTENDED) == 0) {
        return true;
    } else {
        return false;
    }
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

void print_script(script* script) {
    printf("script_name: %s\n", script->name);
    printf("script_num: %d\n", script->number);
    printf("script_size_in_words: %d\n", script->size_in_words);
    printf("script_code_ptr: %d\n", script->script_code_ptr);
    printf("script_code_nodes_num: %d\n", script->code_nodes_num);
}

void print_data_section(sct_f* sf) {
    int size = sf->data_objs_size;
    data_obj** section = sf->data_section;
    for(int i=0; i < size; i++) {
        data_obj* data_o = section[i];
        print_data_obj(data_o);
    }
}

data_obj* inefficient_search_data_id(int id, sct_f* sf) {
    int ds_size = sf->data_objs_size;
    data_obj** ds = sf->data_section;
    data_obj* data_o = NULL;
    for(int i=0; i < ds_size; i++) {
        data_obj* cur_data_o = *ds + i;
        if(cur_data_o->id == id)
            return cur_data_o;
    }

    return data_o;
}

data_obj* get_data_obj_by_id(int id, sct_f* sf) {
    data_obj** ds = sf->data_section;

    if(ds == NULL) { print_err_and_exit("called get_data with no data section.", -3); }

    data_obj* data_o = inefficient_search_data_id(id, sf);
    if(data_o == NULL) { print_err_and_exit("called get_data with no data.", -3); }

    return data_o;
}
