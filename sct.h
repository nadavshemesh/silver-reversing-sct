#include<stdio.h>
#include<stdlib.h>

#include "utils.h"
#include "fileutils.h"
#include "gamefunc.h"

#define VAR -9
#define S_VAR "-9"
#define CODE_PATTERNS_NUM 11
#define PARAM_PATTERNS_NUM 3

const int MAX_FILENAME = 64;
const int MAX_PATH = 512;
const int MAX_DIR_PATH = 216;
const int MAX_VAR_NAME = 20;
const int MAX_TOKENS_PER_EXPRESSION = 50;

typedef enum mode { MODE_BIN, MODE_ASM } mode;
typedef enum o_type { OBJ_CODE, OBJ_DATA, OBJ_PARAM } o_type;
typedef enum pattern_type { CODE_TYPE, PARAM_TYPE } p_type;
typedef enum code_type { IF_STATEMENT, SWITCH, FUNCTION_CALL, SCRIPT_CALL, ASSIGNMENT, VAR_INC, VAR_DEC, TIMER, CODE_BLOCK } c_type;
typedef enum param_type { INTEGER, DATA_PTR, VAR_PTR } param_type;


typedef struct code_pattern {
    c_type type;
    char* name;
    char** var_names;
    int bin_var_num;
    int* bin_var_pos;
    int bin_token_num;
    int* bin_tokens;
    int asm_var_num;
    int* asm_var_pos;
    int asm_token_num;
    char** asm_tokens;
} code_pattern;

typedef struct cp_cmp_result {
    bool is_identified;
    code_pattern *match;
    void* tokens_pos;
    int var_num;
    void* vars;
} cp_cmp_result;

typedef struct data_object {
    int id;
    char* name;
    int references;
    int byte_size;
    int* bin_data;
    char** asm_data;
} data_obj;

typedef struct parameter_pattern {
    param_type type;
    int bin_var_num;
    int bin_token_num;
    int* bin_var_pos;
    int* bin_tokens;
    int asm_var_num;
    int* asm_var_pos;
    int asm_token_num;
    char** asm_tokens;
} param_pattern;

typedef struct parameter_object {
    param_pattern* pp;
    int* bin_vars;
    char** asm_vars;
    data_obj* data;
} param_obj;

typedef struct pp_cmp_result {
    bool is_identified;
    param_pattern *match;
    void* tokens_pos;
    int var_num;
    void* vars;
} pp_cmp_result;

typedef struct code_object {
    code_pattern* cp;
    int* bin_vars;
    char** asm_vars;
    int params_num;
    param_obj* params;
    struct code_object* nested_code_objs;
} code_obj;

typedef struct obj_and_token_ptr {
    void* obj;
    o_type type;
    void* token_ptr;
} obj_and_token_ptr;

// typedef struct code_block {
//     int size_in_words;
//     code_obj* code_objs;
// } code_block;

typedef struct script {
    int size_in_words;
    void** script_code_ptr;
    int code_obj_num;
    code_obj* code_objs;
    // code_block* code_blocks;
} script;

typedef unsigned char BYTE;

typedef struct sct_struct {
    int num_of_scripts;
    int main_script_off;
    int script_table_off;
    int script_table_size;
    int data_sec_off;
    int data_sec_size;
    int link_table_off;
    int link_table_size;
} sct_s;

typedef struct sct_file {
    FILE* file;
    sct_s* structure;
    int code_objs_size;
    script* scripts;
    int data_objs_size;
    data_obj* data_section;
} sct_f;

code_pattern* code_patterns[CODE_PATTERNS_NUM];
param_pattern* param_patterns[PARAM_PATTERNS_NUM];

sct_s* form_structure(FILE* sctfile);
void print_sct_struct(sct_f* sf);
int get_script_off(int script_num, sct_f* sf);
void print_tokens(int* bin_tokens, int length);
int disasm_file(char* filepath);
int disasm_script(int script_offset, sct_f* sf);
bool is_filepath_valid(char* filepath);





char* c_itoa(int num, char *str)
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}

void print_code_pattern(code_pattern* cp) {
    printf("cp_name: %s\n", cp->name);
    printf("bin_vars: %d - positions:", cp->bin_var_num);
    for(int i=0; i < cp->bin_var_num; i++) { printf(" %d ", cp->bin_var_pos[i]); }
    printf("\n");
    printf("asm_vars: %d - positions:", cp->asm_var_num);
    for(int i=0; i < cp->asm_var_num; i++) { printf(" %d ", cp->asm_var_pos[i]); }
    printf("\nvar_names:");
    for(int i=0; i < cp->bin_var_num; i++) { printf(" %s ", cp->var_names[i]); }
    printf("\n");
    printf("bin_tokens:");
    for(int i=0; i < cp->bin_token_num; i++) { printf(" %08x ", cp->bin_tokens[i]); }
    printf("\n");
    printf("asm_tokens:");
    for(int i=0; i < cp->asm_token_num; i++) { printf(" %s ", cp->asm_tokens[i]); }
    printf("\n");
}

void init_cp(code_pattern* cp, c_type type, i_arr bin_tokens, i_arr bin_var_pos, s_arr var_names,
                                            s_arr asm_tokens, i_arr asm_var_pos) {
    cp->bin_tokens = w_malloc(bin_tokens.len*sizeof(int*));
    cp->bin_var_pos = w_malloc(bin_var_pos.len*sizeof(int*));
    cp->asm_tokens = w_malloc(asm_tokens.len*sizeof(char*));
    cp->asm_var_pos = w_malloc(asm_var_pos.len*sizeof(int*));
    cp->var_names = w_malloc(var_names.len*sizeof(char*));

    cp->type = type;
    cp->bin_token_num = bin_tokens.len;
    cp->asm_token_num = asm_tokens.len;
    cp->bin_var_num = bin_var_pos.len;
    cp->asm_var_num = asm_var_pos.len;

    memcpy(cp->bin_tokens, bin_tokens.arr, cp->bin_token_num*sizeof(int*));
    memcpy(cp->bin_var_pos, bin_var_pos.arr, cp->bin_var_num*sizeof(int*));
    memcpy(cp->asm_tokens, asm_tokens.arr, cp->asm_token_num*sizeof(char*));
    memcpy(cp->asm_var_pos, asm_var_pos.arr, cp->asm_var_num*sizeof(int*));
    for(int i=0; i < var_names.len; i++) {
        cp->var_names[i] = aapts(var_names.arr[i]);
    }
}

code_pattern* init_cp_func_call() {
    code_pattern* cp_func_call = (code_pattern*) malloc(sizeof(code_pattern));

    cp_func_call->name = aapts("call_game_function");
    i_arr bin_tokens = { .arr = {0, 3, VAR, VAR}, .len = 4 };
    i_arr bin_var_pos = { .arr = {2, 3}, .len = 2 };
    s_arr asm_tokens = { .arr = { "call", S_VAR }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"func_name", "num_of_params"}, .len = 2 };
    
    init_cp(cp_func_call, FUNCTION_CALL, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_func_call;
}

code_pattern* init_cp_if_var_int() {
    code_pattern* cp_if = (code_pattern*) malloc(sizeof(code_pattern));

    cp_if->name = aapts("compare var with integer value");
    i_arr bin_tokens = { .arr = {1, 0, 0xfffffffd, 3, 0, 6, VAR, 4, VAR, VAR, VAR}, .len = 11 };
    i_arr bin_var_pos = { .arr = {6, 8, 9, 10}, .len = 4 };
    s_arr asm_tokens = { .arr = { "if", "(", "var", S_VAR, S_VAR, "int", S_VAR, ")" }, .len = 8 };
    i_arr asm_var_pos = { .arr = { 2, 3, 4 }, .len = 3 };
    s_arr var_names = { .arr = {"var_ptr", "== , >= , <=, >, <, !=", "int value"}, .len = 2 };
    
    init_cp(cp_if, IF_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_if;
}

code_pattern* init_cp_if_game_var_int() {
    code_pattern* cp_if = (code_pattern*) malloc(sizeof(code_pattern));

    cp_if->name = aapts("compare game_var with integer value");
    i_arr bin_tokens = { .arr = {1, 0, 0xfffffffd, 3, 0, 3, VAR, 4, VAR, VAR, VAR}, .len = 11 };
    i_arr bin_var_pos = { .arr = {6, 8, 9, 10}, .len = 4 };
    s_arr asm_tokens = { .arr = { "if", "(", "game_var", S_VAR, S_VAR, "int", S_VAR, ")" }, .len = 8 };
    i_arr asm_var_pos = { .arr = { 2, 3, 4 }, .len = 3 };
    s_arr var_names = { .arr = {"game_var", "== , >= , <=, >, <, !=", "int value"}, .len = 2 };
    
    init_cp(cp_if, IF_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_if;
}

code_pattern* init_cp_if_var_var() {
    code_pattern* cp_if = (code_pattern*) malloc(sizeof(code_pattern));

    cp_if->name = aapts("compare var with another var");
    i_arr bin_tokens = { .arr = {1, 0, 0xfffffffd, 3, 0, 6, VAR, 4, VAR, VAR, 0, 6, VAR}, .len = 13 };
    i_arr bin_var_pos = { .arr = {6, 8, 9, 12}, .len = 4 };
    s_arr asm_tokens = { .arr = { "if", "(", "var", S_VAR, S_VAR, "var", S_VAR, ")" }, .len = 8 };
    i_arr asm_var_pos = { .arr = { 2, 3, 4 }, .len = 3 };
    s_arr var_names = { .arr = {"var_ptr", "== , >= , <=, >, <, !=", "var_ptr"}, .len = 2 };
    
    init_cp(cp_if, IF_STATEMENT, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_if;
}

code_pattern* init_cp_var_inc() {
    code_pattern* cp_inc = (code_pattern*) malloc(sizeof(code_pattern));

    cp_inc->name = aapts("increment variable by 1");
    i_arr bin_tokens = { .arr = { 0, 6, VAR, 7, 0}, .len = 5 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { S_VAR, "++" }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"var_ptr"}, .len = 1 };
    
    init_cp(cp_inc, VAR_INC, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_inc;
}

code_pattern* init_cp_var_dec() {
    code_pattern* cp_dec = (code_pattern*) malloc(sizeof(code_pattern));

    cp_dec->name = aapts("decrement variable by 1");
    i_arr bin_tokens = { .arr = { 0, 6, VAR, 7, 1}, .len = 5 };
    i_arr bin_var_pos = { .arr = { 2 }, .len = 1 };
    s_arr asm_tokens = { .arr = { S_VAR, "--" }, .len = 2 };
    i_arr asm_var_pos = { .arr = { 1 }, .len = 1 };
    s_arr var_names = { .arr = {"var_ptr"}, .len = 1 };
    
    init_cp(cp_dec, VAR_DEC, bin_tokens, bin_var_pos, var_names, asm_tokens, asm_var_pos);

    return cp_dec;
}

void init_code_patterns() {
    for(int i=0; i < CODE_PATTERNS_NUM; i++){
        code_patterns[i] = NULL;
    }
    code_pattern* cp_fc = init_cp_func_call();
    code_pattern* cp_if_int = init_cp_if_var_int();
    code_pattern* cp_if_gf_int = init_cp_if_game_var_int();
    code_pattern* cp_if_var_var = init_cp_if_var_var();
    code_pattern* cp_var_inc = init_cp_var_inc();
    code_pattern* cp_var_dec = init_cp_var_dec();

    code_patterns[0] = cp_var_inc;
    code_patterns[1] = cp_var_dec;
    code_patterns[2] = cp_if_int;
    code_patterns[3] = cp_fc;
    code_patterns[4] = cp_if_gf_int;
    code_patterns[5] = cp_if_var_var;
    // print_code_pattern(cp_fc);
}
void free_code_patterns() {
    for(int i=0; i<sizeof(code_patterns); i++) {
        free(code_patterns[i]);
    }
}

void print_param_pattern(param_pattern* pp) {
    printf("num_of_vars: %d - positions:", pp->bin_var_num);
    for(int i=0; i < pp->bin_var_num; i++) { printf(" %d ", pp->bin_var_pos[i]); }
    printf("\n");
    printf("bin_tokens:");
    for(int i=0; i < pp->bin_token_num; i++) { printf(" %08x ", pp->bin_tokens[i]); }
    printf("\n");
    printf("asm_tokens:");
    for(int i=0; i < pp->asm_token_num; i++) { printf(" %s ", pp->asm_tokens[i]); }
    printf("\n");
}

param_pattern* init_pp_integer() {
    param_pattern* pp = w_malloc(sizeof(param_pattern));
    pp->type = INTEGER;
    pp->bin_token_num = 4;
    pp->bin_var_num = 1;
    pp->asm_token_num = 2;
    pp->asm_var_num = 1;

    int bin_tokens[4] = { 0xfffffffd, 1, 2, VAR };
    int bin_var_pos[1] = { 3 };
    char* asm_tokens[2] = { "int", S_VAR };
    int asm_var_pos[1] = { 1 };

    pp->bin_tokens = w_malloc(pp->bin_token_num*sizeof(int*));
    pp->bin_var_pos = w_malloc(pp->bin_var_num*sizeof(int*));
    pp->asm_tokens = w_malloc(pp->asm_token_num*sizeof(char*));
    pp->asm_var_pos = w_malloc(pp->asm_var_num*sizeof(int*));

    memcpy(pp->bin_tokens, bin_tokens, pp->bin_token_num*sizeof(int*));
    memcpy(pp->bin_var_pos, bin_var_pos, pp->bin_var_num*sizeof(int*));
    memcpy(pp->asm_tokens, asm_tokens, pp->asm_token_num*sizeof(char*));
    memcpy(pp->asm_var_pos, asm_var_pos, pp->asm_var_num*sizeof(int*));

    return pp;
}

param_pattern* init_pp_data_ptr() {
    param_pattern* pp = w_malloc(sizeof(param_pattern));
    pp->type = DATA_PTR;
    pp->bin_token_num = 5;
    pp->asm_token_num = 2;
    pp->bin_var_num = 1;
    pp->asm_var_num = 1;

    int bin_tokens[5] = { 0xfffffffd, 1, 0, 7, VAR };
    int bin_var_pos[1] = { 4 };
    char* asm_tokens[2] = { "const", S_VAR };
    int asm_var_pos[1] = { 1 };

    pp->bin_tokens = w_malloc(pp->bin_token_num*sizeof(int*));
    pp->bin_var_pos = w_malloc(pp->bin_var_num*sizeof(int*));
    pp->asm_tokens = w_malloc(pp->asm_token_num*sizeof(int*));
    pp->asm_var_pos = w_malloc(pp->asm_var_num*sizeof(int*));

    memcpy(pp->bin_tokens, bin_tokens, pp->bin_token_num*sizeof(int*));
    memcpy(pp->bin_var_pos, bin_var_pos, pp->bin_var_num*sizeof(int*));
    memcpy(pp->asm_tokens, asm_tokens, sizeof(pp->asm_tokens));
    memcpy(pp->asm_var_pos, asm_var_pos, pp->asm_var_num*sizeof(int*));

    return pp;
}

param_pattern* init_pp_var_ptr() {
    param_pattern* pp = w_malloc(sizeof(param_pattern));
    pp->type = VAR_PTR;
    pp->bin_token_num = 5;
    pp->bin_var_num = 1;
    pp->asm_token_num = 2;
    pp->asm_var_num = 1;

    int bin_tokens[5] = { 0xfffffffd, 1, 0, 6, VAR };
    int bin_var_pos[1] = { 4 };
    char* asm_tokens[2] = { "var", S_VAR };
    int asm_var_pos[1] = { 1 };

    pp->bin_tokens = w_malloc(pp->bin_token_num*sizeof(int*));
    pp->bin_var_pos = w_malloc(pp->bin_var_num*sizeof(int*));
    pp->asm_tokens = w_malloc(pp->asm_token_num*sizeof(char*));
    pp->asm_var_pos = w_malloc(pp->asm_var_num*sizeof(int*));

    memcpy(pp->bin_tokens, bin_tokens, pp->bin_token_num*sizeof(int*));
    memcpy(pp->bin_var_pos, bin_var_pos, pp->bin_var_num*sizeof(int*));
    memcpy(pp->asm_tokens, asm_tokens, pp->asm_token_num*sizeof(char*));
    memcpy(pp->asm_var_pos, asm_var_pos, pp->asm_var_num*sizeof(int*));

    return pp;
}

void init_param_patterns() {
    for(int i=0; i < PARAM_PATTERNS_NUM; i++){
        param_patterns[i] = NULL;
    }

    param_pattern* pp_int = init_pp_integer();
    param_pattern* pp_data_ptr = init_pp_data_ptr();
    param_pattern* pp_var_ptr = init_pp_var_ptr();

    param_patterns[0] = pp_int;
    param_patterns[1] = pp_data_ptr;
    param_patterns[2] = pp_var_ptr;
    // print_param_pattern(pp_int);
}

void print_data_obj(data_obj* data_o) {
    printf("data_obj id: %d\n", data_o->id);
    printf("references: %d\n", data_o->references);
    printf("size in bytes: %d\n", data_o->byte_size);
    printf("bin_data: ");
    for(int i=0; i < data_o->byte_size/4; i++) {
        printf(" %08x ", *(data_o->bin_data+i));
    }
    printf(" [%s] ", data_o->bin_data);
    printf("\n");
}

int compare_data_obj_ids(const void* a, const void* b) {
     int int_a = (((data_obj*)a)->id);
     int int_b = (((data_obj*)b)->id);

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

void build_data_from_link_table(sct_f* sct) {
    int offset = (sct->structure->link_table_off)+4;
    int links_num = ((sct->structure->link_table_size)/4);
    // printf("links num: %d\n", links_num);
    int data_ref_size = 0;
    int ref_is_data[links_num];
    int ref_count[links_num];
    int refs[links_num];

    fseek(sct->file, offset, SEEK_SET);
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
    for(int i=0; i < links_num; i++) { if(ref_count[i] > 0) data_ref_size++; } 
    // printf("data_ref_size: %08x\n", data_ref_size);

    // create data objects
    data_obj data_arr[data_ref_size];
    for(int i=0, j=0; i < links_num; i++) {
        if(ref_count[i] > 0) {
            data_arr[j].id = i;
            // printf("data id: %d\n", i);
            data_arr[j].references = ref_count[i];
            // printf("ref count: %d\n", ref_count[i]);
            j++;
        }
    }
    // sort data_arr by id
    qsort(data_arr, data_ref_size, sizeof(data_obj), compare_data_obj_ids);

    // calculate data obj size and data
    for(int i=0; i < data_ref_size-1; i++) {
        int data_section_offset = (sct->structure->data_sec_off) + 4;
        int data_offset = data_section_offset + data_arr[i].id*4;
        int byte_size = (data_arr[i+1].id - data_arr[i].id)*4;

        data_arr[i].byte_size = byte_size;
        int data[byte_size];

        fseek(sct->file, data_offset, SEEK_SET);
        fread(&data, 1, byte_size, sct->file);
        // printf("%08x\n", data[0]);

        //TODO: ASM DATA
        char prefix[] = "VAR_";
        char name[sizeof(prefix)+4];
        sprintf(name, "%s%d", prefix, data_arr[i].id);
        data_arr[i].name = aapts(name);
        data_arr[i].bin_data = w_malloc(byte_size);
        memset(data_arr[i].bin_data, 0, byte_size);
        memcpy(data_arr[i].bin_data, data, byte_size);
        // memcpy(data_arr[i].asm_data, asm_data, byte_size);
    }

    data_obj* data_objs = w_malloc(data_ref_size*sizeof(data_obj));
    memcpy(data_objs, data_arr, data_ref_size*sizeof(data_obj));
    // print_data_obj((&data_objs[5]));

    sct->data_objs_size = data_ref_size;
    sct->data_section = data_objs;
    // print_data_obj(&(sct->data_section)[5]);
}

data_obj* get_data_obj_by_id(int id, sct_f* sf) {
    data_obj* ds = sf->data_section;
    if(ds == NULL) { print_err_and_exit("called get_data with no data section.", -3); }
    if((&ds)[id]->bin_data == NULL) { print_err_and_exit("called get_data with no data.", -3); }

    return &(ds)[id];
}

void print_param_obj(param_obj* po, bool with_pattern) {
    print_title("PARAM_OBJ");
    if(with_pattern)
        print_param_pattern(po->pp);

    printf("bin_vars: ");
    for(int i=0; i < po->pp->bin_var_num; i++) { printf(" %08x ", po->bin_vars[i]); }
    printf("asm_vars: ");
    for(int i=0; i < po->pp->asm_var_num; i++) { printf(" %s ", po->asm_vars[i]); }
    printf("\n");

    print_data_obj(po->data);
}

void print_code_obj(code_obj* co) {
    print_title("CODE_OBJ");
    print_code_pattern(co->cp);
    printf("asm vars:");
    for(int i=0; i < co->cp->asm_var_num; i++) { 
        printf(" %s ", co->asm_vars[i]);
    }
    printf("\nbin vars:");
    for(int i=0; i < co->cp->bin_var_num; i++) { 
        printf(" %08x ", co->bin_vars[i]);
    }
    printf("params num: %d\n", co->params_num);
    for(int i=0; i < co->params_num; i++) {
        param_obj* param = &(co->params)[i];
        print_param_obj(param, true);
    }
}
