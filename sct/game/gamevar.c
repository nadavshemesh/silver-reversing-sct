#include "sct\game\gamevar.h"

game_var* init_gv(int id, char* name, char* desc, int first_off, int second_off, int third_off, int len) {
    game_var* gv = w_malloc(sizeof(game_var));
    gv->name = w_malloc(strlen(name)*sizeof(char)+1);
    gv->desc = w_malloc(strlen(desc)*sizeof(char));

    gv->id = id;
    gv->first_offset = first_off;
    gv->second_offset = second_off;
    gv->third_offset = third_off;
    gv->len = len;

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
    int len = 4;

    return init_gv(id, name, desc, fo, so, to, len);
}

game_var* init_gv_room_state() {
    int id = 1;
    char name[] = "room_state";
    char desc[] = "used to determine state. saved after leaving room.";
    int fo = 0x04;
    int so = 0x09;
    int to = 0x08;
    int len = 4;

    return init_gv(id, name, desc, fo, so, to, len);
}

game_var* init_gv_room_timer() {
    int id = 2;
    char name[] = "room_timer";
    char desc[] = "used to get the time elapsed since entrance. reset when leaving room.";
    int fo = 0x02;
    int so = 0x00;
    int to = 0x5b;
    int len = 4;

    return init_gv(id, name, desc, fo, so, to, len);
}

void init_game_vars(game_var** game_vars) {
    game_vars[0] = init_gv(0, "script_trigger", "-", 0x02, 0x00, 0x02, 0x1c);
    game_vars[1] = init_gv(1, "CURRENT_LEVEL", "-", 0x02, 0x00, 0x00, 0x04);
    game_vars[2] = init_gv(2, "CURRENT_SCENE", "-", 0x02, 0x00, 0x01, 0x04);
    game_vars[3] = init_gv(3, "CURSOR_X", "-", 0x02, 0x00, 0x02, 0x04);
    game_vars[4] = init_gv(4, "CURSOR_Y", "-", 0x02, 0x00, 0x03, 0x04);
    game_vars[5] = init_gv(5, "MOUSE_X", "-", 0x02, 0x00, 0x04, 0x04);
    game_vars[6] = init_gv(6, "MOUSE_Y", "-", 0x02, 0x00, 0x05, 0x04);
    game_vars[7] = init_gv(7, "GLOBAL_ABILITY", "-", 0x02, 0x00, 0x06, 0x04);
    game_vars[8] = init_gv(8, "RENDER_CHARS", "-", 0x02, 0x00, 0x07, 0x04);
    game_vars[9] = init_gv(9, "RENDER_SPEED", "-", 0x02, 0x00, 0x08, 0x04);
    game_vars[10] = init_gv(10, "RENDER_FORCE", "-", 0x02, 0x00, 0x09, 0x04);
    game_vars[11] = init_gv(11, "RENDER_PATH", "-", 0x02, 0x00, 0x0A, 0x04);
    game_vars[12] = init_gv(12, "RENDER_CLICKFACE", "-", 0x02, 0x00, 0x0B, 0x04);
    game_vars[13] = init_gv(13, "ENEMY_FOLLOW_TARGETS", "-", 0x02, 0x00, 0x0C, 0x04);
    game_vars[14] = init_gv(14, "LEFT_HANDED_MOUSE", "-", 0x02, 0x00, 0x0D, 0x04);
    game_vars[15] = init_gv(15, "SOUND_EVENT_LIST", "-", 0x02, 0x00, 0x0E, 0x04);
    game_vars[16] = init_gv(16, "general_movement_anims_table_p", "-", 0x02, 0x00, 0x0F, 0x04);
    game_vars[17] = init_gv(17, "override_attack_anims_table", "-", 0x02, 0x00, 0x10, 0x04);
    game_vars[18] = init_gv(18, "override_movement_anims_table", "-", 0x02, 0x00, 0x11, 0x04);
    game_vars[19] = init_gv(19, "global_inventory_p", "-", 0x02, 0x00, 0x12, 0x04);
    game_vars[20] = init_gv(20, "COMMAND_COMPLETION", "-", 0x02, 0x00, 0x13, 0x04);
    game_vars[21] = init_gv(21, "PRE_CACHE", "-", 0x02, 0x00, 0x14, 0x04);
    game_vars[22] = init_gv(22, "alpha_map_data", "-", 0x02, 0x00, 0x15, 0x04);
    game_vars[23] = init_gv(23, "SMACKER_X", "-", 0x02, 0x00, 0x16, 0x04);
    game_vars[24] = init_gv(24, "SMACKER_Y", "-", 0x02, 0x00, 0x17, 0x04);
    game_vars[25] = init_gv(25, "global_selected_weapon", "-", 0x02, 0x00, 0x18, 0x04);
    game_vars[26] = init_gv(26, "global_behavior", "-", 0x02, 0x00, 0x19, 0x04);
    game_vars[27] = init_gv(27, "level_scene_names_pp", "-", 0x02, 0x00, 0x1A, 0x04);
    game_vars[28] = init_gv(28, "STATE_OVERRIDE", "-", 0x02, 0x00, 0x1B, 0x04);
    game_vars[29] = init_gv(29, "texture_line_data", "-", 0x02, 0x00, 0x1C, 0x04);
    game_vars[30] = init_gv(30, "scene_counter", "-", 0x02, 0x00, 0x1D, 0x04);
    game_vars[31] = init_gv(31, "INPUT_CONTROL_SYSTEM", "-", 0x02, 0x00, 0x1E, 0x04);
    game_vars[32] = init_gv(32, "CLICKED_DOOR", "-", 0x02, 0x00, 0x1F, 0x04);
    game_vars[33] = init_gv(33, "clicked_character", "-", 0x02, 0x00, 0x20, 0x04);
    game_vars[34] = init_gv(34, "handle_names_p", "-", 0x02, 0x00, 0x21, 0x04);
    game_vars[35] = init_gv(35, "item_table", "-", 0x02, 0x00, 0x22, 0x04);
    game_vars[36] = init_gv(36, "number_of_items_in_item_table", "-", 0x02, 0x00, 0x23, 0x04);
    game_vars[37] = init_gv(37, "horn_blow_enemy_min", "-", 0x02, 0x00, 0x24, 0x04);
    game_vars[38] = init_gv(38, "script_object", "-", 0x02, 0x00, 0x25, 0x04);
    game_vars[39] = init_gv(39, "tricode_names_pp", "-", 0x02, 0x00, 0x26, 0x04);
    game_vars[40] = init_gv(40, "global_zone_type", "-", 0x02, 0x00, 0x27, 0x04);
    game_vars[41] = init_gv(41, "room_monster_percent", "-", 0x02, 0x00, 0x28, 0x04);
    game_vars[42] = init_gv(42, "door_key_table_p", "-", 0x02, 0x00, 0x29, 0x04);
    game_vars[43] = init_gv(43, "door_restriction_p", "-", 0x02, 0x00, 0x2A, 0x04);
    game_vars[44] = init_gv(44, "global_selected_shield", "-", 0x02, 0x00, 0x2B, 0x04);
    game_vars[45] = init_gv(45, "DISABLE_AI", "-", 0x02, 0x00, 0x2C, 0x04);
    game_vars[46] = init_gv(46, "RENDER_SENSE", "-", 0x02, 0x00, 0x2D, 0x04);
    game_vars[47] = init_gv(47, "compile_init_character", "-", 0x02, 0x00, 0x2E, 0x04);
    game_vars[48] = init_gv(48, "scroll_x_shake", "-", 0x02, 0x00, 0x2F, 0x04);
    game_vars[49] = init_gv(49, "no_inventory", "-", 0x02, 0x00, 0x30, 0x04);
    game_vars[50] = init_gv(50, "universal_inventory_p", "-", 0x02, 0x00, 0x31, 0x04);
    game_vars[51] = init_gv(51, "ammo_list_p", "-", 0x02, 0x00, 0x32, 0x04);
    game_vars[52] = init_gv(52, "play_sting", "-", 0x02, 0x00, 0x33, 0x04);
    game_vars[53] = init_gv(53, "play_battle_music", "-", 0x02, 0x00, 0x34, 0x04);
    game_vars[54] = init_gv(54, "pnc_look_use_mask", "-", 0x02, 0x00, 0x35, 0x04);
    game_vars[55] = init_gv(55, "time", "-", 0x02, 0x00, 0x36, 0x04);
    game_vars[56] = init_gv(56, "disable_scroll", "-", 0x02, 0x00, 0x37, 0x04);
    game_vars[57] = init_gv(57, "area_fire_fx_scripts", "-", 0x02, 0x00, 0x38, 0x04);
    game_vars[58] = init_gv(58, "area_lightning_fx_scripts", "-", 0x02, 0x00, 0x39, 0x04);
    game_vars[59] = init_gv(59, "area_earth_fx_scripts", "-", 0x02, 0x00, 0x3A, 0x04);
    game_vars[60] = init_gv(60, "area_acid_fx_scripts", "-", 0x02, 0x00, 0x3B, 0x04);
    game_vars[61] = init_gv(61, "area_ice_fx_scripts", "-", 0x02, 0x00, 0x3C, 0x04);
    game_vars[62] = init_gv(62, "area_light_fx_scripts", "-", 0x02, 0x00, 0x3D, 0x04);
    game_vars[63] = init_gv(63, "extra_templates", "-", 0x02, 0x00, 0x3E, 0x04);
    game_vars[64] = init_gv(64, "magic_upgrade_data", "-", 0x02, 0x00, 0x3F, 0x04);
    game_vars[65] = init_gv(65, "char_gen_fx_scripts", "-", 0x02, 0x00, 0x40, 0x04);
    game_vars[66] = init_gv(66, "max_general_char_sound_events", "-", 0x02, 0x00, 0x41, 0x04);
    game_vars[67] = init_gv(67, "override_sound_event_list", "-", 0x02, 0x00, 0x42, 0x04);
    game_vars[68] = init_gv(68, "global_collision_type", "-", 0x02, 0x00, 0x43, 0x04);
    game_vars[69] = init_gv(69, "control_char_p", "-", 0x02, 0x00, 0x44, 0x04);
    game_vars[70] = init_gv(70, "attack_anims_table_pp", "-", 0x02, 0x00, 0x45, 0x04);
    game_vars[71] = init_gv(71, "character_templates_pp", "-", 0x02, 0x00, 0x46, 0x04);
    game_vars[72] = init_gv(72, "misc_fx_scripts", "-", 0x02, 0x00, 0x47, 0x04);
    game_vars[73] = init_gv(73, "damage_table_pp", "-", 0x02, 0x00, 0x48, 0x04);
    game_vars[74] = init_gv(74, "gib_override_table_pp", "-", 0x02, 0x00, 0x49, 0x04);
    game_vars[75] = init_gv(75, "global_exit_door", "-", 0x02, 0x00, 0x4A, 0x04);
    game_vars[76] = init_gv(76, "map_locations_p", "-", 0x02, 0x00, 0x4B, 0x04);
    game_vars[77] = init_gv(77, "collected_map_p", "-", 0x02, 0x00, 0x4C, 0x04);
    game_vars[78] = init_gv(78, "open_all_doors", "-", 0x02, 0x00, 0x4D, 0x04);
    game_vars[79] = init_gv(79, "char_override_names_pp", "-", 0x02, 0x00, 0x4E, 0x04);
    game_vars[80] = init_gv(80, "items_bought", "-", 0x02, 0x00, 0x4F, 0x04);
    game_vars[81] = init_gv(81, "potion_upgrade_data", "-", 0x02, 0x00, 0x50, 0x04);
    game_vars[82] = init_gv(82, "saved_games_count", "-", 0x02, 0x00, 0x51, 0x04);
    game_vars[83] = init_gv(83, "scroll_y_shake", "-", 0x02, 0x00, 0x52, 0x04);
    game_vars[84] = init_gv(84, "chronicler_variation", "-", 0x02, 0x00, 0x53, 0x04);
    game_vars[85] = init_gv(85, "MovieActive", "-", 0x02, 0x00, 0x54, 0x04);
    game_vars[86] = init_gv(86, "current_menu_level_p", "-", 0x02, 0x00, 0x55, 0x04);
    game_vars[87] = init_gv(87, "gPlayOutro", "-", 0x02, 0x00, 0x56, 0x04);
    game_vars[88] = init_gv(88, "valid_hero_list_p", "-", 0x02, 0x00, 0x57, 0x04);
    game_vars[89] = init_gv(89, "zone_under_mouse_type", "-", 0x02, 0x00, 0x58, 0x04);
    game_vars[90] = init_gv(90, "outro_active", "-", 0x02, 0x00, 0x59, 0x04);
    game_vars[91] = init_gv(91, "magic_orb_experience_p", "-", 0x02, 0x00, 0x5A, 0x04);
    game_vars[92] = init_gv(92, "script_time", "-", 0x02, 0x00, 0x5B, 0x04);
    game_vars[93] = init_gv(93, "accel_window_width", "-", 0x02, 0x00, 0x5C, 0x04);
    game_vars[94] = init_gv(94, "no_user_input", "-", 0x02, 0x00, 0x5D, 0x04);
    game_vars[95] = init_gv(95, "collected_arrows_p", "-", 0x02, 0x00, 0x5E, 0x04);
    game_vars[96] = init_gv(96, "collected_fire_arrows_p", "-", 0x02, 0x00, 0x5F, 0x04);
    game_vars[97] = init_gv(97, "left_handed", "-", 0x02, 0x00, 0x60, 0x04);
    game_vars[98] = init_gv(98, "use_mouse_override_sprite", "-", 0x02, 0x00, 0x61, 0x04);
    game_vars[99] = init_gv(99, "items_in_drag_box", "-", 0x02, 0x00, 0x62, 0x04);
    game_vars[100] = init_gv(100, "master", "-", 0x02, 0x00, 0x63, 0x04);
    game_vars[101] = init_gv(101, "changed_scene_via_map", "-", 0x02, 0x00, 0x64, 0x04);
    game_vars[102] = init_gv(102, "allocated_slot_adjustment", "-", 0x04, 0x65, 0x00, 0x04);
    game_vars[103] = init_gv(103, "character_stats_p", "-", 0x04, 0x03, 0x00, 0xDC);
    game_vars[104] = init_gv(104, "character_combat_p", "-", 0x04, 0x04, 0x00, 0x3C);
    game_vars[105] = init_gv(105, "character_movement_p", "-", 0x04, 0x05, 0x00, 0xFC);
    game_vars[106] = init_gv(106, "character_misc_p", "-", 0x04, 0x06, 0x00, 0x04);
    game_vars[107] = init_gv(107, "object_p", "-", 0x04, 0x07, 0x00, 0x20);
    game_vars[108] = init_gv(108, "level_info_p", "-", 0x04, 0x08, 0x00, 0x08);
    game_vars[109] = init_gv(109, "scene_info_p", "-", 0x04, 0x09, 0x00, 0x3C);
    game_vars[110] = init_gv(110, "character_ai_data_p", "-", 0x04, 0x0A, 0x00, 0x04);
    game_vars[111] = init_gv(111, "smack_object_p", "-", 0x04, 0x0B, 0x00, 0x20);
    game_vars[112] = init_gv(112, "psys_p", "-", 0x04, 0x0C, 0x00, 0x04);
    game_vars[113] = init_gv(113, "chr_body_p", "-", 0x04, 0x01, 0x00, 0x18);

    game_vars[114] = init_gv_room_state();
    game_vars[115] = init_gv_room_cleared();
}

game_var* get_game_var_by_offsets(int first_offset, int second_offset, int third_offset) {
    // printf("%02x %02x %02x\n", first_offset, second_offset, third_offset);
    first_offset &= 0x00FFFFFF; // ignore first byte (negation)
    if(game_vars == NULL) print_err_and_exit("Error, game_vars is undefined.", -5);
    for(int i=0; i < GAME_VARS_NUM; i++) {
        game_var* gv = game_vars[i];
        if(gv->first_offset == first_offset && gv->second_offset == second_offset && gv->third_offset == third_offset) {
            if(gv->first_offset == 0x04 && gv->len > 4) {
                char name[256];
                sprintf(name, "*%s+0", gv->name); // add star to pointer
                game_var* gv_mod = init_gv(gv->id, name, "-", first_offset, second_offset, third_offset, gv->len);
                return gv_mod;
            }
            return gv;
        } 
    }

    for(int i=0; i < GAME_VARS_NUM; i++) {
        game_var* gv = game_vars[i];
        if(gv->first_offset == 0x04 && gv->first_offset == first_offset && gv->second_offset == second_offset) {
            // in cases of 0x04 + offset without category
            char name[256];
            sprintf(name, "*%s+%d", gv->name, third_offset/4);
            game_var* gv_new = init_gv(0, name, "-", first_offset, second_offset, third_offset, gv->len);
            return gv_new;
        }
    }
    // fallback
    char name[256];
    sprintf(name, "unknown_%02x_%02x_%02x", first_offset, second_offset, third_offset);
    game_var* gv = init_gv(0, name, "-", first_offset, second_offset, third_offset, 4);
    return gv;

    // return NULL;
}

game_var* get_game_var_by_name(char* name) {
    char unknown_str[] = "unknown";
    if(game_vars == NULL) print_err_and_exit("Error, game_vars is undefined.", -5);
    for(int i=0; i < GAME_VARS_NUM; i++) {
        game_var* gv = game_vars[i];
        if(strlen(gv->name) == strlen(name) && strcmp(gv->name, name) == 0) {
            return gv;
        }
    }

    if(*name == '*') {
        char gvname[256];
        int to;
        sscanf(name, "*%[a-zA-Z\_]\+%d", gvname, &to);
        for(int i=0; i < GAME_VARS_NUM; i++) {
            game_var* gv = game_vars[i];
            if(strlen(gv->name) == strlen(gvname) && strcmp(gv->name, gvname) == 0) {
                game_var* gv_new = init_gv(0, gvname, "-", gv->first_offset, gv->second_offset, to*4, gv->len);
                return gv_new;
            }
        }
        char err[256];
        sprintf(err, "gamevar '%s' was not found.", gvname);
        print_err_and_exit(err, -4);
    }

    if(strlen(name) >= 7 && strncmp(name, unknown_str, strlen(unknown_str) == 0)) {
        int fo, so, to;
        sscanf(name, "unknown_%02x_%02x_%02x",&fo, &so, &to);
        game_var* gv = init_gv(0, name, "", fo, so, to, 4);
        return gv;
    }
    // return NULL;
}