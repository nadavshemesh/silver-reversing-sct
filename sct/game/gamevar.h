#define GAME_VARS_NUM  115
#define MAX_GAME_VAR_NAME 256

#include "utils.h"

typedef struct game_var {
    int id;
    int first_offset;
    int second_offset;
    int third_offset;
    int len;
    char* desc;
    char* name;
} game_var;

extern game_var** game_vars;
void init_game_vars(game_var** game_vars);
game_var* get_game_var_by_offsets(int first_offset, int second_offset, int third_offset);
game_var* get_game_var_by_name(char* name);
