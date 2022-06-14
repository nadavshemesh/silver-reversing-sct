#include "sct\convert\asm\sctasm.h"

bool is_separator_char(char ch) {
    if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'
         || ch == '\0' || ch == '(' || ch == ')' || ch == ',' || ch == '+' || ch == '=')
        return true;
    return false;
}

bool is_special_char(char ch) {
    if(ch == '(' || ch == ')' || ch == ',' || ch == '{' || ch == '!' ||
        ch == '}' || ch == '+' || ch == '>' || ch == '<' || ch == '=')
        return true;
    return false;
}

char peek_next_char(FILE* f) {
    unsigned long f_pos = ftell(f);
    char c = getc(f);
    fseek(f, f_pos, SEEK_SET);

    return c;
}

bool is_char_paranth(char c) {
    return (c == '(' || c == ')');
}

int read_word(FILE* f, char* dest) {
    int c = getc(f);

    // if separator, get next non-separator char.
    if(is_separator_char(c)) {
        while(c != EOF && is_separator_char(c) && !is_special_char(c)) { c = getc(f); }
    }

    // handle string
    if(c == '\'') {
        dest[0] = c;
        c = getc(f);
        int i = 1;
        while(c != EOF && c != '\'' && i < MAX_ASM_TOKEN_LEN) {
            // printf("%c ", c);
            dest[i] = c;
            c = getc(f);
            i++;
        }
        dest[i] = c;
        dest[i+1] = 0;
        c = getc(f);
        fseek(f, ftell(f)-1, SEEK_SET);
        return 0;
    }

    if(is_special_char(c)) {
        int index = 0;
        if(is_special_char(peek_next_char(f)) && !is_char_paranth(c)) { // such as (ie: ++, --, ==, >=)
            dest[index] = c;
            index++;
            c = getc(f);
        } 
        dest[index] = c;
        dest[index+1] = 0;
        return 0;
    }

    // ignore one line comments
    while(c == '/' && peek_next_char(f) == '/') {
        // printf("found comment\n");
        while(c != '\n') {
            c = getc(f);
        }
        c = getc(f);
        while(c == '\t' || c == '\n' || c == '\r' || c == ' ') {
            c = getc(f);
        }
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
            // printf("Found title: %s\n", token);
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
    unsigned long section_pos = ftell(f); // save the section offset
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

    fseek(f, section_pos, SEEK_SET);
    return words_num;
}

void print_asm_tokens(char** tokens, int tokens_len) {
    for(int i=0; i < tokens_len; i++) {
        printf("%s\n", tokens[i]);
    }
}

int count_next_section_tokens(sct_f* sf) {
    int num = 0;
    unsigned long f_pos = ftell(sf->file);

    file_seek_part_of_word("._", sf->file);
    num = count_text_words_until_string(sf->file, "._");

    fseek(sf->file, f_pos, SEEK_SET);

    return num;
}

char** tokenize_section(char* section_title, sct_f* sf) {
    file_seek_word_from_beginning(section_title, sf->file);

    int tokens_len = count_text_words_until_string(sf->file, "._");
    // printf("tokens until next section: %d.\n", tokens_len);
    char** tokens = w_malloc((tokens_len+5)*sizeof(char*)); // +5 for padding 
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

int count_tokens_from_to_curly_paranth(char** tokens_ptr) {
    bool count = false;
    int counter = 0;
    int matches = 0;
    while(*tokens_ptr != NULL) {
        if(count && strlen(*tokens_ptr) == 1 && strcmp(*tokens_ptr, "}") == 0) {
            matches--;
            if(matches == 0) {
                return counter;
            }
        }
        if(count) {
            counter++;
        }
        if(strlen(*tokens_ptr) == 1 && strcmp(*tokens_ptr, "{") == 0) { 
            count = true;
            matches++;
        }
        tokens_ptr++;
    }
    return -1;
}

int count_tokens_in_code_block(char*** tokens_pos_ptr) {
    char** tokens = *tokens_pos_ptr;
    int tokens_num = count_tokens_from_to_curly_paranth(tokens);

    return tokens_num;
}

void build_scripts_lables_and_order(sct_f* sf) {
    unsigned long f_pos = ftell(sf->file);
    int code_sections_num = count_file_sections(sf)-1;
    char** script_labels = w_malloc(code_sections_num*sizeof(char*));
    char data_section_name[] = "._DATA";

    for(int i=0; i < code_sections_num;) {
        char* name = file_seek_part_of_word("._", sf->file);
        if(!(strlen(name) == strlen(data_section_name) && strcmp(name, data_section_name) == 0)) {
            script_labels[i] = aapts(name);
            // printf("script found: %s, id: %d\n", name, i);
            i++;
        }
    }
    fseek(sf->file, f_pos, SEEK_SET);

    sf->scripts_num = code_sections_num;
    sf->scripts_labels = script_labels;
}

int get_script_id_by_label(char* label, sct_f* sf) {
    char prefix[] = "._";
    char full_label[strlen(prefix)+strlen(label)];
    sprintf(full_label, "%s%s", prefix, label); 
    char** script_labels = sf->scripts_labels;
    if(script_labels == NULL) print_err_and_exit("Error, no script lables found.", -4);

    for(int i=0; i < sf->scripts_num; i++) {
        if(strlen(full_label) == strlen(script_labels[i]) 
            && strcmp(full_label, script_labels[i]) == 0) {
                return i;
            }
    }
    return -1; // not found
}

char* get_script_label_by_id(int id, sct_f* sf) {
    char** script_labels = sf->scripts_labels;
    if(script_labels == NULL) print_err_and_exit("Error, no script lables found.", -4);
    char* script_label = script_labels[id];
    if(script_label != NULL && script_label != 0) {
        return script_label;
    }
    return NULL; // not found
}

int count_token_from_to(char** tokens_ptr, char* token, char* from, char* to) {
    bool count = false;
    int counter = 0;
    int matches = 0;
    while(*tokens_ptr != NULL) {
        if(count && strlen(*tokens_ptr) == strlen(to) && strcmp(*tokens_ptr, to) == 0) {
            matches--;
            if(matches == 0)
                return counter;
        }
        if(count && strlen(*tokens_ptr) == strlen(token) && strcmp(*tokens_ptr, token) == 0) {
            counter++;
        }
        if(strlen(*tokens_ptr) == strlen(from) && strcmp(*tokens_ptr, from) == 0) {
            count = true;
            matches++;
        }
        tokens_ptr++;
    }
    return -1;
}

unsigned long get_sct_code_word_count(sct_f* sf) {
    return sf->structure->code_section_word_counter;
}

void add_sct_bin_words(int bin_words_num, sct_f* sf) {
    sf->structure->code_section_word_counter += bin_words_num;
}

void add_sct_bin_words_prologue(sct_f* sf) {
    sf->structure->code_section_word_counter += 2;
}

void add_sct_bin_words_switch_body(sct_f* sf) {
    sf->structure->code_section_word_counter += 0x81;
}

data_obj* asm_create_data_obj(char*** tokens_pos_ptr, int id) {
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
                print_token_area_details(token_ptr, MODE_ASM);
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

            data_o->id = id;
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
                print_token_area_details(token_ptr, MODE_ASM);
            }
            // printf("found string. len: %d\n", len);
            char str[len];
            copy_token_chars_from_to(token, str, '\'', '\'');
            // printf("string: %s\n", str);

            // char* asm_data = { aapts(token) };
            byte* data = (byte*) aapts(str);

            data_o->id = id;
            len += 1; // +1 for null byte
            // printf("len: %d\n", len);
            if(len % 4 != 0) len += 4-(len % 4); // padding
            // printf("len with padding: %d\n", len);
            data_o->byte_size = len; 
            data_o->name = aapts(name);

            // data_o->asm_data = w_malloc(sizeof(char*));
            data_o->data = w_malloc(len);

            // memcpy(data_o->asm_data, &asm_data, sizeof(data_o->asm_data));
            memcpy(data_o->data, data, len);
            break;
        }

        default: {
            // todo: validate structure
            char* asm_data[1] = { aapts(token) };
            int data[1] = { atoi(token) };

            data_o->id = id;
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
    sf->structure->data_sec_size = 0;
    char** tokens;
    char*** tokens_pos_ptr;
    tokens = tokenize_section("._DATA", sf);
    tokens_pos_ptr = &tokens;

    int i = 0;
    node* data_next = NULL;
    node* data_nodes;
    while(**tokens_pos_ptr != NULL) {
        int ds_size = sf->structure->data_sec_size;
        int id = (ds_size % 4 == 0) ? ds_size/4 : (ds_size/4+1);
        // printf("id: %08x\n", id);
        data_obj* data_o = asm_create_data_obj(tokens_pos_ptr, id);
        sf->structure->data_sec_size += data_o->byte_size;
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
    sf->data_objs_num = i;
    sf->data_section = w_malloc(i*sizeof(data_obj*));
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

    // printf("vpos: %d\n", *var_pos);
    for(int k=0; k < var_num; k++) {
        // printf("var_pos: %d, index: %d\n", var_pos[k], index);
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
        // printf("%s == %s\n", *a_token, *b_token);
        return true;
    }
    // printf("%s != %s\n", *a_token, *b_token);
    return false;
}

cp_cmp_result asm_identify_cp(char*** token_pos_ptr, sct_f* sf) {
    cp_cmp_result res = { .is_identified = false, .match = NULL};
    char** token;

    // compare each pattern with tokens
    for(int i=0; i < CODE_PATTERNS_NUM; i++) {
        code_pattern* cp = code_patterns[i];
        token = *token_pos_ptr;

        if(cp != NULL && cp->asm_token_num > 0) {
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
                    vars[var_i] = aapts(*token);
                    // printf("found var: %s\n", *token);
                    var_i++;
                }
                tokens++;
                token++;
           }
           if(eq_tokens == eq_goal) {
                int bin_tokens_num = cp->bin_token_num + cp->bin_extra_token_num;
                add_sct_bin_words(bin_tokens_num, sf);
                // printf("Added %02x tokens for '%s'.\n", bin_tokens_num, cp->name);
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

expr_cmp_result asm_identify_expr(char*** token_pos_ptr, sct_f* sf) {
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
           if(eq_tokens == eq_goal && var_i == var_num) {
            //    printf("last token: %s\n", *token);
                int bin_tokens_num = expr->bin_token_num;
                add_sct_bin_words(bin_tokens_num, sf);
                // printf("Added %02x tokens for '%s'.\n", bin_tokens_num, expr->name);
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
    // print_asm_tokens(token, 5);
    print_token_area_details(token, MODE_ASM);
    return res;
}

bool is_expr_token_binary_op(char** token) {
    if(token == NULL || *token == NULL) return false;
    // printf("checking binary op: %s\n", *token);
    for(int i=0; i < EXPR_PATTERNS_NUM; i++) {
        expr_pattern* pattern = expr_patterns[i];
        if(pattern->type == OPERATOR && 
            strlen(*pattern->asm_tokens) == strlen(*token) && 
            strcmp(*pattern->asm_tokens, *token) == 0) {
                return true;
            }
    }
    return false;
}

void create_data_link(sct_f* sf) {
    node** link_nodes = sf->data_link_table;
    if(link_nodes == NULL) {  print_err_and_exit("Error, data link table undefined.", -4); }

    int* offset = w_malloc(sizeof(int));
    *offset = sf->structure->code_section_word_counter-1;
    *offset |= 0x40000000;

    node* link_node = *link_nodes;
    node* new_link_node = create_node(offset); 
    if(sf->structure->link_table_size == 0 || link_node == NULL) {
        *sf->data_link_table = new_link_node;
    } else {
        insert_node(link_nodes, new_link_node);
    }
    sf->structure->link_table_size++;
}

expr_obj* asm_create_expr_obj(expr_pattern* expr_p, char** vars, char*** token_pos_ptr, sct_f* sf) {
    expr_obj* eo = create_and_init_expr_obj();

    eo->expr_p = expr_p;
    switch (expr_p->type) {
        case INTEGER:
            // todo: validate int
            int num = atoi(vars[0]);
            char* asm_vars[1] = { aapts(vars[0]) };
            int bin_vars[1] = { num }; 

            data_obj* data_o = create_and_init_data_obj();
            data_o->byte_size = 4;
            eo->data = data_o;

            data_o->data = w_malloc(4);
            eo->asm_vars = w_malloc(sizeof(asm_vars));
            eo->bin_vars = w_malloc(sizeof(bin_vars));

            memcpy(data_o->data, bin_vars, sizeof(data_o->data));
            memcpy(eo->asm_vars, asm_vars, sizeof(eo->asm_vars));
            memcpy(eo->bin_vars, bin_vars, sizeof(eo->bin_vars));
            break;

        case VAR_PTR:
        case DATA_PTR: {
            create_data_link(sf);
            // print_bin_link_table(sf);
            char* data_name = vars[0];
            data_obj* data_o = get_data_obj_by_name(data_name, sf);
            eo->data = data_o;
            data_o->references++;

            char* asm_vars[1] = { aapts(data_name) };
            int bin_vars[1] = { data_o->id }; 

            eo->asm_vars = w_malloc(eo->expr_p->asm_var_num*sizeof(asm_vars));
            eo->bin_vars = w_malloc(eo->expr_p->bin_var_num*sizeof(bin_vars));

            memcpy(eo->asm_vars, asm_vars, sizeof(eo->asm_vars));
            memcpy(eo->bin_vars, bin_vars, sizeof(eo->bin_vars));
            break;
        }

        case FUNCTION: {
            asm_read_function_expr(eo, vars, token_pos_ptr, sf);
            break;
        }
        
        default: // default assumes no vars
            break;
    }

    return eo;
}

expression* asm_read_expression(char*** token_pos_ptr, sct_f* sf) {
    expression* exp = create_and_init_expression();
    
    // sf->structure->code_section_word_counter += 2; // prologue
    node** expr_nodes = w_malloc(sizeof(node*));

    add_sct_bin_words_prologue(sf);
    // printf("Added 02 tokens for prologue\n");

    int expr_nodes_num = 0;
    int exprs_to_read = 1;
    while(exprs_to_read > 0) {
        expr_cmp_result res = asm_identify_expr(token_pos_ptr, sf);
        if(res.is_identified) {
            char msg[256];
            sprintf(msg, "Found expr pattern '%s'.", res.match->name);
            // print_success(msg);

            expr_pattern* ep = (expr_pattern*) res.match;
            // int bin_tokens = ep->bin_token_num;
            // sf->structure->code_section_word_counter += bin_tokens;

            expr_obj* eo = asm_create_expr_obj(res.match, (char**)res.vars, token_pos_ptr, sf);
            // print_expr_obj(eo, true);
            node* expr_node = create_node(eo);
            if(expr_nodes_num == 0) {
                *expr_nodes = expr_node;
            } else {
                insert_node(expr_nodes, expr_node);
            }
            expr_nodes_num++;
            exprs_to_read--;
        }
        // char** next_token = *token_pos_ptr+1;
        // size is calculated dynamically looking for binary operators
        if(is_expr_token_binary_op(*token_pos_ptr)) { 
            // printf("found binary op: %s\n", **token_pos_ptr);
            exprs_to_read += 2; 
        }
    }

    // copy expr nodes to expr array
    exp->expr_objs_len = expr_nodes_num;
    exp->expr_objs = w_malloc(expr_nodes_num*sizeof(expr_obj));
    node* cur_expr_node = *expr_nodes;
    int i=0;
    while(cur_expr_node != NULL) {
        expr_obj* eo = cur_expr_node->item;
        memcpy((exp->expr_objs+i), eo, sizeof(expr_obj));
        cur_expr_node = cur_expr_node->next;
        i++;
    }

    return exp;
}

code_obj* asm_read_if_statement(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;
    *token_pos_ptr += 1; //paranthesis
    // read expression
    expression* exp = asm_read_expression(token_pos_ptr, sf);
    node* exp_node = create_node(exp);

    c_obj->expression_node_num = 1;
    c_obj->expression_nodes = w_malloc(sizeof(node*));
    *c_obj->expression_nodes = exp_node;

    // read code block
    *token_pos_ptr += 1; // closing paranthesis
    if(strncmp(**token_pos_ptr, "{", 1) != 0) {
        print_err("Error, no '{' around code block.", -4); 
        print_token_area_details(*token_pos_ptr, MODE_ASM); 
    }

    int code_block_tokens_num = count_tokens_in_code_block(token_pos_ptr);
    // printf("code_block_tokens to read: %d\n", code_block_tokens_num);
    // *token_pos_ptr += 1; // block opening paranthesis
    code_obj* c_block = asm_read_code_block(code_block_tokens_num, token_pos_ptr, sf);
    // printf("after block\n");
    // *token_pos_ptr += 1; // closing paranthesis
    node* code_node = create_node(c_block);
    
    c_obj->code_nodes_num = 1;
    c_obj->code_nodes = w_malloc(sizeof(node*));
    *c_obj->code_nodes = code_node;

    return c_obj;
}

code_obj* asm_read_else_statement(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;

    // read code block
    if(strncmp(**token_pos_ptr, "{", 1) != 0) {
        print_err("Error, no '{' around code block.", -4); 
        print_token_area_details(*token_pos_ptr, MODE_ASM); 
    }

    int code_block_tokens_num = count_tokens_in_code_block(token_pos_ptr);
    // printf("code_block_tokens_num: %d\n", code_block_tokens_num);
    // *token_pos_ptr += 1; // block opening paranthesis
    code_obj* c_block = asm_read_code_block(code_block_tokens_num, token_pos_ptr, sf);
    // *token_pos_ptr += 1; // closing paranthesis
    node* code_node = create_node(c_block);
    
    c_obj->code_nodes_num = 1;
    c_obj->code_nodes = w_malloc(sizeof(node*));
    *c_obj->code_nodes = code_node;

    return c_obj;
}

code_obj* asm_read_switch_case(code_pattern* cp, char** vars, char*** tokens_pos_ptr, sct_f* sf) {
    code_obj* switch_c_obj = create_and_init_c_obj();

    switch_c_obj->cp = cp;
    *tokens_pos_ptr += 1; // opening exp paranthesis

    add_sct_bin_words_switch_body(sf);

    // read expression
    expression* exp = asm_read_expression(tokens_pos_ptr, sf);
    node* exp_node = create_node(exp);

    switch_c_obj->expression_node_num = 1;
    switch_c_obj->expression_nodes = w_malloc(sizeof(node*));
    *switch_c_obj->expression_nodes = exp_node;

    *tokens_pos_ptr += 1; // closing exp paranthesis
    // read cases code block
    if(strncmp(**tokens_pos_ptr, "{", 1) != 0) {
        print_err("Error, no '{' around code block.", -4); 
        print_token_area_details(*tokens_pos_ptr, MODE_ASM); 
    }

    int tokens_to_read = count_tokens_in_code_block(tokens_pos_ptr);
    int cases_num = count_token_from_to(*tokens_pos_ptr, "case", "{", "}");
    // printf("switch_block_tokens to read: %d\n", tokens_to_read);
    // printf("cases to read: %d\n", cases_num);
    *tokens_pos_ptr += 1; // block opening paranthesis
    
    // CODE BLOCK
    code_obj* switch_block = create_and_init_c_obj();
    code_pattern* case_p = init_aid_cp_case();
    char* case_token = case_p->asm_tokens[0];
    int cases_index = 0;
    int cases[cases_num];
    int cases_offsets[cases_num];
    int exp_token_bytes_len = exp->expr_objs[0].expr_p->bin_token_num*4;
    int first_case_offset = 0x0210+0x10+exp_token_bytes_len;
    
    code_pattern* switch_block_cp = code_patterns[3]; //code_block cp
    switch_block->cp = switch_block_cp; 
    
    // sf->structure->code_section_word_counter += switch_block_cp->bin_token_num;

    int start_token_addr = (int) *tokens_pos_ptr;
    int end_token_addr = (start_token_addr + tokens_to_read*sizeof(char*));
    // printf("end at token: %s\n", ((char*)end_token_addr));

    unsigned long before_block_code_word_count = get_sct_code_word_count(sf);
    add_sct_bin_words_prologue(sf);
    int prev_case_pos = sf->structure->code_section_word_counter;
    // printf("Added 02 tokens for prologue\n");

    int block_word_size = 0;
    node** code_nodes = w_malloc(sizeof(node*));
    int code_nodes_num = 0;
    while(*tokens_pos_ptr != NULL && ((int)*tokens_pos_ptr) < end_token_addr) {
        // Check for case
        char** token_ptr = *tokens_pos_ptr;
        if(strlen(*token_ptr) == strlen(case_token) && strcmp(*token_ptr, case_token) == 0) {
            *tokens_pos_ptr += 1; // "case"
            // todo: validate integer
            int case_int = atoi(**tokens_pos_ptr);
            int offset = (first_case_offset + (sf->structure->code_section_word_counter - prev_case_pos)*4);
            cases[cases_index] = case_int;
            cases_offsets[cases_index] = offset;
            cases_index++;
            *tokens_pos_ptr += 1; // value
            // printf("identified case: %d, offset: %08x\n", case_int, offset);
        } else {
            cp_cmp_result res = asm_identify_cp(tokens_pos_ptr, sf);
            if(res.is_identified) {
                char msg[256];
                sprintf(msg, "Found pattern '%s'.\n", res.match->name);
                // print_success(msg);

                code_pattern* cp = (code_pattern*) res.match;
                // int bin_tokens = cp->bin_token_num - cp->bin_extra_token_num;
                // sf->structure->code_section_word_counter += bin_tokens;

                // print_code_pattern(cp);
                obj_and_token_ptr oatp = asm_create_code_obj(cp, (char**) res.vars, tokens_pos_ptr, sf);

                code_obj* co = (code_obj*) oatp.obj;
                // int bin_tokens_num = count_code_obj_bin_tokens(co); // very inefficient way to count every time
                // block_word_size += bin_tokens_num;
                // if(cp->type == SWITCH){
                //     block_word_size += 0x85;
                // }

                node* code_node = create_node(oatp.obj);
                if(code_nodes_num == 0) {
                    *code_nodes = code_node;
                } else {
                    insert_node(code_nodes, code_node);
                }
                code_nodes_num++;
                // print_asm_tokens(*tokens_pos_ptr, 5);
                // printf("\n");
                // print_bin_code_obj(oatp.obj);
                // printf("\n");
            }
        } 
    }
    // printf("bin tokens in code switch_block [%08x]: %d\n", start_token_addr, block_word_size);
    // printf("token_pos_addr: %08x, end_token_addr: %08x\n", *tokens_pos_ptr, end_token_addr);
    switch_block->code_nodes_num = code_nodes_num;
    switch_block->code_nodes = code_nodes;

    unsigned long after_block_code_word_count = get_sct_code_word_count(sf);
    block_word_size = (int) (after_block_code_word_count - before_block_code_word_count);
    // printf("block_word_size: %d (%04x)\n", block_word_size, block_word_size);
    int bin_vars[1] = { block_word_size }; // +2 for the prologue [0xfffffffc size]
    switch_block->bin_vars = w_malloc(switch_block->cp->bin_var_num*sizeof(int));

    memcpy(switch_block->bin_vars, bin_vars, sizeof(switch_block->bin_vars));

    // END OF CODE BLOCK

    // printf("after block\n");
    *tokens_pos_ptr += 1; // closing paranthesis
    node* code_node = create_node(switch_block);

    // copy cases to switch code obj 
    switch_c_obj->bin_var_num = 1+(cases_num*2); // cases_num + cases + cases_offsets
    switch_c_obj->bin_vars = w_malloc(1+(cases_num*2)*sizeof(int));
    memcpy(switch_c_obj->bin_vars, &cases_num, sizeof(int)); // cases_num
    memcpy((switch_c_obj->bin_vars + 1), cases, cases_num*sizeof(int)); // cases
    memcpy((switch_c_obj->bin_vars + 1 + cases_num), cases_offsets, cases_num*sizeof(int)); // offsets

    switch_c_obj->code_nodes_num = 1;
    switch_c_obj->code_nodes = w_malloc(sizeof(node*));
    *switch_c_obj->code_nodes = code_node;

    // print_code_obj(switch_c_obj);
    // print_bin_code_obj(switch_c_obj);
    return switch_c_obj;
}

code_obj* asm_read_function_call(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;
    char* func_name = ((char**)vars)[0];
    game_fun* func = get_game_func_by_name(func_name);
    if(func == NULL) {
        char err[256];
        sprintf(err, "error, game function '%s' not found in known functions.", func_name);
        print_err_and_exit(err, -4);
    }

    // count params(exprs)
    int params_num = 0;
    int commas_num = count_token_from_to(*token_pos_ptr, ",", "(", ")");
    if(commas_num == 0){
        int tokens_in_parenth = count_tokens_from_to(*token_pos_ptr, "(", ")");
        if(tokens_in_parenth > 0) params_num = 1;
    } else {
        params_num = commas_num+1;
    }

    char* asm_vars[1] = { aapts(func_name) };
    int bin_vars[2] = { func->id, params_num };

    c_obj->asm_vars = w_malloc(cp->asm_var_num*sizeof(char*));
    c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));

    memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
    memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

    // printf("params_num: %d\n", params_num);
    if(strlen(**token_pos_ptr) == 1 && strcmp(**token_pos_ptr, "(") == 0)
        *token_pos_ptr += 1; //paranthesis
    // read param expressions
    node** exp_nodes = w_malloc(sizeof(node*));
    int exp_nodes_num = 0;
    for(int i=0; i < params_num; i++){
        if(i > 0) *token_pos_ptr += 1; //comma
        expression* exp = asm_read_expression(token_pos_ptr, sf);
        node* exp_node = create_node(exp);
        if(exp_nodes_num == 0) {
            *exp_nodes = exp_node;
        } else {
            insert_node(exp_nodes, exp_node);
        }
        exp_nodes_num++;
    }

    if(strlen(**token_pos_ptr) == 1 && strcmp(**token_pos_ptr, ")") == 0)
        *token_pos_ptr += 1; // closing paranthesis

    c_obj->expression_node_num = exp_nodes_num;
    c_obj->expression_nodes = exp_nodes;

    return c_obj;
}

expr_obj* asm_read_function_expr(expr_obj* e_obj, char** vars, char*** token_pos_ptr, sct_f* sf) {
    char* func_name = ((char**)vars)[0];
    game_fun* func = get_game_func_by_name(func_name);
    if(func == NULL) {
        char err[256];
        sprintf(err, "error, game function '%s' not found in known functions.", func_name);
        print_err_and_exit(err, -4);
    }

    // count params(exprs)
    int params_num = 0;
    int commas_num = count_token_from_to(*token_pos_ptr, ",", "(", ")");
    if(commas_num == 0){
        int tokens_in_parenth = count_tokens_from_to(*token_pos_ptr, "(", ")");
        if(tokens_in_parenth > 0) params_num = 1;
    } else {
        params_num = commas_num+1;
    }

    char* asm_vars[1] = { aapts(func_name) };
    int bin_vars[2] = { func->id, params_num };

    e_obj->asm_vars = w_malloc(e_obj->expr_p->asm_var_num*sizeof(char*));
    e_obj->bin_vars = w_malloc(e_obj->expr_p->bin_var_num*sizeof(int));

    memcpy(e_obj->asm_vars, asm_vars, sizeof(e_obj->asm_vars));
    memcpy(e_obj->bin_vars, bin_vars, sizeof(e_obj->bin_vars));

    // printf("params_num: %d\n", params_num);
    if(strlen(**token_pos_ptr) == 1 && strcmp(**token_pos_ptr, "(") == 0)
        *token_pos_ptr += 1; //paranthesis
    // read param expressions
    node** exp_nodes = w_malloc(sizeof(node*));
    int exp_nodes_num = 0;
    for(int i=0; i < params_num; i++){
        if(i > 0) *token_pos_ptr += 1; //comma
        expression* exp = asm_read_expression(token_pos_ptr, sf);
        node* exp_node = create_node(exp);
        if(exp_nodes_num == 0) {
            *exp_nodes = exp_node;
        } else {
            insert_node(exp_nodes, exp_node);
        }
        exp_nodes_num++;
    }

    if(strlen(**token_pos_ptr) == 1 && strcmp(**token_pos_ptr, ")") == 0)
        *token_pos_ptr += 1; // closing paranthesis

    e_obj->expression_node_num = exp_nodes_num;
    e_obj->expression_nodes = exp_nodes;

    return e_obj;
}

code_obj* asm_read_script_call(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;
    
    char* script_label = vars[0];
    int script_id = get_script_id_by_label(script_label, sf);
    if(script_id == -1) {
        char err[256];
        sprintf(err, "Error, script '%s' not found.", script_label);
        print_err_and_exit(err, -4);
    }
    // printf("script id: %d\n", script_id);

    char* asm_vars[1] = { aapts(script_label) };
    int bin_vars[1] = { script_id };

    c_obj->asm_vars = w_malloc(c_obj->cp->asm_var_num*sizeof(char*));
    c_obj->bin_vars = w_malloc(c_obj->cp->bin_var_num*sizeof(int));

    memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
    memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

    return c_obj;
}

code_obj* asm_read_room_var_ptr(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;
    char* var_name = vars[0];
    game_var* gv = get_game_var_by_name(var_name);
    if(gv == NULL) {
        char err[256];
        sprintf(err, "Error, game_var '%s' not found.", var_name);
        print_err_and_exit(err, -4);
    }

    char* asm_vars[1] = { aapts(gv->name) };
    int bin_vars[2] = { gv->first_offset, gv->second_offset };

    c_obj->asm_vars = w_malloc(cp->asm_var_num*sizeof(char*));
    c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(int));
    memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
    memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

    return c_obj;
}

code_obj* asm_read_assignment(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;
    c_obj->expression_node_num = 1;
    c_obj->expression_nodes = w_malloc(sizeof(node*));

    expression* exp = asm_read_expression(token_pos_ptr, sf);
    *c_obj->expression_nodes = create_node(exp);

    return c_obj;
}

code_obj* asm_read_var_ptr(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;
    create_data_link(sf);
    // print_bin_link_table(sf);
    char* data_name = vars[0];
    data_obj* data_o = get_data_obj_by_name(data_name, sf);
    c_obj->data = data_o;
    data_o->references++;

    char* asm_vars[1] = { aapts(data_name) };
    int bin_vars[1] = { data_o->id }; 

    c_obj->asm_vars = w_malloc(cp->asm_var_num*sizeof(asm_vars));
    c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(bin_vars));

    memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
    memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

    return c_obj;
}

code_obj* asm_read_default(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;

    return c_obj;
}
    

obj_and_token_ptr asm_create_code_obj(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj;

    switch(cp->type) {
        case IF_STATEMENT:
            c_obj = asm_read_if_statement(cp, vars, token_pos_ptr, sf);
            break;
        case ELSE_STATEMENT:
            c_obj = asm_read_else_statement(cp, vars, token_pos_ptr, sf);
            break;
        case SWITCH:
            c_obj = asm_read_switch_case(cp, vars, token_pos_ptr, sf);
            break;
        case FUNCTION_CALL:
            c_obj = asm_read_function_call(cp, vars, token_pos_ptr, sf);
            break;
        case SCRIPT_CALL:
            c_obj = asm_read_script_call(cp, vars, token_pos_ptr, sf);
            break;
        case ROOM_VAR_PTR:
            c_obj = asm_read_room_var_ptr(cp, vars, token_pos_ptr, sf);
            break;
        case CP_VAR_PTR:
            c_obj = asm_read_var_ptr(cp, vars, token_pos_ptr, sf);
            break;
        case ASSIGNMENT: {
            c_obj = asm_read_assignment(cp, vars, token_pos_ptr, sf);
            break;
        }    
        default:
            c_obj = asm_read_default(cp, vars, token_pos_ptr, sf);
    }

    void* token_ptr = *token_pos_ptr;
    obj_and_token_ptr oatp = { .obj = c_obj, .type = OBJ_CODE, .token_ptr = token_ptr };
    return oatp;
}

int count_expr_bin_tokens(expr_obj* expr) {
    int expr_token_num = 0;
    int exps_token_num = 0;
    expr_token_num += expr->expr_p->bin_token_num;

    if(expr->expression_node_num > 0) {
        node* exp_node = *expr->expression_nodes;
        while(exp_node != NULL) {
            exps_token_num += count_expression_bin_tokens(exp_node->item);
            exp_node = exp_node->next;
        }
    }

    return expr_token_num + exps_token_num;
}

int count_expression_bin_tokens(expression* exp) {
    int exprs_token_num = 0;
    int exprs_num = exp->expr_objs_len;
    for(int i=0; i < exprs_num; i++) {
        expr_obj* expr = (exp->expr_objs+i);
        exprs_token_num += count_expr_bin_tokens(expr);
    }

    return exprs_token_num;
}

int count_code_obj_bin_tokens(code_obj* co) {
    int cp_token_num = 0;
    int exp_token_num = 0;
    int code_token_num = 0;
    
    int cp_bin_tokens_num = co->cp->bin_token_num - co->cp->bin_extra_token_num;
    cp_token_num += cp_bin_tokens_num;

    if(co->expression_node_num > 0) {
        node* exp_node = *co->expression_nodes;
        while(exp_node != NULL) {
            exp_token_num += count_expression_bin_tokens(exp_node->item);
            exp_node = exp_node->next;
        }
    }

    if(co->code_nodes_num > 0) {
        node* code_node = *co->code_nodes;
        while(code_node != NULL) {
            code_token_num += count_code_obj_bin_tokens(code_node->item);
            code_node = code_node->next;
        }
    }

    return cp_token_num + exp_token_num + code_token_num;
}

code_obj* asm_read_code_block(int tokens_to_read, char*** tokens_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();
    
    code_pattern* cp = code_patterns[3]; //code_block cp
    c_obj->cp = cp; 

    
    unsigned long before_block_code_word_count = get_sct_code_word_count(sf);
    add_sct_bin_words_prologue(sf);
    // printf("Added 02 tokens for prologue\n");
    int block_word_size = 0;
    if(tokens_to_read > 0) {
        // sf->structure->code_section_word_counter += cp->bin_token_num;

        if(strlen(**tokens_pos_ptr) == 1 && strncmp(**tokens_pos_ptr, "{", 1) == 0) {
            *tokens_pos_ptr += 1; // block opening paranthesis
        }
        int start_token_addr = (int) *tokens_pos_ptr;
        int end_token_addr = (start_token_addr + tokens_to_read*sizeof(char*));
        node** code_nodes = w_malloc(sizeof(node*));
        int code_nodes_num = 0;
        while(*tokens_pos_ptr != NULL && ((int)*tokens_pos_ptr) < end_token_addr) {
            cp_cmp_result res = asm_identify_cp(tokens_pos_ptr, sf);
            if(res.is_identified) {
                char msg[256];
                sprintf(msg, "Found pattern '%s'.\n", res.match->name);
                // print_success(msg);

                code_pattern* cp = (code_pattern*) res.match;
                // int bin_tokens = cp->bin_token_num - cp->bin_extra_token_num;
                // sf->structure->code_section_word_counter += bin_tokens;

                // print_code_pattern(cp);
                obj_and_token_ptr oatp = asm_create_code_obj(cp, (char**) res.vars, tokens_pos_ptr, sf);

                code_obj* co = (code_obj*) oatp.obj;
                // int bin_tokens_num = count_code_obj_bin_tokens(co); // very inefficient way to count every time
                // block_word_size += bin_tokens_num;
                // if(cp->type == SWITCH){
                //     block_word_size += 0x85;
                // }

                node* code_node = create_node(oatp.obj);
                if(code_nodes_num == 0) {
                    *code_nodes = code_node;
                } else {
                    insert_node(code_nodes, code_node);
                }
                code_nodes_num++;
                // print_asm_tokens(*tokens_pos_ptr, 5);
                // printf("\n");
                // print_bin_code_obj(oatp.obj);
                // printf("\n");
            }
        }

        if(tokens_pos_ptr != NULL && *tokens_pos_ptr != NULL && **tokens_pos_ptr != 0 &&
            strlen(**tokens_pos_ptr) == 1 && strncmp(**tokens_pos_ptr, "}", 1) == 0) {
            *tokens_pos_ptr += 1; // closing paranthesis
        }

        // printf("bin tokens in code block [%08x]: %d\n", start_token_addr, block_word_size);
        // printf("token_pos_addr: %08x, end_token_addr: %08x\n", *tokens_pos_ptr, end_token_addr);
        c_obj->code_nodes_num = code_nodes_num;
        c_obj->code_nodes = code_nodes;
    }

    unsigned long after_block_code_word_count = get_sct_code_word_count(sf);
    block_word_size = (int) (after_block_code_word_count - before_block_code_word_count);
    // printf("block_word_size: %d (%04x)\n", block_word_size, block_word_size);
    int bin_vars[1] = { block_word_size }; // +2 for the prologue [0xfffffffc size]
    c_obj->bin_vars = w_malloc(c_obj->cp->bin_var_num*sizeof(int));

    memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

    return c_obj;
}
