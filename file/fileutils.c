#include "file\fileutils.h"

long getFileSize(FILE* f) {
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	return size;
}

/* int backup_file_win(char* filepath) {
	struct stat st = {0};
	char filename[MAX_FILENAME];
	char partdir[MAX_DIR_PATH];
	char drive[16];
    char dir[MAX_PATH];
    if(_splitpath_s(filepath,
                        drive, sizeof(drive),
                        partdir, sizeof(partdir),
                        filename, sizeof(filename),
                        NULL, 0) != 0) {
        return -1;
    }
    if(_makepath_s(dir, sizeof(dir), drive, partdir, NULL, "backup") != 0) {
        return -1;
    }

    FILE* file = fopen(filepath, "w");

	if(stat(dir, &st) == -1) {
		mkdir(dir, 0700);
		printf("Created backup dir.\n");
	} else printf("Backup dir exists.\n");

	strcat(dir, "/");
	strcat(dir, filename);
	long file_size = getFileSize(file);
	FILE* b_file = fopen(dir, "w");
	if(b_file == NULL) {printf("Couldnt copy backup file."); return -1;}

	for(int i=0; i< file_size; i++) {
		int byte = fgetc(file);
		fwrite(&byte, 1, 1, b_file);
	}

	fseek(file, 0, SEEK_SET);
	fclose(b_file);

	return 0;
} */

char* getDir(char* filepath) {
	regex_t reg;
    regmatch_t m[1];
	char* dir = (char*) malloc(strlen(filepath)+100);
    int val;
    
    if(regcomp(&reg, ".*(/|\\\\)", REG_EXTENDED)) {
        printf("could not compile regex.");
    }
    val = regexec(&reg, filepath, 1, m, REG_EXTENDED);
    
    if(val == 0) {
        memcpy(dir, filepath, m[0].rm_eo);
        *(dir+m[0].rm_eo) = 0;
    } else {
		printf("Error, dir regex not found.\n");
	}

	return dir;
}

char* getFilename(char* filepath) {
	regex_t reg;
    regmatch_t m[1];
	char* filename = (char*) malloc(strlen(filepath)+100);
    int val;
    
    if(regcomp(&reg, ".*(/|\\\\)", REG_EXTENDED)) {
        printf("could not compile regex.");
    }
    val = regexec(&reg, filepath, 1, m, REG_EXTENDED);
    
    if(val == 0) {
        memcpy(filename, filepath+m[0].rm_eo, strlen(filepath)-m[0].rm_eo);
        *(filename+(strlen(filepath)-m[0].rm_eo)) = 0;
    } else {
		printf("Error, filename regex not found.\n");
	}

	return filename;
}

char* getFilenameNoExt(char* filepath) {
	regex_t reg;
    regmatch_t m[1];
	char* fullfilename = getFilename(filepath);
	char* filename = (char*) malloc(strlen(filepath)+100);
    int val;
    
    if(regcomp(&reg, ".*\\.", REG_EXTENDED)) {
        printf("could not compile regex.");
    }
    val = regexec(&reg, fullfilename, 1, m, REG_EXTENDED);
    
    if(val == 0) {
        memcpy(filename, fullfilename, m[0].rm_eo-1);
        *(filename+m[0].rm_eo-1) = 0;
    } else {
		printf("Error, filename regex not found.\n");
	}

	return filename;
}

char* getFilenameExt(char* filepath) {
	regex_t reg;
    regmatch_t m[1];
	char* fullfilename = getFilename(filepath);
	char* filename = (char*) malloc(strlen(filepath)+100);
    int val;
    
    if(regcomp(&reg, "\\..*", REG_EXTENDED)) {
        printf("could not compile regex.");
    }
    val = regexec(&reg, fullfilename, 1, m, REG_EXTENDED);
    
    if(val == 0) {
        memcpy(filename, fullfilename+m[0].rm_so, strlen(fullfilename)-m[0].rm_so);
        *(filename+(strlen(fullfilename)-m[0].rm_so)) = 0;
    } else {
		printf("Error, filename regex not found.\n");
	}

	return filename;
}

int backup_file_gcc(char* filepath) {
    FILE* f = fopen(filepath, "r");
	struct stat st = {0};
	char* dir = getDir(filepath);
	char* name = getFilename(filepath);

	strcat(dir, "backup");
	if(stat(dir, &st) == -1) {
		mkdir(dir, 0700);
		printf("Created backup dir.\n");
	} else printf("Backup dir exists.\n");

	strcat(dir, "/");
	strcat(dir, name);
	long file_size = getFileSize(f);
	FILE* b_file = fopen(dir, "w");
	if(b_file == NULL) {printf("Couldnt copy backup file."); return -1;}

	for(int i=0; i< file_size; i++) {
		int byte = fgetc(f);
		fwrite(&byte, 1, 1, b_file);
	}

	fseek(f, 0, SEEK_SET);
	fclose(b_file);
    free(name);

	return 0;
}
