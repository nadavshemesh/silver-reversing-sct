#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

void print_err(char* err, int err_code) {
    printf("%s\n", err);
}

void print_err_and_exit(char* err, int err_code) {
    printf("%s", err);
    exit(err_code);
}