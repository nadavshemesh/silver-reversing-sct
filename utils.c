#include "utils.h"

 void *w_malloc(size_t size) {
    void* res = malloc(size);
    if(res == NULL) {
        printf("malloc error.\n");
        exit(-1);
    }
    return res;
}

 void *w_calloc(size_t size) {
    void* res = calloc(size, 1);
    if(res == NULL) {
        printf("calloc error.\n");
        exit(-1);
    }
    return res;
}

char *aapts(char* str) {
    char* alloc_str = w_malloc(strlen(str)+1);
    strncpy(alloc_str, str, strlen(str));
    *(alloc_str+strlen(str)) = 0;

    return alloc_str;
}

bool strs_identical(char* str1, char* str2) {
    if(strlen(str1) == strlen(str2) && strcmp(str1, str2) == 0)
        return true;
    return false;
}

char* c_itoa(int num, char *str) {
    if(str == NULL) {
        return NULL;
    }
    sprintf(str, "%d", num);
    return str;
}

void print_title(char* msg) {
    printf(ANSI_COLOR_MAGENTA "\n\t %s" ANSI_COLOR_RESET "\n", msg);
}

void print_info(char* msg) {
    printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET "\n", msg);
}

void print_success(char* msg) {
    printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n", msg);
}

void print_warn(char* warning, int warning_code) {
    printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET "\n", warning);
}

void print_err(char* err, int err_code) {
    printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", err);
}

void print_err_and_exit(char* err, int err_code) {
    printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", err);
    exit(err_code);
}

node* init_node(void* item) {
    node* node = w_malloc(sizeof(node));
    node->item = item;
    node->next = NULL;

    return node;
}

node* create_node(void* item) {
    node* n = w_malloc(sizeof(node));
    n->item = item;
    n->next = NULL;

    return n;
}

int count_node_list(node** head) {
    if(head == NULL) return 0;
    node* cur_node = *head;
    int i = 0;
    while(cur_node != NULL) { i++; }
    return i;
}

node* get_node(node** head, int pos) {
    if(pos == 0) return *head;
    node* cur_node = *head;
    int i = 1;
    while(cur_node->next != NULL && i != pos) {
        cur_node = cur_node->next;
        i++;
    }

    return cur_node;
}

void insert_node(node** head, node* new) {
    if(head == NULL) return;
    node* cur_node = *head;
    while(cur_node->next != NULL) {
        cur_node = cur_node->next;
    }
    cur_node->next = new;
}

void remove_node(node** head, int pos) {
    if(head == NULL) return;
    if(pos == 0) *head = (*head)->next;
    
    int i = 1;
    node* prev = *head;
    node* cur_node = (*head)->next;
    while(cur_node != NULL && i != pos) {
        prev = cur_node;
        cur_node = cur_node->next;
    }

    if(i == pos) {
        if(cur_node->next == NULL) {
            prev->next = NULL;
            free(cur_node);
            return;
        }
        prev->next = cur_node->next;
        free(cur_node);
    }
}

void link_node(node** head, node* link_node, int pos) {
    if(head == NULL) return;
    if(pos == 0) {
        link_node->next = *head;
        *head = link_node;
        return;
    }
    int i = 1;
    node* cur_node = *head;
    while(cur_node != NULL && i != pos) {
        cur_node = cur_node->next;
        i++;
    }
    if(i == pos)
        cur_node = link_node;
}

void free_node_to_end(node* n) {
    if(n->next == NULL) {
        free(n);
        return;
    }
    free_node_to_end(n->next);
    free(n);
}

void free_list(node** head) {
    free_node_to_end(*head);
}
node** init_node_list() {
    node** head = w_malloc(sizeof(node*));
    *head = w_malloc(sizeof(node));

    return head;
}

bool is_exception_char(char ch) { 
    if(ch == 0x00 || ch == 0x40)
        return true;
    return false;
}

bool is_printable_ascii(char ch) { 
    return ((ch & ~0x7f) == 0 ) && (isprint(ch) || isspace(ch));
    // return (ch >= 32 && ch <= 126);
}

bool is_letter_or_number(char ch) {
    return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <=122) || (ch >= 48 && ch <= 57);
}

bool is_letter(char ch) {
    return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <=122);
}

bool is_string(byte* str, int len) {
    int null_counter = 0;
    int row_nulls = 0;
    int non_printable = 0;
    int letters = 0;
    int printable = 0;
    int spaces = 0;
    char last = *(str+(len));
    for(int i=0; i < len; i++) {
        char ch = *(str+i);
        if(ch == 0) { null_counter++; row_nulls++; } else { row_nulls = 0; }
        //if((row_nulls > 8 && len > 4)) return false;
        if((row_nulls > 4)) return false;
        if(is_letter_or_number(ch)) printable++;
        if(is_letter(ch)) letters++;
        if(ch == 0x20) spaces++;
        if(!is_printable_ascii(ch) && !is_exception_char(ch))
            non_printable++;
    }
    if(non_printable >= (((float)len)/10)*5) {return false;}
    //if(null_counter >= 5) return false;
    if(letters <= 1 || printable == 0) {return false;}
    //if(letters > 10 && spaces == 0) return false;
    //if(letters > 1 && last == 0) return true;
    if(last != 0 && last != 0x23) {return false;}
    //if(letters <= (len-null_counter)/2) return false;
    if(letters < (null_counter/2)) {return false;}
    return true;
}
