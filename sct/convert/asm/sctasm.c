#include "sct\convert\asm\sctasm.h"

bool is_separator_char(char ch) {
    if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'
         || ch == '\0' || ch == '(' || ch == ')' || ch == ',')
        return true;
    return false;
}

bool is_special_char(char ch) {
    if(ch == '(' || ch == ')' || ch == ',' || ch == '{' || ch == '}')
        return true;
    return false;
}

int read_word(FILE* f, char* dest) {
    int c = getc(f);

    if(is_special_char(c)) {
        dest[0] = c;
        dest[1] = 0;
        return 0;
    }

    // if separator, get next non-separator char.
    if(is_separator_char(c)) {
        while(c != EOF && is_separator_char(c)) { c = getc(f); }
    }
    // make word
    int i = 0;
    while(c != EOF && !is_separator_char(c)) {
        dest[i] = c;
        c = getc(f);
        // printf("%d ", c);
        i++;
    }
    dest[i] = 0;
    // printf("%s ", dest);
    fseek(f, ftell(f)-1, SEEK_SET);
    if(is_separator_char(c)) return 0;
    return -1;
}

void file_seek_word_from_beginning(char* section_title, FILE* f) {
    fseek(f, 0, SEEK_SET);
    
    char token[MAX_ASM_TOKEN_LEN];
    int scan = read_word(f, token);
    // printf("looking for %s.\n", section_title);
    while(scan != -1) {
        // printf("word: %s\n", token);
        if(strlen(token) == strlen(section_title) && strncmp(token, section_title, strlen(token)) == 0) {
            printf("Found title: %s\n", token);
            return;
        }
        scan = read_word(f, token);
    }
    char err[256];
    sprintf(err, "section title '%s' not found in file.", section_title);
    print_err(err, -4);
}

char* file_seek_part_of_word(char* part, FILE* f) {
    char token[MAX_ASM_TOKEN_LEN];
    int scan = read_word(f, token);
    // printf("looking for %s.\n", section_title);
    while(scan != -1) {
        // printf("word: %s\n", token);
        if(strlen(token) >= strlen(part) && strncmp(token, part, strlen(part)) == 0) {
            // printf("Found part: %s\n", token);
            return aapts(token);
        }
        scan = read_word(f, token);
    }
    char err[256];
    // sprintf(err, "word part '%s' not found in file.", part);
    // print_err(err, -4);
    return NULL;
}

int count_text_words_until_string(FILE* f, char* str) {
    char token[MAX_ASM_TOKEN_LEN];
    int scan = read_word(f, token);
    int words_num = 0;
    while(scan != -1) {
        if(strlen(token) >= strlen(str) && strncmp(token, str, strlen(str)) == 0) {
            break;
        }
        scan = read_word(f, token);
        words_num++;
    }

    return words_num;
}

void print_asm_tokens(char** tokens, int tokens_len) {
    for(int i=0; i < tokens_len; i++) {
        printf("%s\n", tokens[i]);
    }
}

char** tokenize_section(char* section_title, sct_f* sf) {
    file_seek_word_from_beginning(section_title, sf->file);
    unsigned long section_pos = ftell(sf->file); // save the section offset

    int tokens_len = count_text_words_until_string(sf->file, "._");
    printf("tokens until next section: %d.\n", tokens_len);
    char** tokens = w_malloc((tokens_len)*sizeof(char*));
    fseek(sf->file, section_pos, SEEK_SET);
    for(int i=0; i < tokens_len; i++) {
        char token[MAX_ASM_TOKEN_LEN];
        read_word(sf->file, token);
        tokens[i] = aapts(token);
    }
    // print_asm_tokens(*tokens_ptr, tokens_len);
    return tokens;
}

int count_token_chars_from_to(char* token, char from, char to) {
    bool count = false;
    int counter = 0;
    int len = strlen(token);
    for(int i=0 ; i < len; i++) {
        if(count && token[i] == to) return counter;
        if(count) counter++;
        if(token[i] == from) count = true;
    }
    return -1;
}

int copy_token_chars_from_to(char* token, char* dest, char from, char to) {
    bool copy = false;
    int counter = 0;
    int len = strlen(token);
    for(int i=0, j=0; i < len; i++) {
        if(copy && token[i] == to) { 
            dest[j] = 0;
            return 0;
        }
        if(copy) { 
            dest[j] = token[i];
            j++;
        }
        if(token[i] == from) copy = true;
    }
    return -1;
}

int count_tokens_from_to(char** tokens_ptr, char* from, char* to) {
    bool count = false;
    int counter = 0;
    while(*tokens_ptr != NULL) {
        if(count && strlen(*tokens_ptr) == strlen(to) && strcmp(*tokens_ptr, to) == 0) {
            return counter;
        }
        if(count) counter++;
        if(strlen(*tokens_ptr) == strlen(from) && strcmp(*tokens_ptr, from) == 0) count = true;
        tokens_ptr++;
    }
    return -1;
}

data_obj* asm_create_data_obj(char*** tokens_pos_ptr, int index) {
    char** token_ptr = *tokens_pos_ptr;
    if(*token_ptr == NULL)
        print_err_and_exit("Error, data obj ptr is null.", -4);
    char* name = *token_ptr;
    token_ptr++;

    char* token = *token_ptr;
    data_obj* data_o = create_and_init_data_obj();
    switch(token[0]) {
        case '{': {
            int len = count_tokens_from_to(token_ptr, "{", "}");
            if(len == -1) {
                print_token_area_details(&token_ptr, MODE_ASM);
                print_err_and_exit("invalid int array", -4); 
            }
            // todo: validate structure
            token_ptr++;
            int integers_num = len - (len/2);
            // printf("array len: %d.\n", integers_num);
            int integers[integers_num];
            for(int i=0, j=0; i < len; i+=2, j++) {
                int num = atoi(*(token_ptr+i));
                integers[j] = num;
            }
            token_ptr+=len;
            // for(int i=0; i < integers_num; i++) {
            //     printf("%08x\n", integers[i]);
            // }

            data_o->id = index;
            data_o->byte_size = integers_num*sizeof(int);
            data_o->name = aapts(name);

            data_o->data = w_malloc(integers_num*sizeof(int));
            memcpy(data_o->data, integers, integers_num*sizeof(int));
            break;
        } 
        case '\'': {
            int len = count_token_chars_from_to(token, '\'', '\'');
            if(len == -1) {
                print_err("Error, invalid string", -4); 
                print_token_area_details(&token_ptr, MODE_ASM);
            }
            // printf("found string. len: %d\n", len);
            char str[len];
            copy_token_chars_from_to(token, str, '\'', '\'');
            // printf("string: %s\n", str);

            // char* asm_data = { aapts(token) };
            byte* data = (byte*) aapts(str);

            data_o->id = index;
            data_o->byte_size = len;
            data_o->name = aapts(name);

            // data_o->asm_data = w_malloc(sizeof(char*));
            data_o->data = w_malloc(len);

            // memcpy(data_o->asm_data, &asm_data, sizeof(data_o->asm_data));
            memcpy(data_o->data, data, sizeof(data_o->data));
            break;
        }

        default: {
            // todo: validate structure
            char* asm_data[1] = { aapts(token) };
            int data[1] = { atoi(token) };

            data_o->id = index;
            data_o->byte_size = 4;
            data_o->name = aapts(name);

            // data_o->asm_data = w_malloc(sizeof(char*));
            data_o->data = w_malloc(sizeof(int*));

            // memcpy(data_o->asm_data, &asm_data, sizeof(data_o->asm_data));
            memcpy(data_o->data, data, sizeof(data_o->data));
        }
    }
    token_ptr++;
    *tokens_pos_ptr = token_ptr;

    // print_data_obj(data_o);
    return data_o;
}

void build_data_section(sct_f* sf) {
    char** tokens;
    char*** tokens_pos_ptr;
    tokens = tokenize_section("._DATA", sf);
    tokens_pos_ptr = &tokens;

    int i = 0;
    node* data_next = NULL;
    node* data_nodes;
    while(**tokens_pos_ptr != NULL) {
        data_obj* data_o = asm_create_data_obj(tokens_pos_ptr, i);
        node* n = create_node(data_o);
        if(data_next == NULL) { 
            data_next = n;
            data_nodes = data_next;
        } else {
            data_next->next = n;
            data_next = n;
        }
        i++;
    }

    sf->data_objs_size = i;
    sf->data_section = w_malloc(i*sizeof(data_obj));
    data_next = data_nodes; // point to start again
    int j = 0;
    while(data_next != NULL) {
        sf->data_section[j] = data_next->item;
        data_next = data_next->next;
        j++;
    }
}

int count_file_sections(sct_f* sf) {
    unsigned long f_pos = ftell(sf->file);
    int sections_num = 0;
    fseek(sf->file, 0, SEEK_SET);
    char* found = file_seek_part_of_word("._", sf->file);
    while(found != NULL) {
        sections_num++;
        found = file_seek_part_of_word("._", sf->file);
    }
    fseek(sf->file, f_pos, SEEK_SET);
    return sections_num;
}

char** tokenize_next_section(sct_f* sf) {
    char** tokens;
    char* found = file_seek_part_of_word("._", sf->file);
    if(found == NULL) {
        fseek(sf->file, 0, SEEK_SET);
        found = file_seek_part_of_word("._", sf->file);
    }
    if(found == "._DATA") {
        found = file_seek_part_of_word("._", sf->file);
    }
    if(found != NULL && found != "._DATA") {
        //todo: validate code script structure
        tokens = tokenize_section(found, sf);
    } else {
        print_err_and_exit("Error, Couldn't find next section to tokenize.", -4);
    }

    return tokens;
}

bool asm_is_var_pos_expr(void* pattern, int index) {
    expr_pattern* p = (expr_pattern*) pattern;        

    int var_num = p->asm_var_num;
    int* var_pos = p->asm_var_pos;

    // printf("vpos: %d\n", *var_pos);
    for(int k=0; k < var_num; k++) {
        // printf("var_pos: %d, index: %d\n", var_pos[k], index);
        if(var_pos[k] == index) {
            return true;
        }
    }
    return false;
}

bool asm_is_var_pos(void* pattern, p_type pattern_type, int index) {
    code_pattern* p = (code_pattern*) pattern;        

    if(pattern_type == EXPRESSION_TYPE) {
        return asm_is_var_pos_expr(pattern, index);
    }

    int var_num = p->asm_var_num;
    int* var_pos = p->asm_var_pos;

    printf("vpos: %d\n", *var_pos);
    for(int k=0; k < var_num; k++) {
        printf("var_pos: %d, index: %d\n", var_pos[k], index);
        if(var_pos[k] == index) {
            return true;
        }
    }
    return false;
}

bool asm_cmp_token(char** a_token_ptr, char** b_token_ptr) {
    char** a_token = (char**) a_token_ptr;
    char** b_token = (char**) b_token_ptr;
    int res = strncmp(*a_token, *b_token, sizeof(*a_token));
    
    if(res == 0) {
        printf("%s == %s\n", *a_token, *b_token);
        return true;
    }
    printf("%s != %s\n", *a_token, *b_token);
    printf("res: %d\n", res);
    return false;
}

cp_cmp_result asm_identify_cp(char*** token_pos_ptr) {
    cp_cmp_result res = { .is_identified = false, .match = NULL};
    char** token;

    // compare each pattern with tokens
    for(int i=0; i < CODE_PATTERNS_NUM; i++) {
        code_pattern* cp = code_patterns[i];
        token = *token_pos_ptr;

        if(cp != NULL) {
           int token_num = cp->asm_token_num;
           int var_num = cp->asm_var_num;
           char** tokens = cp->asm_tokens;
           char* vars[var_num];
           int var_i = 0;
           int eq_goal = token_num - var_num;
           int eq_tokens = 0;
           // compare const tokens
           for(int j=0; j < token_num; j++) {
               if(!asm_is_var_pos(cp, CODE_TYPE, j)) {
                   if(!asm_cmp_token(tokens, token))  {
                       break; // not a match
                   }
                    eq_tokens++;
                } else {
                    vars[var_i] = *tokens;
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
                res.vars = w_malloc(var_num*sizeof(char*));
                memcpy(res.vars, vars, var_num*sizeof(char*));
                return res;
           }
       }
    }
    print_err("Error, code pattern not recognized.", 2);
    print_token_area_details(token, MODE_ASM);

    return res;
}

expr_cmp_result identify_expr(char*** token_pos_ptr) {
    expr_cmp_result res = { .is_identified = false, .match = NULL};
    char** token;

    // compare each pattern with tokens
    for(int i=0; i < EXPR_PATTERNS_NUM; i++) {
        expr_pattern* expr = expr_patterns[i];

        if(expr != NULL) {
           token = *token_pos_ptr;
           int token_num = expr->asm_token_num;
           int var_num = expr->asm_var_num;
           char** tokens = expr->asm_tokens;
           char* vars[var_num];
           
           int var_i = 0;
           int eq_goal = token_num - var_num;
           int eq_tokens = 0;
           // compare const tokens
           for(int j=0; j < token_num; j++) {
               if(!asm_is_var_pos(expr, EXPRESSION_TYPE, j)) {
                   if(!asm_cmp_token(tokens, token))  {
                       break; // not a match
                   }
                    eq_tokens++;
                } else {
                    vars[var_i] = *token;
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
                res.vars = w_malloc(var_num*sizeof(char*));
                memcpy(res.vars, vars, var_num*sizeof(char*));
                return res;
           }
       }
    }
    print_err("Error, expression not recognized.", 2);
    print_token_area_details(token, MODE_ASM);
    return res;
}

