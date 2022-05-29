#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define MAX_LOCAL_ARR 1024

/* to be used for local var arrays */
typedef struct i_array {
    long len;
    int arr[MAX_LOCAL_ARR];
} i_arr;
typedef struct s_array {
    long len;
    char *arr[MAX_LOCAL_ARR];
} s_arr;

typedef struct node {
    void* item;
    struct node* next;
} node;

void *w_malloc(size_t size);
char *aapts(char* str);
char* c_itoa(int num, char *str);
void print_title(char* msg);
void print_info(char* msg);
void print_success(char* msg);
void print_warn(char* warning, int warning_code);
void print_err(char* err, int err_code);
void print_err_and_exit(char* err, int err_code);
node* init_node_list(void* item);
node* create_node(void* item);
int count_node_list(node** head);
node* get_node(node** head, int pos);
void insert_node(node** head, node* new);
void link_node(node** head, node* node, int pos);
void remove_node(node** head, int pos);