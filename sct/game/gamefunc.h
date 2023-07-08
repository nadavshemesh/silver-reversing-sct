#define GAME_FUNCTIONS_NUM  0x139
#define MAX_FUNC_NAME 256
#define ENEMY_GEN_CP_NUM  15

typedef enum enemy_gen_order { SINGLE, SEQUENTIAL, RANDOM } enemy_gen_order;
typedef enum enemy_gen_cp_type { DOOR, POS, POS_ORDER, WAVES, ENEMIES_NUM, WAVE_TRIGGER_THRESHOLD, DROPS, DELAY_ONE, DELAY_TWO, DEST, UNKNOWN_ENEMY_GEN_TYPE } enemy_gen_cp_type;

#include "sct\catalog.h"
#include "sct\structure\object.h"

typedef struct catalog_ref {
    cat_type type;
    int var_index;
    char* prefix;
    char* postfix;
} catalog_ref;

typedef struct enemy_gen_script {
    int door;
    int num_of_positions;
    enemy_gen_order order;
    int num_of_waves;
    int num_of_enemies_in_list;
    int num_of_items_in_list;
    int num_of_enemies_in_each_wave;
    int num_of_enemies_to_trigger_next_wave;
    int delay_between_enemies;
    int delay_for_first_enemy;
    int pos_var_id;
    int dest_var_id;
    int structs_recognized;
    char* enemy_gen_script_var_name;
    char* pos_var_name;
    char* dest_var_name;
    int* enemies_id_list;
    int* items_drop_id_list;
} enemy_gen_script;

typedef struct var_hint_name {
    int used_counter;
    char* name;
} var_hint_name;

typedef struct type_forcing {
    data_type type;
    int var_index;
    var_hint_name* hint_name;
} forced_type;

typedef struct game_function {
    int id;
    int params;
    char* desc;
    char* default_name;
    char* name;
    node** forced_types;
    catalog_ref* cat_ref;
} game_fun;

extern game_fun** game_functions;

void print_game_function(game_fun* gf);
void print_game_function_i(int num, game_fun** functions_arr);
enemy_gen_script* create_enemy_gen_script_from_data_obj(data_obj* gen_script);
void print_enemy_gen_script(enemy_gen_script* egs);
void init_game_functions(game_fun** functions_arr);
void free_game_functions(game_fun** functions_arr);
game_fun* get_game_func_by_name(char* name);
void init_enemy_gen_code_patterns();