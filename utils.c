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