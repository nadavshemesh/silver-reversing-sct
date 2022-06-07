#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include "sct\convert\bin\sctbin.h"
#include "sct\convert\asm\sctasm.h"

int get_script_off(int script_num, sct_f* sf);
sct_f* disasm_file(char* filepath);
script* disasm_script(int script_offset, sct_f* sf);
bool is_filepath_valid(char* filepath);
int count_file_sections(sct_f* sf);
char** tokenize_next_section(sct_f* sf);