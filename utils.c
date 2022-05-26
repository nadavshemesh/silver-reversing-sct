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
    printf(ANSI_COLOR_MAGENTA "\n\t -%s- \t" ANSI_COLOR_RESET "\n", msg);
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

node* init_node_list(void* item) {
    node* node = w_malloc(sizeof(node));
    node->id = 231;
    node->item = item;
    node->next = NULL;

    return node;
}

node* create_node(void* item) {
    node* n = w_malloc(sizeof(node));
    n->id = 231;
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