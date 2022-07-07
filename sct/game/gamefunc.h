#define GAME_FUNCTIONS_NUM  0x139
#define MAX_FUNC_NAME 256


typedef struct game_function {
    int id;
    int params;
    char* desc;
    char* name;
} game_fun;

extern game_fun** game_functions;

void print_game_function(game_fun* gf);
void print_game_function_i(int num, game_fun** functions_arr);
void init_game_functions(game_fun** functions_arr);
void free_game_functions(game_fun** functions_arr);
game_fun* get_game_func_by_name(char* name);