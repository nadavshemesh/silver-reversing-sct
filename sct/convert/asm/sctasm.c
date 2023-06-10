#include "sct\convert\asm\sctasm.h"

bool is_separator_char(char ch) {
    if(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '|' || ch == '&' || ch == '@' || ch == ']' || ch == '-' ||
         ch == '{' || ch == '}' || ch == '[' || ch == '\0' || ch == '(' || ch == ')' || ch == ',' || ch == '+' || ch == '=' || ch == '*' || ch == '%')
        return true;
    return false;
}

bool is_special_char(char ch) {
    if(ch == '(' || ch == ')' || ch == ',' || ch == '{' || ch == '!' || ch == '|' || ch == '@' || ch == '*' || ch == '-' ||
       ch == '[' || ch == ']' || ch == '}' || ch == '+' || ch == '>' || ch == '<' || ch == '=' || ch == '&' || ch == '%')
        return true;
    return false;
}

bool is_keyword(char* str) {
    char* keywords[] = { "var", "gamevar", "call", "int" };
    int len = 4;

    for(int i=0; i < len; i++) {
        char* keyword = keywords[i];
        if(strlen(str) == strlen(keyword) && strcmp(str, keyword) == 0) {
            return true;
        }
    }
    return false;
}

char peek_next_char(FILE* f) {
    unsigned long f_pos = ftell(f);
    char c = getc(f);
    fseek(f, f_pos, SEEK_SET);

    return c;
}

bool is_char_paranth(char c) {
    return (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}');
}

int read_word(FILE* f, char* dest) {
    int c = getc(f);

    // if separator, get next non-separator char.
    if(is_separator_char(c)) {
        while(c != EOF && is_separator_char(c) && !is_special_char(c)) { c = getc(f); }
    }

    // private case "-=" workaround
    if((c == '-' || c == '/') && peek_next_char(f) == '=') {
        dest[0] = c;
        getc(f);
        dest[1] = '=';
        dest[2] = 0;
        return 0;
    }

    // handle string
    if(c == '\"') {
        dest[0] = c;
        c = getc(f);
        int i = 1;
        while(c != EOF && c != '\"' && i < MAX_ASM_TOKEN_LEN) {
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
    
    // ignore multi line comments
    if(c == '/' && peek_next_char(f) == '*') {
        // printf("found multiline comment\n");
        while(!(c == '*' && peek_next_char(f) == '/')) {
            c = getc(f);
        }
        c = getc(f);
        c = getc(f);
        while(c == '\t' || c == '\n' || c == '\r' || c == ' ') {
            c = getc(f);
        }
    }

    if(is_special_char(c)) {
        int index = 0;
        if(is_special_char(peek_next_char(f)) && !is_char_paranth(c) && c != ',') { // such as (ie: ++, --, ==, >=)
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

// int count_next_section_tokens_old(sct_f* sf) {
//     int num = 0;
//     unsigned long f_pos = ftell(sf->file);

//     file_seek_part_of_word("._", sf->file);
//     num = count_text_words_until_string(sf->file, "._");

//     fseek(sf->file, f_pos, SEEK_SET);

//     return num;
// }

int count_tokens(char** tokens) {
    if(tokens == NULL) return 0;
    int i = 0;
    char** token_ptr = tokens;
    char* token = *token_ptr;
    while(token != NULL) {
        i++;
        token_ptr += 1;
        if(token_ptr == NULL) break;
        token = *token_ptr;
    }

    return i;
}

// int count_next_section_tokens(sct_f* sf) {
//     unsigned long f_pos = ftell(sf->file);

//     file_seek_part_of_word("._", sf->file);
//     int tokens_len = count_text_words_until_string(sf->file, "._");
//     char** tokens = tokenize_next_section(sf);
//     tokens_len = count_tokens(tokens);
//     printf("tokens_len: %d\n", tokens_len);

//     fseek(sf->file, f_pos, SEEK_SET);

//     return tokens_len;
// }

// char** join_tokens_until_space_eq_keyword(char** tokens) {
//     char** token_ptr = tokens;
//     int until_space = count_tokens_from_to(token_ptr, *token_ptr, " ");
//     int until_eq = count_tokens_from_to(token_ptr, *token_ptr, "=");
//     int len = (until_eq > until_space)? until_space : until_eq;

//     for(int i=0; i < len; i++) {
//         if(is_keyword(*token_ptr)) {

//         }
//     }
// }

char* merge_tokens(char** tokens, int num_to_join) {
    char** token_ptr = tokens;
    char* prev_token = *token_ptr;
    token_ptr += 1;

    for(int i=1; i < num_to_join; i++) {
        int size = strlen(prev_token) + strlen(*token_ptr)+1;
        char* new_token = calloc(size, size); 
        strncat(new_token, prev_token, strlen(prev_token));
        strncat(new_token, *token_ptr, strlen(*token_ptr));
        new_token[size-1] = 0;
        prev_token = new_token;
        token_ptr += 1;
    }

    return prev_token;
}

char* peek_next_token(char** tokens_ptr) {
    if((tokens_ptr+1) == NULL || *(tokens_ptr+1) == NULL)
        return 0;
    return *(tokens_ptr+1);
}

char* peek_prev_token(char** tokens_ptr) {
    if((tokens_ptr-1) == NULL || *(tokens_ptr-1) == NULL)
        return 0;
    return *(tokens_ptr-1);
}

bool is_string_ascii_digits(char* str) {
    for(int i=0; i < strlen(str); i++) {
        int ch = (int) str[i];
        if(!(ch >= 48 && ch <= 57)) {
            return false;
        }
    }
    return true;
}

char** join_tokens_by_rules(char** tokens, int tokens_len) {
    char** new_tokens = w_malloc((tokens_len+5)*sizeof(char*)); // +5 for padding 
    char** token_ptr = tokens;
    int new_len = tokens_len;

    for(int i=0; i < new_len; i++) {
        if(token_ptr == NULL || *token_ptr == NULL){
            new_len = (i==0)?i:i-1;
            break;
        }
        char* new_token = aapts(*token_ptr);
        if(i > 0 && token_ptr != NULL && strs_identical(peek_prev_token(token_ptr), "gamevar")
             && strs_identical(*token_ptr, "*")) {
            free(new_token);
            new_token = merge_tokens(token_ptr, 4); // * , name, +, number
            token_ptr += 3;
            new_len -= 3;
        } else if(token_ptr != NULL && strs_identical(*token_ptr, "-") && is_string_ascii_digits(peek_next_token(token_ptr))) {
            free(new_token);
            new_token = merge_tokens(token_ptr, 2); // -, number
            token_ptr += 1;
            new_len -= 1;
        }

        new_tokens[i] = new_token;
        // free(*token_ptr);
        token_ptr += 1;
    }

    // print_asm_tokens(new_tokens, new_len);
    // free old tokens
    free(tokens);

    return new_tokens;
}

char** tokenize_section(char* section_title, sct_f* sf) {
    file_seek_word_from_beginning(section_title, sf->file);

    int tokens_len = count_text_words_until_string(sf->file, "._");
    // printf("tokens until next section: %d.\n", tokens_len);
    char** tokens = w_calloc((tokens_len+5)*sizeof(char*)); // +5 for padding 
    for(int i=0; i < tokens_len; i++) {
        char token[MAX_ASM_TOKEN_LEN];
        read_word(sf->file, token);
        tokens[i] = aapts(token);
    }
    // print_asm_tokens(*tokens_ptr, tokens_len);
    // return tokens;
    tokens = join_tokens_by_rules(tokens, tokens_len);
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
        if(strlen(*tokens_ptr) == strlen(from) && strcmp(*tokens_ptr, from) == 0) {
            count = true;
        }
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

int count_parameters(char** tokens_ptr) {
    bool count = false;
    int counter = 0;
    int matches = 0;
    int nested_paranth = 0;
    int nested_quotes = 0;
    bool quotes_open = false;
    // count only from '(' to ')' and exclude counting inside "" and '{' '}'
    while(*tokens_ptr != NULL) {
        if(count && strs_identical(*tokens_ptr, ")")) {
            matches--;
            if(matches == 0)
                return counter;
        }
        if(count && strs_identical(*tokens_ptr, ",")) {
            if(nested_paranth == 0 && nested_quotes == 0 && matches == 1) {
                counter++;
            }
        }
        if(nested_paranth == 0 && nested_quotes == 0 && strs_identical(*tokens_ptr, "(")) {
            count = true;
            matches++;
        }
        if(count && strs_identical(*tokens_ptr, "{")) {
            nested_paranth++;
        }
        if(count && strs_identical(*tokens_ptr, "}")) {
            nested_paranth--;
        }
        if(count && !quotes_open && **tokens_ptr == '\"') {
            nested_quotes++;
            quotes_open = true;
        }
        if(count && quotes_open && **tokens_ptr == '\"') {
            nested_quotes--;
            quotes_open = false;
        }
        tokens_ptr++;
    }
    return -1;
}

int count_token_from_to_no_nesting(char** tokens_ptr, char* token, char* from, char* to) {
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
            if(matches == 1)
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

long gen_id = 0;
data_obj* asm_create_inline_data_obj(char*** tokens_pos_ptr, sct_f* sf) {
    char** token_ptr = *tokens_pos_ptr;
    if(*token_ptr == NULL)
        print_err_and_exit("Error, inline data obj ptr is null.", -4);

    char gen[256];
    sprintf(gen, "gen_%ld", gen_id);
    gen_id++;
    char* name = aapts(gen);

    int ds_size = sf->structure->data_sec_size;
    int id = (ds_size % 4 == 0) ? ds_size/4 : (ds_size/4+1);
    node* data_nodes = *sf->data_nodes;

    char* token = *token_ptr;
    data_obj* data_o = create_and_init_data_obj();
    switch(token[0]) {
        case '{': {
            int len = count_tokens_from_to(token_ptr, "{", "}");
            if(len == -1) {
                print_err("invalid inline int array", -4); 
                print_token_area_details(token_ptr, MODE_ASM);
            }
            // todo: validate structure
            token_ptr++;
            int integers_num = len - (len/2);
            //printf("array len: %d.\n", integers_num);
            int integers[integers_num];
            for(int i=0, j=0; i < len; i+=2, j++) {
                // check for memory reference (data ref)
                char* str = *(token_ptr+i);
                if(is_letter(*str)) {
                    // printf("%c\n", *str);
                    // printf("memory ref: %s\n", str);
                    node* data_node = data_nodes;
                    bool found = false;
                    while(data_node != NULL && !found) {
                        data_obj* data_item = data_node->item;
                        if(strlen(str) == strlen(data_item->name) && strcmp(data_item->name, str) == 0) {
                            found = true;
                            data_ref* dr = create_and_init_data_ref();
                            dr->id = j;
                            dr->data_parent = data_o;
                            dr->data_item = data_item;
                            int num = data_item->id;
                            // int num_id = id+j;
                            integers[j] = num;
                            // printf("offset: %08x\n", id+j);
                            // create_data_data_link(num_id, sf);
                            create_data_data_link_with_dr(dr, sf);
                        }
                        data_node = data_node->next;
                    }
                    if(!found) { 
                        char err[256];
                        sprintf(err, "Error, '%s' var was not found.", str);
                        print_err_and_exit(err, -4); 
                        printf("%s var was not found.\n", str);
                    }
                } else { // not a letter, assuming it is an int, todo: validate
                    int num = atoi(*(token_ptr+i));
                    integers[j] = num;
                }
            }
            token_ptr+=len;

            data_o->id = id;
            data_o->byte_size = integers_num*sizeof(int);
            data_o->name = aapts(name);

            data_o->data = w_malloc(integers_num*sizeof(int));
            memcpy(data_o->data, integers, integers_num*sizeof(int));
            break;
        } 
        case '\"': {
            int len = count_token_chars_from_to(token, '\"', '\"');
            if(len == -1) {
                print_err("Error, invalid string", -4); 
                print_token_area_details(token_ptr, MODE_ASM);
            }
            // printf("found string. len: %d\n", len);
            char str[len+8];
            copy_token_chars_from_to(token, str, '\"', '\"');
            str[len] = 0;
            // printf("string: %s\n", str);

            // char* asm_data = { aapts(token) };
            byte* data = (byte*) aapts(str);
            // printf("string: %s\n", (char*) data);

            data_o->id = id;
            len += 1; // +1 for null byte
            int pre_padding_len = len;
            // printf("len: %d\n", len);
            if(len % 4 != 0) len += 4-(len % 4); // padding
            for(int i=pre_padding_len; i < len; i++) {
                str[i] = 0xAA;
            }
            data = str;
            // printf("len with padding: %d\n", len);
            data_o->byte_size = len; 
            data_o->name = aapts(name);

            // data_o->asm_data = w_malloc(sizeof(char*));
            data_o->data = w_calloc(len);

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

data_obj* asm_create_data_obj(char*** tokens_pos_ptr, int id, node* data_nodes, sct_f* sf) {
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
            int integers_num = (len == 1) ? len : len - (len/2);
            // printf("array len: %d.\n", integers_num);
            int integers[integers_num];
            for(int i=0, j=0; i < len; i+=2, j++) {
                // check for memory reference (data ref)
                char* str = *(token_ptr+i);
                if(is_letter(*str)) {
                    // printf("%c\n", *str);
                    // printf("memory ref: %s\n", str);
                    node* data_node = data_nodes;
                    bool found = false;
                    while(data_node != NULL && !found) {
                        data_obj* data_item = data_node->item;

                        if(strs_identical(str, data_item->name)) {
                            found = true;
                            data_ref* dr = create_and_init_data_ref();
                            dr->id = j;
                            dr->data_parent = data_o;
                            dr->data_item = data_item;
                            int num = data_item->id;
                            integers[j] = num;
                            // printf("offset: %08x\n", id+j);
                            // printf("%s var was found.\n", str);
                            create_data_data_link_with_dr(dr, sf);
                        }
                        data_node = data_node->next;
                    }
                    if(!found) { 
                        // char err[256];
                        // sprintf(err, "Error, %s var was not found.", str);
                        // print_err_and_exit(err, -4); 
                        // printf("%s var was not found.\n", str);
                        data_ref* dr = create_and_init_data_ref();
                        dr->id = j;
                        dr->data_parent = data_o;
                        dr->unfound_name = aapts(str);
                        int num = -9;
                        integers[j] = num;
                        create_data_data_link_with_dr(dr, sf);
                    }
                } else { // not a letter, assuming it is an int, todo: validate
                    int num = atoi(*(token_ptr+i));
                    integers[j] = num;
                }
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
        case '\"': {
            int len = count_token_chars_from_to(token, '\"', '\"');
            if(len == -1) {
                print_err("Error, invalid string", -4); 
                print_token_area_details(token_ptr, MODE_ASM);
            }
            // printf("found string. len: %d\n", len);
            char str[len+8];
            copy_token_chars_from_to(token, str, '\"', '\"');
            str[len] = 0;
            // printf("string: %s\n", str);

            byte* data = (byte*) aapts(str);
            // char* asm_data = { aapts(token) };
            // printf("string: %s\n", (char*) data);

            data_o->id = id;
            len += 1; // +1 for null byte
            int pre_padding_len = len;
            // printf("len: %d\n", len);
            if(len % 4 != 0) len += 4-(len % 4); // padding
            for(int i=pre_padding_len; i < len; i++) {
                str[i] = 0xAA;
            }
            data = str;
            // printf("len with padding: %d\n", len);
            data_o->byte_size = len; 
            data_o->name = aapts(name);

            // data_o->asm_data = w_malloc(sizeof(char*));
            data_o->data = w_calloc(len);

            // memcpy(data_o->asm_data, &asm_data, sizeof(data_o->asm_data));
            memcpy(data_o->data, data, len);
            break;
        }

        default: {
                // check for memory reference (data ref)
                bool is_unfound_data_ref = false;
                char* str = *(token_ptr);
                if(is_letter(*str)) {
                    // printf("%c\n", *str);
                    // printf("memory ref: %s\n", str);
                    node* data_node = data_nodes;
                    bool found = false;
                    while(data_node != NULL && !found) {
                        data_obj* data_item = data_node->item;
                        if(strlen(str) == strlen(data_item->name) && strcmp(data_item->name, str) == 0) {
                            found = true;
                            data_ref* dr = create_and_init_data_ref();
                            dr->id = id;
                            dr->data_parent = data_o;
                            dr->data_item = data_item;
                            int num = data_o->id;
                            int num_id = id;
                            // printf("offset: %08x\n", id+j);
                            // printf("%s var was found.\n", str);
                            create_data_data_link_with_dr(dr, sf);
                        }
                        data_node = data_node->next;
                    }
                    if(!found) { 
                        // char err[256];
                        // sprintf(err, "Error, %s var was not found.", str);
                        // print_err_and_exit(err, -4); 
                        // printf("%s var was not found.\n", str);
                        data_ref* dr = create_and_init_data_ref();
                        dr->id = id;
                        dr->data_parent = data_o;
                        dr->unfound_name = aapts(str);
                        is_unfound_data_ref = true;
                        create_data_data_link_with_dr(dr, sf);
                    }
                }
            // todo: validate structure
            char* asm_data[1] = { aapts(token) };
            int data[1];
            if(is_unfound_data_ref) {
                data[0] = -9;
            } else {
                data[0] = atoi(token);
            }

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

    return data_o;
}

void flush_data_nodes_to_data_section(sct_f* sf) {
    node* data_next = NULL;

    sf->data_section = w_malloc(sf->data_objs_num*sizeof(data_obj*));
    data_next = *sf->data_nodes; // point to start again
    int j = 0;
    while(data_next != NULL) {
        sf->data_section[j] = data_next->item;
        data_next = data_next->next;
        j++;
    }
}

int get_original_var_name_num_if_exists(char *var_name) {
    char prefix[5] = "VAR_";
    if(strlen(var_name) > 4 && strncmp(prefix, var_name, 4) == 0) {
        int var_num_len = strlen(var_name)-4;
        char var_num_str[var_num_len];
        strncpy(var_num_str, var_name+4, var_num_len); 
        int var_num = atoi(var_num_str);

        return var_num;
    }
    return -1; // format is not VAR_
}

void increment_data_nodes_ids(node* start) {
    node* prev = start;
    if(prev != NULL && prev->next != NULL) {
        node* next = prev->next;
        while(next != NULL) {
            int prev_id = ((data_obj*)prev->item)->id;
            int prev_size = ((data_obj*)prev->item)->byte_size;
            ((data_obj*)next->item)->id = prev_id + prev_size/4;

            prev = next;
            next = next->next;
        }
    }
}

void copy_data_nodes_with_new_item(sct_f* sf, data_obj* data, int var_num) {
    node* next_new = NULL;
    node* next = *sf->data_nodes;
    int id = ((data_obj*)next->item)->id;
    bool found = false;
    node* new;
    
    while(next != NULL) {
        node* new_node = create_node(next->item);
        ((data_obj*)next->item)->id = id;

        char* cur_name = ((data_obj*)next->item)->name;
        int cur_var_num = get_original_var_name_num_if_exists(cur_name);
        int size = ((data_obj*)next->item)->byte_size;
        id += (size%4==0)?size/4:size/4+1;

        if(next_new == NULL) {
            next_new = new_node;
            new = new_node;
        } else {
            if(!found && cur_var_num > var_num) {
                found = true;
                data->id = id;
                node* new_node = create_node(data);
                next_new->next = new_node;
                next_new = new_node;
                id++;
            }
            next_new->next = new_node;
            next_new = new_node;
        }
        next = next->next;
    }
    if(!found) {
        data->id = id;
        node* new_node = create_node(data);
        next_new->next = new_node;
    }

    *sf->data_nodes = new;
}

void update_data_id_for_referees(data_obj* data_o) {
    if(data_o->c_referees != NULL) {
        node* cnode = *data_o->c_referees;
        while(cnode != NULL) {
            code_obj* cref = (code_obj*)cnode->item;
            cref->bin_vars[0] = data_o->id;
            cnode = cnode->next;
        }
    }
    if(data_o->e_referees != NULL) {
        node* enode = *data_o->e_referees;
        while(enode != NULL) {
            expr_obj* eref = (expr_obj*)enode->item;
            eref->bin_vars[0] = data_o->id;
            enode = enode->next;
        }
    }
    if(data_o->d_references != NULL) {
        node* dnode = *(data_o->d_references);
        while(dnode != NULL) {
            data_ref* dref = (data_ref*)dnode->item;
            // printf("%d\n", dref->id);
            // print_data_obj(dref->data_item);
            int* offset = (int*)data_o->data;
            *(offset+(dref->id)) = dref->data_item->id;
            dnode = dnode->next;
        }
    }
}

void sync_data_refs_ids(sct_f* sf) {
    node* next = *sf->data_nodes;
    while(next != NULL) {
        data_obj* data_o = (data_obj*)next->item;
        update_data_id_for_referees(data_o);
        next = next->next;
    }
}

void add_new_var_to_data_nodes_by_num(sct_f* sf, data_obj* data, int var_num) {
    node* prev = NULL;
    node* next = *sf->data_nodes;
    node* new_node = create_node(data);
    int id = 0;
    bool found = false;
    
    while(next != NULL) {
        ((data_obj*)next->item)->id = id;

        char* cur_name = ((data_obj*)next->item)->name;
        int cur_var_num = get_original_var_name_num_if_exists(cur_name);
        int size = ((data_obj*)next->item)->byte_size;
        id += size/4;

        if(!found && cur_var_num > var_num) {
            found = true;
            data->id = id;
            if(prev == NULL) {
                *sf->data_nodes = new_node;
            } else {
                prev->next = new_node;
            }
            new_node->next = next;
        }

        prev = (prev == NULL)? next:prev->next;
        next = next->next;
    }
    if(!found) {
        data->id = id;
        prev->next = new_node;
    }
}

data_obj* add_and_init_data_var_by_name(char* var_name, sct_f* sf) {
    data_obj* data_o = create_and_init_data_obj();
    
    data_o->name = aapts(var_name);
    data_o->byte_size = 4;
    data_o->data = (byte*) w_malloc(sizeof(int));
    *((int*)data_o->data) = 0;

    int ds_size = sf->structure->data_sec_size;
    int id = (ds_size % 4 == 0) ? ds_size/4 : (ds_size/4+1);
    data_o->id = id;

    sf->data_objs_num++;
    sf->structure->data_sec_size += 4;

    node* data_node = *sf->data_nodes;
    node* new_node = create_node(data_o);

    // check if it has a VAR_ format to try and not break the order
    int var_num = get_original_var_name_num_if_exists(var_name);

    if(var_num != -1) { // the var has VAR_ format, check where it should fit in order
        copy_data_nodes_with_new_item(sf, data_o, var_num);
    } else { // the var doesnt have VAR_ format, go to the end of the list
        while(data_node->next != NULL) {
            data_node = data_node->next;
        }

        data_node->next = new_node;
    }

    sync_data_refs_ids(sf);
    flush_data_nodes_to_data_section(sf);
        // while(data_node->next != NULL) {
        //     data_node = data_node->next;
        // }

        // data_node->next = new_node;


    return data_o;
}

data_obj* add_inline_var_declaration_to_data_section(sct_f* sf, char*** token_pos_ptr) {
    data_obj* data_o = asm_create_inline_data_obj(token_pos_ptr, sf);

    sf->data_objs_num++;
    sf->structure->data_sec_size += data_o->byte_size;

    node* data_node = *sf->data_nodes;
    while(data_node->next != NULL) { 
        data_node = data_node->next;
    }

    node* new_node = create_node(data_o);
    data_node->next = new_node;

    flush_data_nodes_to_data_section(sf);

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
    node* data_nodes = NULL;
    while(**tokens_pos_ptr != NULL) {
        int ds_size = sf->structure->data_sec_size;
        int id = (ds_size % 4 == 0) ? ds_size/4 : (ds_size/4+1);
        // printf("id: %08x\n", id);
        data_obj* data_o = asm_create_data_obj(tokens_pos_ptr, id, data_nodes, sf);
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
    sf->data_nodes = w_malloc(sizeof(node*));
    *sf->data_nodes = data_nodes;

    flush_data_nodes_to_data_section(sf);

    // attempt to find prev unfound var names
    node* links = *sf->data_link_table;
    while(sf->structure->link_table_size > 0 && links != NULL) {
        data_ref* dr = (data_ref*) links->item;
        if(dr->type == DATA_DATA && dr->data_item == NULL) {
            data_obj* ref_obj = get_data_obj_by_name(dr->unfound_name, false, sf);
            if(ref_obj == NULL) {
                char err[256];
                sprintf(err, "%s is not found. (unfound both times).", dr->unfound_name);
                print_err_and_exit(err, -4);
            }
            dr->data_item = ref_obj;
            int* offset = (int*)dr->data_parent->data;
            *(offset+(dr->id)) = ref_obj->id;
        } 
        links = links->next;
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

expr_cmp_result asm_identify_expr(char*** token_pos_ptr, bool silent_err, sct_f* sf) {
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
    if(silent_err)
        return res;
    print_err("Error, expression not recognized.", 2);
    // print_asm_tokens(token, 5);
    print_token_area_details(token, MODE_ASM);
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

bool is_expr_token_multi_op(char** token) {
    if(token == NULL || *token == NULL) return false;
    // printf("checking binary op: %s\n", *token);
    for(int i=0; i < EXPR_PATTERNS_NUM; i++) {
        expr_pattern* pattern = expr_patterns[i];
        if(pattern->type == MUL_EXP_OP && 
            strlen(*pattern->asm_tokens) == strlen(*token) && 
            strcmp(*pattern->asm_tokens, *token) == 0) {
                return true;
            }
    }
    return false;
}

void create_data_code_link(sct_f* sf) {
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

void create_data_code_link_with_dr(sct_f* sf) {
    node** link_nodes = sf->data_link_table;
    if(link_nodes == NULL) {  print_err_and_exit("Error, data link table undefined.", -4); }

    int* offset = w_malloc(sizeof(int));
    *offset = sf->structure->code_section_word_counter-1;
    *offset |= 0x40000000;

    data_ref* dr = create_and_init_data_ref();
    dr->offset = offset;
    dr->type = DATA_CODE;

    node* link_node = *link_nodes;
    node* new_link_node = create_node(dr); 
    if(sf->structure->link_table_size == 0 || link_node == NULL) {
        *sf->data_link_table = new_link_node;
    } else {
        insert_node(link_nodes, new_link_node);
    }
    sf->structure->link_table_size++;
}

void create_data_data_link(int data_offset, sct_f* sf) {
    node** link_nodes = sf->data_link_table;
    if(link_nodes == NULL) {  print_err_and_exit("Error, data link table undefined.", -4); }

    int* offset = w_malloc(sizeof(int));
    *offset = data_offset;
    *offset |= 0xC0000000;

    node* link_node = *link_nodes;
    node* new_link_node = create_node(offset); 
    if(sf->structure->link_table_size == 0 || link_node == NULL) {
        *sf->data_link_table = new_link_node;
    } else {
        insert_node(link_nodes, new_link_node);
    }
    sf->structure->link_table_size++;
}

void create_data_data_link_with_dr(data_ref* dr, sct_f* sf) {
    node** link_nodes = sf->data_link_table;
    if(link_nodes == NULL) {  print_err_and_exit("Error, data link table undefined.", -4); }

    dr->type = DATA_DATA;
    node* link_node = *link_nodes;
    node* new_link_node = create_node(dr); 
    if(sf->structure->link_table_size == 0 || link_node == NULL) {
        *sf->data_link_table = new_link_node;
    } else {
        insert_node(link_nodes, new_link_node);
    }
    sf->structure->link_table_size++;

    node* dref_node = create_node(dr);

    dr->data_parent->d_references = insert_node_create_head_if_needed(dr->data_parent->d_references, dref_node);
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

        case DATA_INDEX_PTR:
        case VAR_PTR:
        case ADDROF_VAR_PTR: {
            create_data_code_link_with_dr(sf);
            // print_bin_link_table(sf);
            char* data_name = vars[0];
            bool is_negated = false;
            if(strlen(data_name) == 1 && strcmp(data_name, "!") == 0) {
                is_negated = true;
                data_name = **token_pos_ptr;
                *token_pos_ptr += 1;
            }
            // auto create data_obj* data_e only for var ptr
            data_obj* data_o;
            if(expr_p->type == VAR_PTR || expr_p->type == ADDROF_VAR_PTR) {
                data_o = get_data_obj_by_name(data_name, true, sf);
                // auto create var if it isnt declared in the data section
                if(data_o == NULL) {
                    data_o = add_and_init_data_var_by_name(data_name, sf);
                }
            } else {
                data_o = get_data_obj_by_name(data_name, false, sf);
            }
            eo->data = data_o;
            data_o->references++;

            node* ref = create_node(eo);
            if(data_o->e_referees == NULL) {
                data_o->e_referees = w_malloc(sizeof(node*));
                *data_o->e_referees = ref;
            } else {
                node* next = *data_o->e_referees;
                while(next->next != NULL) { next = next->next; }
                next->next = ref;
            }

            if(is_negated) {
                expr_pattern* neg_var_p = init_expr_neg_var_ptr();
                eo->expr_p = neg_var_p;
            }

            char* asm_vars[1] = { aapts(data_name) };
            int bin_vars[1] = { data_o->id }; 

            eo->asm_vars = w_malloc(eo->expr_p->asm_var_num*sizeof(asm_vars));
            eo->bin_vars = w_malloc(eo->expr_p->bin_var_num*sizeof(bin_vars));

            memcpy(eo->asm_vars, asm_vars, sizeof(eo->asm_vars));
            memcpy(eo->bin_vars, bin_vars, sizeof(eo->bin_vars));

            if(expr_p->type == DATA_INDEX_PTR) {
                char* token = **token_pos_ptr;
                if(strlen(token) == 1 && strcmp(token, "[") == 0) {
                    *token_pos_ptr += 1;
                } else { print_err("Error, missing '[' at data index pointer.", -4);
                            print_token_area_details(*token_pos_ptr, MODE_ASM); }

                expression* exp = asm_read_expression(token_pos_ptr, false, sf);
                eo->expression_node_num = 1;
                eo->expression_nodes = w_malloc(sizeof(node*));
                *eo->expression_nodes = create_node(exp);

                token = **token_pos_ptr;
                if(strlen(token) == 1 && strcmp(token, "]") == 0) {
                    *token_pos_ptr += 1;
                } else { print_err("Error, missing ']' at data index pointer.", -4); 
                        print_token_area_details(*token_pos_ptr, MODE_ASM); }
            }
            break;
        }

        case GAME_VAR:{
            char* name = vars[0];
            bool is_negated = false;
            if(strlen(name) == 1 && strcmp(name, "!") == 0) {
                is_negated = true;
                name = **token_pos_ptr;
                *token_pos_ptr += 1;
            }
            game_var* gv = get_game_var_by_name(name);
            if(gv == NULL) { 
                print_err("Error, gamevar not found.", -2);
                print_token_area_details(*token_pos_ptr, MODE_ASM);
            }
            
            int first_off = gv->first_offset;
            if(is_negated) {
                first_off = ((gv->first_offset)|0x20000000);
            }
            int bin_vars[3] = { first_off, gv->second_offset, gv->third_offset };
            char* asm_vars[1] = { aapts(name) };
            eo->bin_vars = w_malloc(expr_p->bin_var_num*sizeof(int));
            eo->asm_vars = w_malloc(expr_p->asm_var_num*sizeof(char*));
            memcpy(eo->bin_vars, bin_vars, expr_p->bin_var_num*sizeof(int));
            memcpy(eo->asm_vars, asm_vars, expr_p->asm_var_num*sizeof(char*));
            break; 
        }

        case FUNC_PTR: {
            char* name = vars[0];
            game_fun* gf = get_game_func_by_name(name);
            if(gf == NULL) {
                char err[256];
                sprintf(err, "game function %s is not found.", name);
                print_err_and_exit(err, -4);
            }
            int bin_vars[1] = { gf->id };
            char* asm_vars[1] = { aapts(name) };
            eo->bin_vars = w_malloc(expr_p->bin_var_num*sizeof(int));
            eo->asm_vars = w_malloc(expr_p->asm_var_num*sizeof(char*));
            memcpy(eo->bin_vars, bin_vars, expr_p->bin_var_num*sizeof(int));
            memcpy(eo->asm_vars, asm_vars, expr_p->asm_var_num*sizeof(char*));
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

expression* asm_read_expression(char*** token_pos_ptr, bool allow_var_decl, sct_f* sf) {
    expression* exp = create_and_init_expression();
    
    // sf->structure->code_section_word_counter += 2; // prologue
    node** expr_nodes = w_malloc(sizeof(node*));

    add_sct_bin_words_prologue(sf);
    // printf("Added 02 tokens for prologue\n");

    int expr_nodes_num = 0;
    int exprs_to_read = 1;
    while(exprs_to_read > 0) {
        expr_cmp_result res = asm_identify_expr(token_pos_ptr, allow_var_decl, sf);
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
        } else {
            // printf("checking inline var declaration\n");

            // TODO: validate!!!

            // check for inline var declaration
            char first = ***token_pos_ptr;
            expr_pattern* ep;
            expr_obj* eo = create_and_init_expr_obj();

            if(first == '\"' || first == '{') {
                ep = expr_patterns[1]; // addr of var
            } else {
                ep = expr_patterns[2]; // var
            }

            eo->expr_p = ep;
            add_sct_bin_words(ep->bin_token_num, sf);

            data_obj* data_o = add_inline_var_declaration_to_data_section(sf, token_pos_ptr);

            create_data_code_link_with_dr(sf);
            eo->data = data_o;
            data_o->references++;
            
            node* ref = create_node(eo);
            if(data_o->e_referees == NULL) {
                data_o->e_referees = w_malloc(sizeof(node*));
                *data_o->e_referees = ref;
            } else {
                node* next = *data_o->e_referees;
                while(next->next != NULL) { next = next->next; }
                next->next = ref;
            }

            char* asm_vars[1] = { aapts(data_o->name) };
            int bin_vars[1] = { data_o->id }; 

            eo->asm_vars = w_malloc(eo->expr_p->asm_var_num*sizeof(asm_vars));
            eo->bin_vars = w_malloc(eo->expr_p->bin_var_num*sizeof(bin_vars));

            memcpy(eo->asm_vars, asm_vars, sizeof(eo->asm_vars));
            memcpy(eo->bin_vars, bin_vars, sizeof(eo->bin_vars));

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
        if(is_expr_token_multi_op(*token_pos_ptr)) { 
            exprs_to_read++; 
        }
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

    // read expressions
    c_obj->expression_nodes = w_malloc(sizeof(node*));
    c_obj->expression_node_num = 0;

    int expressions_to_read = 1;
    while(expressions_to_read > 0) {
        expression* exp = asm_read_expression(token_pos_ptr, false, sf);
        node* exp_node = create_node(exp);
        expressions_to_read--;

        if(get_last_expr_obj(exp)->expr_p->type == MUL_EXP_OP) {
            expressions_to_read++;
        }

        if(c_obj->expression_node_num == 0) {
            *c_obj->expression_nodes = exp_node;
        } else {
            insert_node(c_obj->expression_nodes, exp_node);
        }
        c_obj->expression_node_num++;
    }

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
    expression* exp = asm_read_expression(tokens_pos_ptr, false, sf);
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
    int default_num = count_token_from_to(*tokens_pos_ptr, "default", "{", "}");
    if(default_num > 1) { print_err_and_exit("Error, more than one default cases.", -4); }
    // printf("switch_block_tokens to read: %d\n", tokens_to_read);
    // printf("cases to read: %d\n", cases_num);
    *tokens_pos_ptr += 1; // block opening paranthesis
    
    // CODE BLOCK
    code_obj* switch_block = create_and_init_c_obj();
    code_pattern* case_p = init_aid_cp_case();
    code_pattern* default_case_p = init_aid_cp_default_case();
    char* case_token = case_p->asm_tokens[0];
    char* default_case_token = default_case_p->asm_tokens[0];
    int default_addr = -1;
    int cases_index = 0;
    int cases[cases_num];
    int cases_offsets[cases_num];
    int exp_token_bytes_len = exp->expr_objs[0].expr_p->bin_token_num*4;
    int first_case_offset = 0x0210+0x10+exp_token_bytes_len;
    
    code_pattern* switch_block_cp = code_patterns[3]; //code_block cp
    switch_block->cp = switch_block_cp; 
    
    // sf->structure->code_section_word_counter += switch_block_cp->bin_token_num;

    intptr_t start_token_addr = (intptr_t) *tokens_pos_ptr;
    intptr_t end_token_addr = (start_token_addr + tokens_to_read*sizeof(char*));
    // printf("end at token: %s\n", ((char*)end_token_addr));

    unsigned long before_block_code_word_count = get_sct_code_word_count(sf);
    add_sct_bin_words_prologue(sf);
    int prev_case_pos = sf->structure->code_section_word_counter;
    // printf("Added 02 tokens for prologue\n");

    int block_word_size = 0;
    node** code_nodes = w_malloc(sizeof(node*));
    int code_nodes_num = 0;
    while(*tokens_pos_ptr != NULL && ((intptr_t)*tokens_pos_ptr) < end_token_addr) {
        // Check for case
        char** token_ptr = *tokens_pos_ptr;
        if(strlen(*token_ptr) == strlen(default_case_token) && strcmp(*token_ptr, default_case_token) == 0) {
            *tokens_pos_ptr += 1; // default
            int offset = (first_case_offset + (sf->structure->code_section_word_counter - prev_case_pos)*4);
            default_addr = offset;
        } else if((strlen(*token_ptr) == strlen(case_token) && strcmp(*token_ptr, case_token) == 0)) {
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
    // printf("cases num: %d\n", cases_num);
    switch_c_obj->bin_var_num = 1+1+(cases_num*2); // cases_num + default_case + cases + cases_offsets
    switch_c_obj->bin_vars = w_malloc(1+1+(cases_num*2)*sizeof(int));
    memcpy(switch_c_obj->bin_vars, &cases_num, sizeof(int)); // cases_num
    memcpy(switch_c_obj->bin_vars+1, &default_addr, sizeof(int)); // cases_num
    memcpy((switch_c_obj->bin_vars + 2), cases, cases_num*sizeof(int)); // cases
    memcpy((switch_c_obj->bin_vars + 2 + cases_num), cases_offsets, cases_num*sizeof(int)); // offsets

    // for(int i=1; i < cases_num+1; i++) { 
    //     printf("case: %d\n", switch_c_obj->bin_vars[i]);
    // }
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
    int commas_num = count_parameters(*token_pos_ptr);
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
        expression* exp = asm_read_expression(token_pos_ptr, true, sf);
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
    //int commas_num = count_token_from_to_no_nesting(*token_pos_ptr, ",", "(", ")");
    int commas_num = count_parameters(*token_pos_ptr);
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
        expression* exp = asm_read_expression(token_pos_ptr, true, sf);
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
    char* name = vars[0];
    game_var* gv = get_game_var_by_name(name);
    if(gv == NULL) {
        char err[256];
        sprintf(err, "Error, game_var '%s' not found.", name);
        print_err_and_exit(err, -4);
    }
    int first_off = (*name == '~') ? ((gv->first_offset)|0x20000000) : gv->first_offset;
    int bin_vars[3] = { first_off, gv->second_offset, gv->third_offset };
    char* asm_vars[1] = { aapts(gv->name) };
    c_obj->bin_vars = w_malloc(c_obj->cp->bin_var_num*sizeof(int));
    c_obj->asm_vars = w_malloc(c_obj->cp->asm_var_num*sizeof(char*));
    memcpy(c_obj->bin_vars, bin_vars, c_obj->cp->bin_var_num*sizeof(int));
    memcpy(c_obj->asm_vars, asm_vars, c_obj->cp->asm_var_num*sizeof(char*));

    return c_obj;
}

code_obj* asm_read_assignment(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;
    c_obj->expression_node_num = 1;
    c_obj->expression_nodes = w_malloc(sizeof(node*));

    expression* exp = asm_read_expression(token_pos_ptr, true, sf);
    *c_obj->expression_nodes = create_node(exp);

    return c_obj;
}

code_obj* asm_read_var_ptr(code_pattern* cp, char** vars, char*** token_pos_ptr, sct_f* sf) {
    code_obj* c_obj = create_and_init_c_obj();

    c_obj->cp = cp;
    create_data_code_link_with_dr(sf);
    // print_bin_link_table(sf);
    char* data_name = vars[0];
    data_obj* data_o = get_data_obj_by_name(data_name, true, sf);

    // auto create var if it isnt declared in the data section
    if(data_o == NULL) {
        data_o = add_and_init_data_var_by_name(data_name, sf);
    }
    c_obj->data = data_o;
    data_o->references++;

    node* ref = create_node(c_obj);
    if(data_o->c_referees == NULL) {
        data_o->c_referees = w_malloc(sizeof(node*));
        *data_o->c_referees = ref;
    } else {
        node* next = *data_o->c_referees;
        while(next->next != NULL) { next = next->next; }
        next->next = ref;
    }

    char* asm_vars[1] = { aapts(data_name) };
    int bin_vars[1] = { data_o->id }; 

    c_obj->asm_vars = w_malloc(cp->asm_var_num*sizeof(asm_vars));
    c_obj->bin_vars = w_malloc(cp->bin_var_num*sizeof(bin_vars));

    memcpy(c_obj->asm_vars, asm_vars, sizeof(c_obj->asm_vars));
    memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

    if(cp->type == CP_DATA_INDEX_PTR) {
        char* token = **token_pos_ptr;
        if(strlen(token) == 1 && strcmp(token, "[") == 0) {
            *token_pos_ptr += 1;
        } else { print_err("Error, missing '[' at data index pointer.", -4);
                    print_token_area_details(*token_pos_ptr, MODE_ASM); }

        expression* exp = asm_read_expression(token_pos_ptr, false, sf);
        c_obj->expression_node_num = 1;
        c_obj->expression_nodes = w_malloc(sizeof(node*));
        *c_obj->expression_nodes = create_node(exp);

        token = **token_pos_ptr;
        if(strlen(token) == 1 && strcmp(token, "]") == 0) {
            *token_pos_ptr += 1;
        } else { print_err("Error, missing ']' at data index pointer.", -4); 
                print_token_area_details(*token_pos_ptr, MODE_ASM); }
    }

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
        case CP_DATA_INDEX_PTR:
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
    if(tokens_pos_ptr != NULL && *tokens_pos_ptr != NULL && **tokens_pos_ptr != 0 
        &&strlen(**tokens_pos_ptr) == 1 && strncmp(**tokens_pos_ptr, "{", 1) == 0) {
        *tokens_pos_ptr += 1; // block opening paranthesis
    }

    if(tokens_to_read > 0) {
        // sf->structure->code_section_word_counter += cp->bin_token_num;

        intptr_t start_token_addr = (intptr_t) *tokens_pos_ptr;
        intptr_t end_token_addr = (start_token_addr + tokens_to_read*sizeof(char*));
        node** code_nodes = w_malloc(sizeof(node*));
        int code_nodes_num = 0;
        while(*tokens_pos_ptr != NULL && ((intptr_t)*tokens_pos_ptr) < end_token_addr) {
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

        // printf("bin tokens in code block [%08x]: %d\n", start_token_addr, block_word_size);
        // printf("token_pos_addr: %08x, end_token_addr: %08x\n", *tokens_pos_ptr, end_token_addr);
        c_obj->code_nodes_num = code_nodes_num;
        c_obj->code_nodes = code_nodes;
    }

    if(tokens_pos_ptr != NULL && *tokens_pos_ptr != NULL && **tokens_pos_ptr != 0 &&
        strlen(**tokens_pos_ptr) == 1 && strncmp(**tokens_pos_ptr, "}", 1) == 0) {
        *tokens_pos_ptr += 1; // closing paranthesis
    }

    unsigned long after_block_code_word_count = get_sct_code_word_count(sf);
    block_word_size = (int) (after_block_code_word_count - before_block_code_word_count);
    // printf("block_word_size: %d (%04x)\n", block_word_size, block_word_size);
    int bin_vars[1] = { block_word_size }; // +2 for the prologue [0xfffffffc size]
    c_obj->bin_vars = w_malloc(c_obj->cp->bin_var_num*sizeof(int));

    memcpy(c_obj->bin_vars, bin_vars, sizeof(c_obj->bin_vars));

    return c_obj;
}
