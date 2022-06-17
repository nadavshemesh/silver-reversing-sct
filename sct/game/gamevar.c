#include "sct\game\gamevar.h"

game_var* init_gv(int id, char* name, char* desc, int first_off, int second_off, int third_off) {
    game_var* gv = w_malloc(sizeof(game_var));
    gv->name = w_malloc(strlen(name)*sizeof(char));
    gv->desc = w_malloc(strlen(desc)*sizeof(char));

    gv->id = id;
    gv->first_offset = first_off;
    gv->second_offset = second_off;
    gv->third_offset = third_off;

    memcpy(gv->name, name, strlen(name)*sizeof(char));
    memcpy(gv->desc, desc, strlen(desc)*sizeof(char));

    return gv;
}

game_var* init_gv_room_cleared() {
    int id = 0;
    char name[] = "is_room_cleared";
    char desc[] = "used to mark if the room is already finished. saved after leaving room.";
    int fo = 0x04;
    int so = 0x09;
    int to = 0x2C;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_room_state() {
    int id = 1;
    char name[] = "room_state";
    char desc[] = "used to determine state. saved after leaving room.";
    int fo = 0x04;
    int so = 0x09;
    int to = 0x08;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_room_timer() {
    int id = 2;
    char name[] = "room_timer";
    char desc[] = "used to get the time elapsed since entrance. reset when leaving room.";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x5b;

    return init_gv(id, name, desc, fo, so, to);
}

void init_game_vars(game_var** game_vars) {
    // game_vars = w_malloc(GAME_VARS_NUM*sizeof(game_var*));
    game_vars[0] = init_gv_room_cleared();
    game_vars[1] = init_gv_room_state();
    game_vars[2] = init_gv_room_timer();
}

game_var* get_game_var_by_offsets(int first_offset, int second_offset, int third_offset) {
    // printf("%02x %02x %02x\n", first_offset, second_offset, third_offset);
    first_offset &= 0x00FFFFFF; // ignore first byte (negation)
    if(game_vars == NULL) print_err_and_exit("Error, game_vars is undefined.", -5);
    for(int i=0; i < GAME_VARS_NUM; i++) {
        game_var* gv = game_vars[i];
        if(gv->first_offset == first_offset && gv->second_offset == second_offset && gv->third_offset == third_offset) {
            return gv;
        }
    }

    return NULL;
}

game_var* get_game_var_by_name(char* name) {
    if(*name == '~') { name += 1; } // ignore negation if exists
    if(game_vars == NULL) print_err_and_exit("Error, game_vars is undefined.", -5);
    for(int i=0; i < GAME_VARS_NUM; i++) {
        game_var* gv = game_vars[i];
        if(strlen(gv->name) == strlen(name) && strcmp(gv->name, name) == 0) {
            return gv;
        }
    }

    return NULL;
}