#define MAX_FILENAME 64;
#define MAX_PATH 512;
#define MAX_DIR_PATH 216;

int backup_file_win(char* filepath);
char* getDir(char* filepath);
char* getFilename(char* filepath);
long getFileSize(FILE* f);
int backup_file_gcc(char* filepath);