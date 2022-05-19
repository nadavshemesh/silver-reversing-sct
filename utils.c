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