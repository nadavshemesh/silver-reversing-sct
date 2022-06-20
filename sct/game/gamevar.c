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

game_var* init_gv_unknown25() {
    int id = 3;
    char name[] = "unknown_25";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x25;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown44() {
    int id = 4;
    char name[] = "unknown_44";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x44;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown53() {
    int id = 5;
    char name[] = "unknown_53";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x53;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown45c() {
    int id = 6;
    char name[] = "unknown_45c";
    char desc[] = "";
    int fo = 0x04;
    int so = 0x05;
    int to = 0x0c;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown4510() {
    int id = 7;
    char name[] = "unknown_4510";
    char desc[] = "";
    int fo = 0x04;
    int so = 0x05;
    int to = 0x10;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown4514() {
    int id = 8;
    char name[] = "unknown_4514";
    char desc[] = "";
    int fo = 0x04;
    int so = 0x05;
    int to = 0x14;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown51() {
    int id = 9;
    char name[] = "unknown_02051";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x51;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown55() {
    int id = 10;
    char name[] = "unknown_02055";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x55;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown20() {
    int id = 11;
    char name[] = "unknown_02020";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x20;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown40() {
    int id = 11;
    char name[] = "unknown_02040";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x40;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown16() {
    int id = 11;
    char name[] = "unknown_02016";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x16;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown17() {
    int id = 11;
    char name[] = "unknown_02017";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x17;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown45f0() {
    int id = 11;
    char name[] = "unknown_045f0";
    char desc[] = "";
    int fo = 0x04;
    int so = 0x05;
    int to = 0xf0;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown45e4() {
    int id = 11;
    char name[] = "unknown_045e4";
    char desc[] = "";
    int fo = 0x04;
    int so = 0x05;
    int to = 0xe4;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown45e8() {
    int id = 11;
    char name[] = "unknown_045e8";
    char desc[] = "";
    int fo = 0x04;
    int so = 0x05;
    int to = 0xe8;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown45ec() {
    int id = 11;
    char name[] = "unknown_045ec";
    char desc[] = "";
    int fo = 0x04;
    int so = 0x05;
    int to = 0xec;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown433c() {
    int id = 11;
    char name[] = "unknown_0433c";
    char desc[] = "";
    int fo = 0x04;
    int so = 0x03;
    int to = 0x3c;

    return init_gv(id, name, desc, fo, so, to);
}

game_var* init_gv_unknown202e() {
    int id = 11;
    char name[] = "unknown_0202e";
    char desc[] = "";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x2e;

    return init_gv(id, name, desc, fo, so, to);
}

void init_game_vars(game_var** game_vars) {
    // game_vars = w_malloc(GAME_VARS_NUM*sizeof(game_var*));
    game_vars[0] = init_gv_room_cleared();
    game_vars[1] = init_gv_room_state();
    game_vars[2] = init_gv_room_timer();
    // game_vars[3] = init_gv_unknown25();
    // game_vars[4] = init_gv_unknown44();
    // game_vars[5] = init_gv_unknown53();
    // game_vars[6] = init_gv_unknown45c();
    // game_vars[7] = init_gv_unknown4510();
    // game_vars[8] = init_gv_unknown4514();
    // game_vars[9] = init_gv_unknown51();
    // game_vars[10] = init_gv_unknown55();
    // game_vars[11] = init_gv_unknown20();
    // game_vars[12] = init_gv_unknown40();
    // game_vars[13] = init_gv_unknown16();
    // game_vars[14] = init_gv_unknown17();
    // game_vars[15] = init_gv_unknown45f0();
    // game_vars[16] = init_gv_unknown45e4();
    // game_vars[17] = init_gv_unknown45e8();
    // game_vars[18] = init_gv_unknown45ec();
    // game_vars[19] = init_gv_unknown433c();
    // game_vars[20] = init_gv_unknown202e();
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
    char name[256];
    sprintf(name, "unknown_%02x_%02x_%02x", first_offset, second_offset, third_offset);
    game_var* gv = init_gv(0, name, "", first_offset, second_offset, third_offset);
    return gv;

    // return NULL;
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

    int fo, so, to;
    sscanf(name, "unknown_%02x_%02x_%02x",&fo, &so, &to);
    game_var* gv = init_gv(0, name, "", fo, so, to);
    return gv;
    // return NULL;
}