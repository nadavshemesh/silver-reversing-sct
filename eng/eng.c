#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<unistd.h>

#include "file\fileutils.h"
const bool debug = false;

int xor_file(char* filepath, int op) {
	int xor_val = 0x52;
	char* dir = getDir(filepath);
	char* filename = getFilenameNoExt(filepath);
	char* ext = (op == 0)? ".dec" : ".eng";
	char filepath_out[strlen(dir)+strlen(filename)+strlen(ext)];
	sprintf(filepath_out, "%s%s%s", dir, filename, ext);

	FILE* f = fopen(filepath, "r");
	if(f == NULL) { printf("Error opening file %s, exiting...", filepath); return -1; }

	long file_size = getFileSize(f);
	unsigned char buff[file_size];
	unsigned char buff_decoded[file_size];

	backup_file_gcc(filepath);
	printf("size of file: %d bytes\n", file_size);

	for(int i=0; i <= file_size; i++) {
		buff[i] = fgetc(f);
		buff_decoded[i] = buff[i]^xor_val;
		xor_val += 0x54;
		if(debug) {
			printf("%x ", buff[i]);
			printf("en:%c ", buff_decoded[i]);
		}
	}

	if(debug) {
		printf("output:\n");
		for(int i=0; i <= file_size; i++) {
			printf("%x ", buff_decoded[i]);
		}
	}

	FILE* nf = fopen(filepath_out, "w");
	for(int i=0; i <= file_size; i++) {
		fwrite(&buff_decoded[i], 1, 1, nf);
	}

	fclose(nf);
	fclose(f);

	return 0;
}

int main(int argc, char* argv[]) {
	char default_filepath[77] = "D:\\SteamLibrary\\steamapps\\common\\Silver\\silver\\levels\\gno\\veranda\\veranda.eng";
	char* filepath = &default_filepath[0];
	int op = 0;

	if(argc > 1) {
		op = atoi(argv[1]);
		if(op != 0 && op != 1) {
			printf("Error, no such operation %d. [0-decode, 1-encode]", op);
			return -1;
		}
	}

	if(argc > 2) {
		filepath = argv[2];
	}

	int ecode = xor_file(filepath, op);
	if(ecode == 0) printf("Done.\n");

	return ecode;
}
