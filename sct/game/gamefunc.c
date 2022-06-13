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
        char* num = w_malloc(4);
        char* prefix = "func_";

        // c_itoa(i,num);
        sprintf(num, "%x", i);
        strncat(name, prefix, strlen(prefix));
        strcat(name, num);
        gf->name = name;
        functions_arr[i] = gf;
    }

    functions_arr[0x97]->name = aapts("create_enemy");
    functions_arr[0x97]->params = 4;
    functions_arr[0x97]->desc = aapts("(type, id, position_ptr, -)");
    functions_arr[0xb7]->name = aapts("load_anim");
    functions_arr[0xb7]->name = aapts("load_anim");
    functions_arr[0xb7]->params = 1;
    functions_arr[0xb7]->desc = aapts("(animation_name)");
    functions_arr[0xbd]->name = aapts("load_sound");
    functions_arr[0xbd]->params = 1;
    functions_arr[0xbd]->desc = aapts("(sound_num)");
    functions_arr[0xc8]->name = aapts("play_bgm");
    functions_arr[0xc8]->params = 1;
    functions_arr[0xc8]->desc = aapts("(name)");
    functions_arr[0x69]->name = aapts("get_char_handle");
    functions_arr[0x69]->params = 1;
    functions_arr[0x69]->desc = aapts("(char_id)");
    functions_arr[0x48]->name = aapts("freeze_enemy");
    functions_arr[0x48]->params = 1;
    functions_arr[0x48]->desc = aapts("(char_ptr)");
    functions_arr[0xc6]->name = aapts("fly_to_pos");
    functions_arr[0xc6]->params = 3;
    functions_arr[0xc6]->desc = aapts("(char_ptr, unknown int, const pos_ptr)");
    functions_arr[0x1a]->name = aapts("char_walk_to_pos");
    functions_arr[0x1a]->params = 2;
    functions_arr[0x1a]->desc = aapts("(character, position_ptr)");
    functions_arr[0x81]->name = aapts("get_num_of_enemies");
    functions_arr[0x81]->params = 0;
    functions_arr[0x81]->desc = aapts("()");
    functions_arr[0x06]->name = aapts("put_item_in_char_inv");
    functions_arr[0x06]->params = 4;
    functions_arr[0x06]->desc = aapts("(is_equiped, is_dropped, item, character)");
    functions_arr[0xa4]->name = aapts("eval");
    functions_arr[0xa4]->params = 2;
    functions_arr[0xa4]->desc = aapts("(runtime_script_ptr , unknown)");
    functions_arr[0xA2]->name = aapts("play_loaded_sound");
    functions_arr[0xA2]->params = 4;
    functions_arr[0xA2]->desc = aapts("(sound_offset, mode, id, unknown)");
    functions_arr[0xD3]->name = aapts("randomize_int");
    functions_arr[0xD3]->params = 2;
    functions_arr[0xD3]->desc = aapts("(from, to)");

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
        }
        return NULL; // not found.
    } else print_err_and_exit("Game functions uninitialized.", -4);
}
