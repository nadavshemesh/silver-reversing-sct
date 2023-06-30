#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "utils.h"
#include "sct\game\gamefunc.h"

bool game_functions_initialized = false;
node* var_hint_names = NULL;

void print_game_function(game_fun* gf) {
    printf("func_name: %s\n", gf->name);
    printf("func_params: %d\n", gf->params);
    printf("func_description: %s\n", gf->desc);
}

catalog_ref* create_cat_ref(cat_type type, int var_index, char* prefix, char* postfix) {
    catalog_ref* cref = w_malloc(sizeof(catalog_ref));
    cref->type = type;
    cref->var_index = var_index;
    cref->prefix = (prefix != NULL) ? aapts(prefix) : NULL;
    cref->postfix = (postfix != NULL) ? aapts(postfix) : NULL;

    return cref;
}

var_hint_name* init_vhn(char* name) {
    var_hint_name* vhn = w_malloc(sizeof(var_hint_name));
    vhn->name = aapts(name);
    vhn->used_counter = 0;

    return vhn;
}

var_hint_name* get_or_create_vhn(char* name) {
    if(var_hint_names == NULL) {
        var_hint_name* vhn = init_vhn(name);
        var_hint_names = create_node(vhn);
        return vhn;
    }
    // check the first one seperately
    node* next_vhn = var_hint_names;
    if(strs_identical(((var_hint_name*)next_vhn->item)->name, name)) {
        return next_vhn->item;
    }

    while(next_vhn->next != NULL) {
        next_vhn = next_vhn->next;
        if(strs_identical(((var_hint_name*)next_vhn->item)->name, name)) {
            return next_vhn->item;
        }
    }
    var_hint_name* vhn = init_vhn(name);
    next_vhn->next = create_node(vhn);
    return vhn;
}

void add_forced_type(game_fun* gf, data_type type, int var_index, char* hint_name) {
    forced_type* ft = w_malloc(sizeof(forced_type));
    ft->type = type;
    ft->var_index = var_index;
    ft->hint_name = (hint_name != NULL) ? get_or_create_vhn(hint_name) : NULL;

    node* ft_node = create_node(ft);
    if(*gf->forced_types == NULL) {
        *gf->forced_types = ft_node;
    } else {
        insert_node(gf->forced_types, ft_node);
    }
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
        gf->forced_types = w_malloc(sizeof(node*));
        char* name = w_malloc(MAX_FUNC_NAME);
        char* prefix = "func_";
        gf->cat_ref = NULL;

        // c_itoa(i,num);
        sprintf(name, "%s%x", prefix, i);
        gf->name = name;
        gf->default_name = name;
        functions_arr[i] = gf;
    }

    functions_arr[0x07]->name = aapts("save_pos_to_room_memory");
    functions_arr[0x07]->params = 4;
    functions_arr[0x07]->desc = aapts("(int id, int x, int y, int z");
    functions_arr[0xd8]->name = aapts("load_pos_from_room_memory");
    functions_arr[0xd8]->params = 2;
    functions_arr[0xd8]->desc = aapts("(int id, var unpack_memory_location");
    functions_arr[0x96]->name = aapts("create_enemy_through_door");
    functions_arr[0x96]->params = 4;
    functions_arr[0x96]->desc = aapts("(int enemy_type, int id, int door, int face_direction_radius_counter_clockwise)");
    functions_arr[0x96]->cat_ref =  create_cat_ref(ENEMY_CAT, 0, NULL, NULL);
    functions_arr[0x97]->name = aapts("create_enemy");
    functions_arr[0x97]->params = 4;
    functions_arr[0x97]->desc = aapts("(int enemy_type, int id, var position_ptr, int face_direction_radius_counter_clockwise)");
    functions_arr[0x97]->cat_ref =  create_cat_ref(ENEMY_CAT, 0, NULL, NULL);
    add_forced_type(functions_arr[0x97], ARRAY, 2, "init_position");
    functions_arr[0x93]->name = aapts("create_char");
    functions_arr[0x93]->params = 4;
    functions_arr[0x93]->desc = aapts("(var pos_ptr, string char_string_id, int face_direction_radius_counter_clockwise, int init_state)");
    functions_arr[0x93]->cat_ref =  create_cat_ref(NO_CAT_USE_PARAM_STRING, 1, NULL, NULL);
    add_forced_type(functions_arr[0x93], ARRAY, 0, "init_position");
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
    functions_arr[0xb8]->name = aapts("load_item");
    functions_arr[0xb8]->params = 1;
    functions_arr[0xb8]->desc = aapts("(int item_type)");
    functions_arr[0xb8]->cat_ref =  create_cat_ref(ITEM_CAT, 0, NULL, NULL);
    functions_arr[0x12c]->name = aapts("play_char_anim");
    functions_arr[0x12c]->params = 3;
    functions_arr[0x12c]->desc = aapts("(var char_ptr, var anim_name_ptr, int unknown)");
    functions_arr[0xa8]->name = aapts("is_char_at_pos");
    functions_arr[0xa8]->params = 3;
    functions_arr[0xa8]->desc = aapts("(var char_ptr, var pos_ptr, int radius)");
    add_forced_type(functions_arr[0xa8], ARRAY, 1, "position");
    functions_arr[0x130]->name = aapts("char_unfollow_char");
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
    functions_arr[0xb7]->cat_ref =  create_cat_ref(NO_CAT_USE_PARAM_STRING, 0, NULL, "_anim");
    functions_arr[0xbd]->name = aapts("load_sound_set");
    functions_arr[0xbd]->params = 1;
    functions_arr[0xbd]->desc = aapts("(int sound_num)");
    functions_arr[0xc8]->name = aapts("play_bgm");
    functions_arr[0xc8]->params = 1;
    functions_arr[0xc8]->desc = aapts("(var name_ptr)");
    functions_arr[0xbc]->name = aapts("load_char");
    functions_arr[0xbc]->params = 1;
    functions_arr[0xbc]->desc = aapts("(var char_name_ptr)");
    functions_arr[0xff]->name = aapts("force_scroll_to_pos");
    functions_arr[0xff]->params = 1;
    functions_arr[0xff]->desc = aapts("(var pos_ptr)");
    add_forced_type(functions_arr[0xff], ARRAY, 0, "scroll_position");
    functions_arr[0x69]->name = aapts("get_char_handle");
    functions_arr[0x69]->params = 1;
    functions_arr[0x69]->desc = aapts("(int char_handle_id)");
    functions_arr[0x69]->cat_ref = create_cat_ref(HANDLE_CAT, 0, NULL, NULL);
    functions_arr[0x51]->name = aapts("set_char_template");
    functions_arr[0x51]->params = 2;
    functions_arr[0x51]->desc = aapts("(var char_ptr, int char_type)");
    functions_arr[0xbb]->name = aapts("load_char_anim");
    functions_arr[0xbb]->params = 2;
    functions_arr[0xbb]->desc = aapts("(var char_ptr, var anim_table)");
    functions_arr[0x18]->name = aapts("char_walk_to");
    functions_arr[0x18]->params = 4;
    functions_arr[0x18]->desc = aapts("(var char_ptr, int x, int y, int z)");
    functions_arr[0x0d]->name = aapts("char_play_attack_anim");
    functions_arr[0x0d]->params = 4;
    functions_arr[0x0d]->desc = aapts("(var char_ptr, int attack, var pos_ptr, int unknown)");
    add_forced_type(functions_arr[0x0d], ARRAY, 2, "att_position");
    functions_arr[0x0e]->name = aapts("remove_face");
    functions_arr[0x0e]->params = 1;
    functions_arr[0x0e]->desc = aapts("(var pos_ptr)");
    functions_arr[0x6e]->name = aapts("load_next_dialog");
    functions_arr[0x6e]->params = 2;
    functions_arr[0x6e]->desc = aapts("(var dialog_name_ptr_1, var dialog_name_ptr_2)");
    functions_arr[0x10d]->name = aapts("load_dialog_by_number");
    functions_arr[0x10d]->params = 3;
    functions_arr[0x10d]->desc = aapts("(var dialog_name_ptr_1, var dialog_name_ptr_2, int dialog number)");
    functions_arr[0x48]->name = aapts("freeze_enemy");
    functions_arr[0x48]->params = 1;
    functions_arr[0x48]->desc = aapts("(var char_ptr)");
    functions_arr[0x43]->name = aapts("kill_char");
    functions_arr[0x43]->params = 1;
    functions_arr[0x43]->desc = aapts("(var char_ptr)");
    functions_arr[0xc6]->name = aapts("fly_to_pos");
    functions_arr[0xc6]->params = 3;
    functions_arr[0xc6]->desc = aapts("(var char_ptr, int is_run, var pos_ptr)");
    add_forced_type(functions_arr[0xc6], ARRAY, 2, "fly_position");
    functions_arr[0x1a]->name = aapts("char_walk_to_pos");
    functions_arr[0x1a]->params = 2;
    functions_arr[0x1a]->desc = aapts("(var char_ptr, var position_ptr)");
    add_forced_type(functions_arr[0x1a], ARRAY, 1, "walk_position");
    functions_arr[0x10b]->name = aapts("char_teleport_to_pos");
    functions_arr[0x10b]->params = 3;
    functions_arr[0x10b]->desc = aapts("(var char_ptr, var position_ptr, int unknown)");
    add_forced_type(functions_arr[0x10b], ARRAY, 1, "tele_position");
    functions_arr[0x08]->name = aapts("create_waypoint");
    functions_arr[0x08]->params = 3;
    functions_arr[0x08]->desc = aapts("(int x, int y, int z)");
    functions_arr[0x10]->name = aapts("add_waypoint_to_char");
    functions_arr[0x10]->params = 2;
    functions_arr[0x10]->desc = aapts("(var char_ptr, int waypoint)");
    functions_arr[0x3f]->name = aapts("clear_char_waypoints");
    functions_arr[0x3f]->params = 0;
    functions_arr[0x3f]->desc = aapts("()");
    functions_arr[0x72]->name = aapts("get_selected_hero");
    functions_arr[0x72]->params = 0;
    functions_arr[0x72]->desc = aapts("()");
    functions_arr[0x72]->cat_ref =  create_cat_ref(NO_CAT_USE_PREFIX_AS_HARDCODED_STRING, 0, "selected_hero", NULL);
    functions_arr[0x70]->name = aapts("get_david_char");
    functions_arr[0x70]->params = 0;
    functions_arr[0x70]->desc = aapts("()");
    functions_arr[0x70]->cat_ref =  create_cat_ref(NO_CAT_USE_PREFIX_AS_HARDCODED_STRING, 0, "david", NULL);
    functions_arr[0x81]->name = aapts("get_num_of_enemies");
    functions_arr[0x81]->params = 0;
    functions_arr[0x81]->desc = aapts("()");
    functions_arr[0x81]->cat_ref =  create_cat_ref(NO_CAT_USE_PREFIX_AS_HARDCODED_STRING, 0, "enemies_num", NULL);
    functions_arr[0x06]->name = aapts("add_item_to_char");
    functions_arr[0x06]->params = 4;
    functions_arr[0x06]->desc = aapts("(var char_ptr, int item_type, int is_equiped, int is_dropped)");
    functions_arr[0x06]->cat_ref =  create_cat_ref(ITEM_CAT, 1, NULL, NULL);
    functions_arr[0x33]->name = aapts("create_char_game_history_link");
    functions_arr[0x33]->params = 1;
    functions_arr[0x33]->desc = aapts("(var char_ptr)");
    functions_arr[0x23]->name = aapts("char_approach_char_pos");
    functions_arr[0x23]->params = 2;
    functions_arr[0x23]->desc = aapts("(var src_char_ptr, var dest_char_ptr, int radius)");
    functions_arr[0x62]->name = aapts("alloc_space_in_general_heap");
    functions_arr[0x62]->params = 0;
    functions_arr[0x62]->desc = aapts("()");
    functions_arr[0xa4]->name = aapts("run_enemy_generator");
    functions_arr[0xa4]->params = 2;
    functions_arr[0xa4]->desc = aapts("(var runtime_script_ptr , int time_delay)");
    add_forced_type(functions_arr[0xa4], ARRAY, 0, "enemy_gen_script");
    functions_arr[0xe5]->name = aapts("toggle_or_use_char_item");
    functions_arr[0xe5]->params = 2;
    functions_arr[0xe5]->desc = aapts("(var char_ptr , var item_ptr)");
    functions_arr[0xae]->name = aapts("get_char_item_if_equiped");
    functions_arr[0xae]->params = 2;
    functions_arr[0xae]->desc = aapts("(var char_ptr , int item_type)");
    functions_arr[0xae]->cat_ref =  create_cat_ref(ITEM_CAT, 1, NULL, NULL);
    functions_arr[0x12e]->name = aapts("open_save_screen");
    functions_arr[0x12e]->params = 1;
    functions_arr[0x12e]->desc = aapts("(var unknown_int_ptr)");
    functions_arr[0x1d]->name = aapts("char_shoot_bow");
    functions_arr[0x1d]->params = 2;
    functions_arr[0x1d]->desc = aapts("(var char_ptr, var position_ptr)");
    functions_arr[0xa5]->name = aapts("run_wandering_generator");
    functions_arr[0xa5]->params = 2;
    functions_arr[0xa5]->desc = aapts("(var runtime_script_ptr , int time_delay)");
    add_forced_type(functions_arr[0xa5], ARRAY, 0, "wandering_gen_script");
    functions_arr[0xf6]->name = aapts("apply_effect_on_char");
    functions_arr[0xf6]->params = 4;
    functions_arr[0xf6]->desc = aapts("(var src_fx_ptr, int set, int num, var char_ptr)");
    functions_arr[0x66]->name = aapts("get_char_handle_id");
    functions_arr[0x66]->params = 1;
    functions_arr[0x66]->desc = aapts("(var char_ptr)");
    functions_arr[0x5e]->name = aapts("is_not_force_scrolling");
    functions_arr[0x5e]->params = 0;
    functions_arr[0x5e]->desc = aapts("()");
    functions_arr[0x30]->name = aapts("create_item");
    functions_arr[0x30]->params = 1;
    functions_arr[0x30]->desc = aapts("(int item_type)");
    functions_arr[0x30]->cat_ref =  create_cat_ref(ITEM_CAT, 0, NULL, NULL);
    functions_arr[0xb0]->name = aapts("get_char_item_if_exists");
    functions_arr[0xb0]->params = 2;
    functions_arr[0xb0]->desc = aapts("(var char_ptr, int item_type)");
    functions_arr[0xb0]->cat_ref =  create_cat_ref(ITEM_CAT, 1, NULL, NULL);
    functions_arr[0xb5]->name = aapts("remove_item_from_pie");
    functions_arr[0xb5]->params = 1;
    functions_arr[0xb5]->desc = aapts("(var item_ptr)");
    functions_arr[0x22]->name = aapts("set_char_walking");
    functions_arr[0x22]->params = 1;
    functions_arr[0x22]->desc = aapts("(var char_ptr)");
    functions_arr[0xa2]->name = aapts("play_loaded_sound");
    functions_arr[0xa2]->params = 4;
    functions_arr[0xa2]->desc = aapts("(int sound_num, int mode[0 -> once -1 -> repeat], int sound_set, var pos_ptr or int 0)");
    functions_arr[0xa2]->cat_ref =  create_cat_ref(SOUND_CAT, 0, NULL, "_sound_ref");
    functions_arr[0x6a]->name = aapts("get_char_movement_ptr");
    functions_arr[0x6a]->params = 1;
    functions_arr[0x6a]->desc = aapts("(var char_ptr)");
    functions_arr[0x67]->name = aapts("get_char_pos");
    functions_arr[0x67]->params = 1;
    functions_arr[0x67]->desc = aapts("(var char_ptr)");
    functions_arr[0x117]->name = aapts("set_scene_overwrite_act");
    functions_arr[0x117]->params = 3;
    functions_arr[0x117]->desc = aapts("(var zone_name, var room_name, int door)");
    functions_arr[0x11c]->name = aapts("set_sound_volume");
    functions_arr[0x11c]->params = 2;
    functions_arr[0x11c]->desc = aapts("(var sound_ptr, int volume)");
    functions_arr[0x125]->name = aapts("stop_playing_sound");
    functions_arr[0x125]->params = 1;
    functions_arr[0x125]->desc = aapts("(var sound_ptr)");
    functions_arr[0x77]->name = aapts("get_char_x");
    functions_arr[0x77]->params = 1;
    functions_arr[0x77]->desc = aapts("(var char_ptr)");
    functions_arr[0x78]->name = aapts("get_char_y");
    functions_arr[0x78]->params = 1;
    functions_arr[0x78]->desc = aapts("(var char_ptr)");
    functions_arr[0x79]->name = aapts("get_char_z");
    functions_arr[0x79]->params = 1;
    functions_arr[0x79]->desc = aapts("(var char_ptr)");
    functions_arr[0x10a]->name = aapts("set_char_lvl");
    functions_arr[0x10a]->params = 2;
    functions_arr[0x10a]->desc = aapts("(var char_ptr, int lvl)");
    functions_arr[0xd3]->name = aapts("randomize_int");
    functions_arr[0xd3]->params = 2;
    functions_arr[0xd3]->desc = aapts("(int from, int to)");
    functions_arr[0xc7]->name = aapts("char_attack");
    functions_arr[0xc7]->params = 2;
    functions_arr[0xc7]->desc = aapts("(var src_char, var dest_char)");
    functions_arr[0xcd]->name = aapts("log_string");
    functions_arr[0xcd]->params = 1;
    functions_arr[0xcd]->desc = aapts("(string some_string)");
    add_forced_type(functions_arr[0xcd], STRING, 0, NULL);
    functions_arr[0xce]->name = aapts("log_var");
    functions_arr[0xce]->params = 1;
    functions_arr[0xce]->desc = aapts("(var some_var)");
    functions_arr[0xf4]->name = aapts("play_3d_fx");
    functions_arr[0xf4]->params = 4;
    functions_arr[0xf4]->desc = aapts("(var fx_name, var pos_ptr1, var pos_ptr2 or int 0, int duration)");
    add_forced_type(functions_arr[0xf4], ARRAY, 1, "fx_position");
    functions_arr[0x4f]->name = aapts("set_char_init_state");
    functions_arr[0x4f]->params = 1;
    functions_arr[0x4f]->desc = aapts("(int init_state)");
    functions_arr[0x9a]->name = aapts("put_item_in_position");
    functions_arr[0x9a]->params = 4;
    functions_arr[0x9a]->desc = aapts("(var position_ptr, int item_id, int flag1, int flag2)");
    functions_arr[0x11f]->name = aapts("set_input_sys_2_if_pos_occupied");
    functions_arr[0x11f]->params = 2;
    functions_arr[0x11f]->desc = aapts("(var position_ptr, int boolean_val)");
    functions_arr[0x11e]->name = aapts("remove_effect");
    functions_arr[0x11e]->params = 1;
    functions_arr[0x11e]->desc = aapts("(var effect_ptr)");
    functions_arr[0x7e]->name = aapts("get_next_char_from_list");
    functions_arr[0x7e]->params = 0;
    functions_arr[0x7e]->desc = aapts("()");
    functions_arr[0x58]->name = aapts("stop_bgm");
    functions_arr[0x58]->params = 1;
    functions_arr[0x58]->desc = aapts("(int unknown)");
    functions_arr[0xe8]->name = aapts("play_tele_effect_at_pos");
    functions_arr[0xe8]->params = 2;
    functions_arr[0xe8]->desc = aapts("(var position_ptr, int num)");
    functions_arr[0x61]->name = aapts("char_remove_magic_and_weapons");
    functions_arr[0x61]->params = 1;
    functions_arr[0x61]->desc = aapts("(var char_ptr)");
    functions_arr[0x111]->name = aapts("set_input_system");
    functions_arr[0x111]->params = 1;
    functions_arr[0x111]->desc = aapts("(int type)");
    functions_arr[0x16]->name = aapts("is_char_handle_in_level");
    functions_arr[0x16]->params = 1;
    functions_arr[0x16]->desc = aapts("(int char_handle_id)");
    functions_arr[0x16]->cat_ref = create_cat_ref(HANDLE_CAT, 0, "is_", "_exist");
    functions_arr[0x107]->name = aapts("play_char_ai_script");
    functions_arr[0x107]->params = 2;
    functions_arr[0x107]->desc = aapts("(var char_ptr, var anim_table_ptr)");
    add_forced_type(functions_arr[0x107], ARRAY, 1, "ai_script");
    functions_arr[0x20]->name = aapts("set_char_running");
    functions_arr[0x20]->params = 1;
    functions_arr[0x20]->desc = aapts("(var char_ptr)");
    functions_arr[0xd5]->name = aapts("reset_local_timer");
    functions_arr[0xd5]->params = 0;
    functions_arr[0xd5]->desc = aapts("()");
    functions_arr[0xdf]->name = aapts("resume_conversation");
    functions_arr[0xdf]->params = 0;
    functions_arr[0xdf]->desc = aapts("()");
    functions_arr[0x129]->name = aapts("is_local_timer_greater_than");
    functions_arr[0x129]->params = 1;
    functions_arr[0x129]->desc = aapts("(int time)");
    functions_arr[0x12a]->name = aapts("is_local_timer_between");
    functions_arr[0x12a]->params = 2;
    functions_arr[0x12a]->desc = aapts("(int min_time, int max_time)");
    functions_arr[0x35]->name = aapts("char_turn_to_pos");
    functions_arr[0x35]->params = 2;
    functions_arr[0x35]->desc = aapts("(var char_ptr, var pos_ptr)");
    add_forced_type(functions_arr[0x35], ARRAY, 1, "position");
    functions_arr[0xe3]->name = aapts("chars_face_opposite");
    functions_arr[0xe3]->params = 2;
    functions_arr[0xe3]->desc = aapts("(var char1_ptr, var char2_ptr)");
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
