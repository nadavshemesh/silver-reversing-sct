#include<stdio.h>
#include<regex.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/stat.h>

#define MAX_FILENAME 64;
#define MAX_PATH 512;
#define MAX_DIR_PATH 216;

int backup_file_win(char* filepath);
char* getDir(char* filepath);
char* getFilename(char* filepath);
long getFileSize(FILE* f);
int backup_file_gcc(char* filepath);