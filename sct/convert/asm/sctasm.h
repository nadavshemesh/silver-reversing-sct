#include<stdio.h>
#include<stdbool.h>

#include "sct\sct.h"
#include "sct\print\asm\console.h"

#define MAX_ASM_TOKEN_LEN 256


void build_data_section(sct_f* sf);
cp_cmp_result asm_identify_cp(char*** token_pos_ptr);