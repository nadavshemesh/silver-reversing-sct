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

int get_script_offset(int script_num, sct_f* sf) {
    if(script_num >= sf->structure->num_of_scripts) {
        printf("script #%d does'nt exist, there are %d scripts.\n", script_num, sf->structure->num_of_scripts);
        return -2;
    }
    int* script_table = sf->script_table;
    if(script_table == NULL) print_err_and_exit("Error, script table is undefined.", -4);
    int script_offset = script_table[script_num];

    return script_offset*4+0x20;
}

int get_script_num_by_offset(int offset, sct_f* sf) {
    for(int i=0; i < sf->structure->num_of_scripts; i++) {
        int s_offset = sf->script_table[i];
        if(s_offset*4+0x20 == offset) {
            return i;
        }
    }

    return -1;
}

int cmp_nums (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int get_script_offset_sorted(int script_num, sct_f* sf) {
    int num_of_scripts = sf->structure->num_of_scripts;
    if(script_num >= num_of_scripts) {
        printf("script #%d does'nt exist, there are %d scripts.\n", script_num, sf->structure->num_of_scripts);
        return -2;
    }
    int script_table[num_of_scripts];
    if(sf->script_table == NULL) print_err_and_exit("Error, script table is undefined.", -4);

    memcpy(script_table, sf->script_table, num_of_scripts*sizeof(int));
    qsort(script_table, num_of_scripts, sizeof(int), cmp_nums);
    
    int script_offset = script_table[script_num];
    return script_offset*4+0x20;
}

void print_script_table_offsets(sct_f* sf) {
    for(int i=0; i < sf->scripts_num; i++) {
        printf("script offset: %08x\n", sf->script_table[i]);
    }
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
    int size = sf->data_objs_num;
    data_obj** section = sf->data_section;
    for(int i=0; i < size; i++) {
        data_obj* data_o = section[i];
        print_data_obj(data_o);
    }
}

void print_bin_script_table(sct_f* sf) {
    int* st = sf->script_table;
    if(sf->script_table == NULL) { print_err_and_exit("Error, script table undefined.", -4); }

    for(int i=0; i < sf->scripts_num; i++) {
        printf("%08x", sf->script_table[i]);
    }
}

void print_bin_link_table(sct_f* sf) {
    print_title("LINK_TABLE");
    node** link_nodes = sf->data_link_table;
    if(link_nodes == NULL) {  print_err_and_exit("Error, data link table undefined.", -4); }
    node* link_node = *link_nodes;

    while(link_node != NULL) {
        printf("%08x", *((int*)link_node->item));
        link_node = link_node->next;
    }
    printf("\n");
}

data_obj* inefficient_search_data_id(int id, sct_f* sf) {
    int ds_size = sf->data_objs_num;
    data_obj** ds = sf->data_section;
    data_obj* data_o = NULL;
    for(int i=0; i < ds_size; i++) {
        data_obj* cur_data_o = ds[i];
        if(cur_data_o->id == id)
            return cur_data_o;
    }

    return data_o;
}

data_obj* search_data_by_name(char* name, sct_f* sf) {
    int ds_size = sf->data_objs_num;
    data_obj** ds = sf->data_section;
    data_obj* data_o = NULL;
    for(int i=0; i < ds_size; i++) {
        data_obj* cur_data_o = ds[i];
        if(strlen(cur_data_o->name) == strlen(cur_data_o->name) && strcmp(cur_data_o->name, name) == 0)
            return cur_data_o;
    }

    return data_o;
}

data_obj* get_data_obj_by_id(int id, sct_f* sf) {
    data_obj** ds = sf->data_section;

    if(ds == NULL) { print_err_and_exit("called get_data with no data section.", -3); }

    data_obj* data_o = inefficient_search_data_id(id, sf);
    if(data_o == NULL) { 
        print_data_section(sf);
        printf("data id: %d\n", id);
        print_err_and_exit("called get_data with no data.", -3);
    }

    return data_o;
}

data_obj* get_data_obj_by_name(char* name, bool silent_err, sct_f* sf) {
    data_obj** ds = sf->data_section;

    if(ds == NULL) { print_err_and_exit("called get_data with no data section.", -3); }

    data_obj* data_o = search_data_by_name(name, sf);
    if(data_o == NULL) { 
        if(!silent_err) {
            print_data_section(sf);
            printf("data name: %s\n", name);
            print_err_and_exit("called get_data with no data.", -3);
        }
        return NULL;
    }

    return data_o;
}
