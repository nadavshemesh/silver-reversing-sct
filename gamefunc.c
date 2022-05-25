#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "utils.h"
#include "gamefunc.h"

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
    functions_arr[0x06]->name = aapts("put_item_in_char_inv");
    functions_arr[0x06]->params = 4;
    functions_arr[0x06]->desc = aapts("(is_equiped, is_dropped, item, character)");
}

void free_game_functions(game_fun** functions_arr) {
    for(int i=0; i<GAME_FUNCTIONS_NUM; i++) {
        free(functions_arr[i]);
    }
}
