#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "utils.h"
#include "sct\game\gamefunc.h"

bool game_functions_initialized = false;

void print_game_function(game_fun* gf) {
    printf("func_name: %s\n", gf->name);
    printf("func_params: %d\n", gf->params);
    printf("func_description: %s\n", gf->desc);
}

void print_game_function_i(int num, game_fun** functions_arr) {
    game_fun* gf = functions_arr[num];
    if(gf == NULL) { print_err_and_exit("error - game_functions uninitialized.\n", -2); }
    else print_game_function(gf);
}

void init_game_functions(game_fun** functions_arr) {
    for(int i=0; i<GAME_FUNCTIONS_NUM; i++) {
        game_fun* gf = w_malloc(sizeof(game_fun));
        gf->id = i;
        char* name = w_malloc(MAX_FUNC_NAME);
        char* prefix = "func_";

        // c_itoa(i,num);
        sprintf(name, "%s%x", prefix, i);
        gf->name = name;
        gf->default_name = name;
        functions_arr[i] = gf;
    }

    functions_arr[0x97]->name = aapts("create_enemy");
    functions_arr[0x97]->params = 4;
    functions_arr[0x97]->desc = aapts("(int char_type, int id, var position_ptr, int radius)");
    functions_arr[0x93]->name = aapts("create_char");
    functions_arr[0x93]->params = 4;
    functions_arr[0x93]->desc = aapts("(var pos_ptr, var char_name_ptr, int unknown, int unknown)");
    functions_arr[0x25]->name = aapts("is_char_standing");
    functions_arr[0x25]->params = 1;
    functions_arr[0x25]->desc = aapts("(var char_ptr)");
    functions_arr[0x4b]->name = aapts("start_dialog");
    functions_arr[0x4b]->params = 2;
    functions_arr[0x4b]->desc = aapts("(var start_dialog, var end_dialog)");
    functions_arr[0x2c]->name = aapts("char_exit_door");
    functions_arr[0x2c]->params = 2;
    functions_arr[0x2c]->desc = aapts("(var char_ptr, int door_number)");
    functions_arr[0x76]->name = aapts("get_input_sys");
    functions_arr[0x76]->params = 0;
    functions_arr[0x76]->desc = aapts("()");
    functions_arr[0x5f]->name = aapts("char_stop");
    functions_arr[0x5f]->params = 1;
    functions_arr[0x5f]->desc = aapts("(var char_ptr)");
    functions_arr[0x12c]->name = aapts("play_char_anim");
    functions_arr[0x12c]->params = 3;
    functions_arr[0x12c]->desc = aapts("(var char_ptr, var anim_name_ptr, int unknown)");
    functions_arr[0xa8]->name = aapts("is_char_at_pos");
    functions_arr[0xa8]->params = 3;
    functions_arr[0xa8]->desc = aapts("(var char_ptr, var pos_ptr, int radius)");
    functions_arr[0x130]->name = aapts("char_unfollow");
    functions_arr[0x130]->params = 1;
    functions_arr[0x130]->desc = aapts("(var char1_ptr, var char2_ptr)");
    functions_arr[0x131]->name = aapts("char_follow_char");
    functions_arr[0x131]->params = 2;
    functions_arr[0x131]->desc = aapts("(var char1_ptr, var char2_ptr)");
    functions_arr[0xb2]->name = aapts("remove_char");
    functions_arr[0xb2]->params = 1;
    functions_arr[0xb2]->desc = aapts("(var char_ptr)");
    functions_arr[0xb7]->name = aapts("load_anim");
    functions_arr[0xb7]->params = 1;
    functions_arr[0xb7]->desc = aapts("(var anim_name_ptr)");
    functions_arr[0xbd]->name = aapts("load_sound");
    functions_arr[0xbd]->params = 1;
    functions_arr[0xbd]->desc = aapts("(int sound_num)");
    functions_arr[0xc8]->name = aapts("play_bgm");
    functions_arr[0xc8]->params = 1;
    functions_arr[0xc8]->desc = aapts("(var name_ptr)");
    functions_arr[0x69]->name = aapts("get_char_handle");
    functions_arr[0x69]->params = 1;
    functions_arr[0x69]->desc = aapts("(int char_id)");
    functions_arr[0x51]->name = aapts("set_char_behav");
    functions_arr[0x51]->params = 2;
    functions_arr[0x51]->desc = aapts("(var char_ptr, int char_type)");
    functions_arr[0x48]->name = aapts("freeze_enemy");
    functions_arr[0x48]->params = 1;
    functions_arr[0x48]->desc = aapts("(var char_ptr)");
    functions_arr[0xc6]->name = aapts("fly_to_pos");
    functions_arr[0xc6]->params = 3;
    functions_arr[0xc6]->desc = aapts("(var char_ptr, unknown int, var pos_ptr)");
    functions_arr[0x1a]->name = aapts("char_walk_to_pos");
    functions_arr[0x1a]->params = 2;
    functions_arr[0x1a]->desc = aapts("(var char_ptr, var position_ptr)");
    functions_arr[0x72]->name = aapts("get_selected_hero");
    functions_arr[0x72]->params = 0;
    functions_arr[0x72]->desc = aapts("()");
    functions_arr[0x81]->name = aapts("get_num_of_enemies");
    functions_arr[0x81]->params = 0;
    functions_arr[0x81]->desc = aapts("()");
    functions_arr[0x06]->name = aapts("put_item_in_char_inv");
    functions_arr[0x06]->params = 4;
    functions_arr[0x06]->desc = aapts("(int is_equiped, int is_dropped, int item_type, var char_ptr)");
    functions_arr[0xa4]->name = aapts("eval");
    functions_arr[0xa4]->params = 2;
    functions_arr[0xa4]->desc = aapts("(var runtime_script_ptr , int unknown)");
    functions_arr[0xa2]->name = aapts("play_loaded_sound");
    functions_arr[0xa2]->params = 4;
    functions_arr[0xa2]->desc = aapts("(int sound_offset, int mode[0 -> once -1 -> repeat], int id, var pos_ptr or int 0)");
    functions_arr[0x11c]->name = aapts("set_sound_volume");
    functions_arr[0x11c]->params = 2;
    functions_arr[0x11c]->desc = aapts("(var sound_ptr, int volume)");
    functions_arr[0x125]->name = aapts("stop_playing_sound");
    functions_arr[0x125]->params = 1;
    functions_arr[0x125]->desc = aapts("(var sound_ptr)");
    functions_arr[0xd3]->name = aapts("randomize_int");
    functions_arr[0xd3]->params = 2;
    functions_arr[0xd3]->desc = aapts("(int from, int to)");
    functions_arr[0xcd]->name = aapts("log");
    functions_arr[0xcd]->params = 1;
    functions_arr[0xcd]->desc = aapts("(var string_ptr)");
    functions_arr[0xf4]->name = aapts("play_3d_fx");
    functions_arr[0xf4]->params = 4;
    functions_arr[0xf4]->desc = aapts("(var fx_name, var pos_ptr, int unkown, int unknown)");
    functions_arr[0x4f]->name = aapts("set_char_init_state");
    functions_arr[0x4f]->params = 1;
    functions_arr[0x4f]->desc = aapts("(int init_state)");
    functions_arr[0x111]->name = aapts("set_input_system");
    functions_arr[0x111]->params = 1;
    functions_arr[0x111]->desc = aapts("(int type)");
    functions_arr[0x16]->name = aapts("is_char_exist");
    functions_arr[0x16]->params = 1;
    functions_arr[0x16]->desc = aapts("(int char_id)");
    functions_arr[0x107]->name = aapts("set_char_anim_seq");
    functions_arr[0x107]->params = 2;
    functions_arr[0x107]->desc = aapts("(var char_ptr, var anim_table_ptr)");
    functions_arr[0x20]->name = aapts("play_char_anim_seq");
    functions_arr[0x20]->params = 1;
    functions_arr[0x20]->desc = aapts("(var char_ptr)");
    functions_arr[0xd5]->name = aapts("reset_local_timer");
    functions_arr[0xd5]->params = 0;
    functions_arr[0xd5]->desc = aapts("()");
    functions_arr[0x129]->name = aapts("is_local_timer_greater_than");
    functions_arr[0x129]->params = 1;
    functions_arr[0x129]->desc = aapts("(int time)");
    functions_arr[0x12a]->name = aapts("is_local_timer_between");
    functions_arr[0x12a]->params = 2;
    functions_arr[0x12a]->desc = aapts("(int min_time, int max_time)");
    functions_arr[0x35]->name = aapts("char_turn_to_pos");
    functions_arr[0x35]->params = 2;
    functions_arr[0x35]->desc = aapts("(var char_ptr, var pos_ptr)");
    functions_arr[0xa0]->name = aapts("play_2d_anim");
    functions_arr[0xa0]->params = 4;
    functions_arr[0xa0]->desc = aapts("(var anim_name_ptr, int on_repeat, unknown but always 1, var play_flag_ptr or 0 to always play)");

    game_functions_initialized = true;
}

void free_game_functions(game_fun** functions_arr) {
    for(int i=0; i<GAME_FUNCTIONS_NUM; i++) {
        free(functions_arr[i]);
    }
}

game_fun* get_game_func_by_name(char* name) {
    if(game_functions_initialized) {
        for(int i=0; i<GAME_FUNCTIONS_NUM; i++) {
            game_fun* gf = game_functions[i];
            if(strlen(name) == strlen(gf->name) && strcmp(gf->name, name) == 0) {
                return gf;
            }
            if(strlen(name) == strlen(gf->default_name) && strcmp(gf->default_name, name) == 0) {
                return gf;
            }
        }
        return NULL; // not found.
    } else print_err_and_exit("Game functions uninitialized.", -4);
}
