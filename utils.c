#include "utils.h"

 void *w_malloc(size_t size) {
    void* res = malloc(size);
    if(res == NULL) {
        printf("malloc error.\n");
        exit(-1);
    }
    return res;
}

char *aapts(char* str) {
    char* alloc_str = w_malloc(strlen(str));
    strncpy(alloc_str, str, strlen(str));

    return alloc_str;
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

bool is_letter(char ch) {
    return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <=122) || (ch >= 48 && ch <= 57);
}

bool is_string(byte* str, int len) {
    int null_counter = 0;
    int row_nulls = 0;
    int non_printable = 0;
    int letters = 0;
    for(int i=0; i < len; i++) {
        char ch = *(str+i);
        if(ch == 0) { null_counter++; row_nulls++; } else { row_nulls = 0; }
        if(row_nulls >= 4) return false;
        if(is_letter(ch)) letters++;
        if(!is_printable_ascii(ch) && !is_exception_char(ch))
            non_printable++;
    }
    if(non_printable >= (len/2)) return false;
    if(null_counter >= len/2) return false;
    if(letters <= (len-null_counter)/2) return false;
    return true;
}
