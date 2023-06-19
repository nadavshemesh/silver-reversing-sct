# Game Functions (Incomplete, 91/313)
### Description
The game function calls are the main ingredient in the scripts, thats how we actually use the game's inner logic.
         The important thing to remember is that these functions take arguments that are essential for the function to work properly.
         If one of the arguments is wrong the game will produce an unexpected result or most likely will just crash.

#

*Note: the information provided here can be very inaccurate or even wrong,         these are based on experiments and guesswork. If you notice a mistake, please contact me to correct it.*
| #ID | String ID | N.o Arguments | Arguments | References |
| --- | --- | --- | --- | --- |
| 6 | [add_item_to_char](./func_refs/add_item_to_char.md) | 4 | (var char_ptr, int item_type, int is_equiped, int is_dropped) | 563 |
| 7 | [save_pos_to_room_memory](./func_refs/save_pos_to_room_memory.md) | 4 | (int id, int x, int y, int z | 47 |
| 8 | [create_waypoint](./func_refs/create_waypoint.md) | 3 | (int x, int y, int z) | 31 |
| 13 | [char_play_attack_anim](./func_refs/char_play_attack_anim.md) | 4 | (var char_ptr, int attack, var pos_ptr, int unknown) | 11 |
| 14 | [remove_face](./func_refs/remove_face.md) | 1 | (var pos_ptr) | 25 |
| 16 | [add_waypoint_to_char](./func_refs/add_waypoint_to_char.md) | 2 | (var char_ptr, int waypoint) | 35 |
| 22 | [is_char_handle_in_level](./func_refs/is_char_handle_in_level.md) | 1 | (int char_handle_id) | 767 |
| 24 | [char_walk_to](./func_refs/char_walk_to.md) | 4 | (var char_ptr, int x, int y, int z) | 243 |
| 26 | [char_walk_to_pos](./func_refs/char_walk_to_pos.md) | 2 | (var char_ptr, var position_ptr) | 318 |
| 29 | [char_shoot_bow](./func_refs/char_shoot_bow.md) | 2 | (var char_ptr, var position_ptr) | 3 |
| 32 | [set_char_running](./func_refs/set_char_running.md) | 1 | (var char_ptr) | 377 |
| 34 | [set_char_walking](./func_refs/set_char_walking.md) | 1 | (var char_ptr) | 83 |
| 35 | [char_approach_char_pos](./func_refs/char_approach_char_pos.md) | 2 | (var src_char_ptr, var dest_char_ptr, int radius) | 3 |
| 37 | [is_char_standing](./func_refs/is_char_standing.md) | 1 | (var char_ptr) | 352 |
| 44 | [char_exit_door](./func_refs/char_exit_door.md) | 2 | (var char_ptr, int door_number) | 22 |
| 48 | [create_item](./func_refs/create_item.md) | 1 | (int item_type) | 41 |
| 51 | [create_char_game_history_link](./func_refs/create_char_game_history_link.md) | 1 | (var char_ptr) | 3 |
| 53 | [char_turn_to_pos](./func_refs/char_turn_to_pos.md) | 2 | (var char_ptr, var pos_ptr) | 416 |
| 63 | [clear_char_waypoints](./func_refs/clear_char_waypoints.md) | 0 | () | 7 |
| 67 | [kill_char](./func_refs/kill_char.md) | 1 | (var char_ptr) | 9 |
| 72 | [freeze_enemy](./func_refs/freeze_enemy.md) | 1 | (var char_ptr) | 158 |
| 75 | [start_dialog](./func_refs/start_dialog.md) | 2 | (var start_dialog, var end_dialog) | 398 |
| 79 | [set_char_init_state](./func_refs/set_char_init_state.md) | 1 | (int init_state) | 262 |
| 81 | [set_char_template](./func_refs/set_char_template.md) | 2 | (var char_ptr, int char_type) | 78 |
| 88 | [stop_bgm](./func_refs/stop_bgm.md) | 1 | (int unknown) | 60 |
| 94 | [is_not_force_scrolling](./func_refs/is_not_force_scrolling.md) | 0 | () | 36 |
| 95 | [char_stop](./func_refs/char_stop.md) | 1 | (var char_ptr) | 85 |
| 97 | [char_remove_magic_and_weapons](./func_refs/char_remove_magic_and_weapons.md) | 1 | (var char_ptr) | 33 |
| 98 | [alloc_space_in_general_heap](./func_refs/alloc_space_in_general_heap.md) | 0 | () | 5 |
| 102 | [get_char_handle_id](./func_refs/get_char_handle_id.md) | 1 | (var char_ptr) | 160 |
| 103 | [get_char_pos](./func_refs/get_char_pos.md) | 1 | (var char_ptr) | 293 |
| 105 | [get_char_handle](./func_refs/get_char_handle.md) | 1 | (int char_handle_id) | 317 |
| 106 | [get_char_movement_ptr](./func_refs/get_char_movement_ptr.md) | 1 | (var char_ptr) | 47 |
| 110 | [load_next_dialog](./func_refs/load_next_dialog.md) | 2 | (var dialog_name_ptr_1, var dialog_name_ptr_2) | 283 |
| 112 | [get_david_char](./func_refs/get_david_char.md) | 0 | () | 205 |
| 114 | [get_selected_hero](./func_refs/get_selected_hero.md) | 0 | () | 77 |
| 118 | [get_input_sys](./func_refs/get_input_sys.md) | 0 | () | 297 |
| 119 | [get_char_x](./func_refs/get_char_x.md) | 1 | (var char_ptr) | 3 |
| 120 | [get_char_y](./func_refs/get_char_y.md) | 1 | (var char_ptr) | 11 |
| 121 | [get_char_z](./func_refs/get_char_z.md) | 1 | (var char_ptr) | 1 |
| 126 | [get_next_char_from_list](./func_refs/get_next_char_from_list.md) | 0 | () | 240 |
| 129 | [get_num_of_enemies](./func_refs/get_num_of_enemies.md) | 0 | () | 170 |
| 147 | [create_char](./func_refs/create_char.md) | 4 | (var pos_ptr, var char_name_ptr, int face_direction_radius_counter_clockwise, int init_state) | 436 |
| 150 | [create_enemy_through_door](./func_refs/create_enemy_through_door.md) | 4 | (int enemy_type, int id, int door, int face_direction_radius_counter_clockwise) | 49 |
| 151 | [create_enemy](./func_refs/create_enemy.md) | 4 | (int enemy_type, int id, var position_ptr, int face_direction_radius_counter_clockwise) | 664 |
| 154 | [put_item_in_position](./func_refs/put_item_in_position.md) | 4 | (var position_ptr, int item_id, int flag1, int flag2) | 27 |
| 160 | [play_2d_anim](./func_refs/play_2d_anim.md) | 4 | (var anim_name_ptr, int on_repeat, unknown but always 1, var play_flag_ptr or 0 to always play) | 323 |
| 162 | [play_loaded_sound](./func_refs/play_loaded_sound.md) | 4 | (int sound_num, int mode[0 -> once -1 -> repeat], int sound_set, var pos_ptr or int 0) | 1074 |
| 164 | [run_enemy_generator](./func_refs/run_enemy_generator.md) | 2 | (var runtime_script_ptr , int time_delay) | 263 |
| 165 | [run_wandering_generator](./func_refs/run_wandering_generator.md) | 2 | (var runtime_script_ptr , int time_delay) | 104 |
| 168 | [is_char_at_pos](./func_refs/is_char_at_pos.md) | 3 | (var char_ptr, var pos_ptr, int radius) | 54 |
| 174 | [get_char_item_if_equiped](./func_refs/get_char_item_if_equiped.md) | 2 | (var char_ptr , int item_type) | 10 |
| 176 | [get_char_item_if_exists](./func_refs/get_char_item_if_exists.md) | 2 | (var char_ptr, int item_type) | 3 |
| 178 | [remove_char](./func_refs/remove_char.md) | 1 | (var char_ptr) | 183 |
| 181 | [remove_item_from_pie](./func_refs/remove_item_from_pie.md) | 1 | (var item_ptr) | 6 |
| 183 | [load_anim](./func_refs/load_anim.md) | 1 | (var anim_name_ptr) | 238 |
| 184 | [load_item](./func_refs/load_item.md) | 1 | (int item_type) | 79 |
| 187 | [load_char_anim](./func_refs/load_char_anim.md) | 2 | (var char_ptr, var anim_table) | 28 |
| 188 | [load_char](./func_refs/load_char.md) | 1 | (var char_name_ptr) | 522 |
| 189 | [load_sound_set](./func_refs/load_sound_set.md) | 1 | (int sound_num) | 443 |
| 198 | [fly_to_pos](./func_refs/fly_to_pos.md) | 3 | (var char_ptr, int is_run, var pos_ptr) | 26 |
| 199 | [char_attack](./func_refs/char_attack.md) | 2 | (var src_char, var dest_char) | 76 |
| 200 | [play_bgm](./func_refs/play_bgm.md) | 1 | (var name_ptr) | 180 |
| 205 | [log](./func_refs/log.md) | 1 | (var string_ptr) | 1461 |
| 206 | [log_obj](./func_refs/log_obj.md) | 1 | (var obj_ptr) | 171 |
| 211 | [randomize_int](./func_refs/randomize_int.md) | 2 | (int from, int to) | 854 |
| 213 | [reset_local_timer](./func_refs/reset_local_timer.md) | 0 | () | 303 |
| 216 | [load_pos_from_room_memory](./func_refs/load_pos_from_room_memory.md) | 2 | (int id, var unpack_memory_location | 24 |
| 223 | [resume_conversation](./func_refs/resume_conversation.md) | 0 | () | 100 |
| 227 | [chars_face_opposite](./func_refs/chars_face_opposite.md) | 2 | (var char1_ptr, var char2_ptr) | 18 |
| 229 | [toggle_or_use_char_item](./func_refs/toggle_or_use_char_item.md) | 2 | (var char_ptr , var item_ptr) | 14 |
| 232 | [play_tele_effect_at_pos](./func_refs/play_tele_effect_at_pos.md) | 2 | (var position_ptr, int num) | 20 |
| 244 | [play_3d_fx](./func_refs/play_3d_fx.md) | 4 | (var fx_name, var pos_ptr1, var pos_ptr2 or int 0, int duration) | 99 |
| 246 | [apply_effect_on_char](./func_refs/apply_effect_on_char.md) | 4 | (var src_fx_ptr, int set, int num, var char_ptr) | 138 |
| 255 | [force_scroll_to_pos](./func_refs/force_scroll_to_pos.md) | 1 | (var pos_ptr) | 57 |
| 263 | [play_char_ai_script](./func_refs/play_char_ai_script.md) | 2 | (var char_ptr, var anim_table_ptr) | 302 |
| 266 | [set_char_lvl](./func_refs/set_char_lvl.md) | 2 | (var char_ptr, int lvl) | 30 |
| 267 | [char_teleport_to_pos](./func_refs/char_teleport_to_pos.md) | 3 | (var char_ptr, var position_ptr, int unknown) | 62 |
| 269 | [load_dialog_by_number](./func_refs/load_dialog_by_number.md) | 3 | (var dialog_name_ptr_1, var dialog_name_ptr_2, int dialog number) | 284 |
| 273 | [set_input_system](./func_refs/set_input_system.md) | 1 | (int type) | 564 |
| 279 | [set_scene_overwrite_act](./func_refs/set_scene_overwrite_act.md) | 3 | (var zone_name, var room_name, int door) | 72 |
| 284 | [set_sound_volume](./func_refs/set_sound_volume.md) | 2 | (var sound_ptr, int volume) | 396 |
| 286 | [remove_effect](./func_refs/remove_effect.md) | 1 | (var effect_ptr) | 10 |
| 287 | [set_input_sys_2_if_pos_occupied](./func_refs/set_input_sys_2_if_pos_occupied.md) | 2 | (var position_ptr, int boolean_val) | 13 |
| 293 | [stop_playing_sound](./func_refs/stop_playing_sound.md) | 1 | (var sound_ptr) | 33 |
| 297 | [is_local_timer_greater_than](./func_refs/is_local_timer_greater_than.md) | 1 | (int time) | 247 |
| 298 | [is_local_timer_between](./func_refs/is_local_timer_between.md) | 2 | (int min_time, int max_time) | 2 |
| 300 | [play_char_anim](./func_refs/play_char_anim.md) | 3 | (var char_ptr, var anim_name_ptr, int unknown) | 301 |
| 302 | [open_save_screen](./func_refs/open_save_screen.md) | 1 | (var unknown_int_ptr) | 49 |
| 304 | [char_unfollow_char](./func_refs/char_unfollow_char.md) | 1 | (var char1_ptr, var char2_ptr) | 11 |
| 305 | [char_follow_char](./func_refs/char_follow_char.md) | 2 | (var char1_ptr, var char2_ptr) | 8 |

#

# Unnamed functions (yet to be identified and cataloged)
| #ID | String ID | N.o Arguments | Arguments | References |
| --- | --- | --- | --- | --- |
| 0 | [func_0](./func_refs/func_0.md) | - | - | 1 |
| 1 | [func_1](./func_refs/func_1.md) | - | - | 48 |
| 2 | [func_2](./func_refs/func_2.md) | - | - | 45 |
| 3 | [func_3](./func_refs/func_3.md) | - | - | 2 |
| 4 | [func_4](./func_refs/func_4.md) | - | - | 9 |
| 5 | [func_5](./func_refs/func_5.md) | - | - | 0 |
| 9 | [func_9](./func_refs/func_9.md) | - | - | 27 |
| 10 | [func_a](./func_refs/func_a.md) | - | - | 1 |
| 11 | [func_b](./func_refs/func_b.md) | - | - | 1 |
| 12 | [func_c](./func_refs/func_c.md) | - | - | 7 |
| 15 | [func_f](./func_refs/func_f.md) | - | - | 38 |
| 17 | [func_11](./func_refs/func_11.md) | - | - | 0 |
| 18 | [func_12](./func_refs/func_12.md) | - | - | 1 |
| 19 | [func_13](./func_refs/func_13.md) | - | - | 5 |
| 20 | [func_14](./func_refs/func_14.md) | - | - | 7 |
| 21 | [func_15](./func_refs/func_15.md) | - | - | 1 |
| 23 | [func_17](./func_refs/func_17.md) | - | - | 63 |
| 25 | [func_19](./func_refs/func_19.md) | - | - | 56 |
| 27 | [func_1b](./func_refs/func_1b.md) | - | - | 0 |
| 28 | [func_1c](./func_refs/func_1c.md) | - | - | 2 |
| 30 | [func_1e](./func_refs/func_1e.md) | - | - | 9 |
| 31 | [func_1f](./func_refs/func_1f.md) | - | - | 1 |
| 33 | [func_21](./func_refs/func_21.md) | - | - | 2 |
| 36 | [func_24](./func_refs/func_24.md) | - | - | 1 |
| 38 | [func_26](./func_refs/func_26.md) | - | - | 1 |
| 39 | [func_27](./func_refs/func_27.md) | - | - | 3 |
| 40 | [func_28](./func_refs/func_28.md) | - | - | 2 |
| 41 | [func_29](./func_refs/func_29.md) | - | - | 12 |
| 42 | [func_2a](./func_refs/func_2a.md) | - | - | 48 |
| 43 | [func_2b](./func_refs/func_2b.md) | - | - | 81 |
| 45 | [func_2d](./func_refs/func_2d.md) | - | - | 3 |
| 46 | [func_2e](./func_refs/func_2e.md) | - | - | 1 |
| 47 | [func_2f](./func_refs/func_2f.md) | - | - | 2 |
| 49 | [func_31](./func_refs/func_31.md) | - | - | 0 |
| 50 | [func_32](./func_refs/func_32.md) | - | - | 3 |
| 52 | [func_34](./func_refs/func_34.md) | - | - | 52 |
| 54 | [func_36](./func_refs/func_36.md) | - | - | 52 |
| 55 | [func_37](./func_refs/func_37.md) | - | - | 21 |
| 56 | [func_38](./func_refs/func_38.md) | - | - | 5 |
| 57 | [func_39](./func_refs/func_39.md) | - | - | 0 |
| 58 | [func_3a](./func_refs/func_3a.md) | - | - | 3 |
| 59 | [func_3b](./func_refs/func_3b.md) | - | - | 5 |
| 60 | [func_3c](./func_refs/func_3c.md) | - | - | 20 |
| 61 | [func_3d](./func_refs/func_3d.md) | - | - | 3 |
| 62 | [func_3e](./func_refs/func_3e.md) | - | - | 1 |
| 64 | [func_40](./func_refs/func_40.md) | - | - | 15 |
| 65 | [func_41](./func_refs/func_41.md) | - | - | 22 |
| 66 | [func_42](./func_refs/func_42.md) | - | - | 72 |
| 68 | [func_44](./func_refs/func_44.md) | - | - | 1 |
| 69 | [func_45](./func_refs/func_45.md) | - | - | 5 |
| 70 | [func_46](./func_refs/func_46.md) | - | - | 109 |
| 71 | [func_47](./func_refs/func_47.md) | - | - | 41 |
| 73 | [func_49](./func_refs/func_49.md) | - | - | 3 |
| 74 | [func_4a](./func_refs/func_4a.md) | - | - | 12 |
| 76 | [func_4c](./func_refs/func_4c.md) | - | - | 1 |
| 77 | [func_4d](./func_refs/func_4d.md) | - | - | 12 |
| 78 | [func_4e](./func_refs/func_4e.md) | - | - | 21 |
| 80 | [func_50](./func_refs/func_50.md) | - | - | 381 |
| 82 | [func_52](./func_refs/func_52.md) | - | - | 0 |
| 83 | [func_53](./func_refs/func_53.md) | - | - | 18 |
| 84 | [func_54](./func_refs/func_54.md) | - | - | 5 |
| 85 | [func_55](./func_refs/func_55.md) | - | - | 29 |
| 86 | [func_56](./func_refs/func_56.md) | - | - | 2 |
| 87 | [func_57](./func_refs/func_57.md) | - | - | 0 |
| 89 | [func_59](./func_refs/func_59.md) | - | - | 8 |
| 90 | [func_5a](./func_refs/func_5a.md) | - | - | 25 |
| 91 | [func_5b](./func_refs/func_5b.md) | - | - | 0 |
| 92 | [func_5c](./func_refs/func_5c.md) | - | - | 133 |
| 93 | [func_5d](./func_refs/func_5d.md) | - | - | 5 |
| 96 | [func_60](./func_refs/func_60.md) | - | - | 4 |
| 99 | [func_63](./func_refs/func_63.md) | - | - | 10 |
| 100 | [func_64](./func_refs/func_64.md) | - | - | 1 |
| 101 | [func_65](./func_refs/func_65.md) | - | - | 17 |
| 104 | [func_68](./func_refs/func_68.md) | - | - | 7 |
| 107 | [func_6b](./func_refs/func_6b.md) | - | - | 3 |
| 108 | [func_6c](./func_refs/func_6c.md) | - | - | 4 |
| 109 | [func_6d](./func_refs/func_6d.md) | - | - | 94 |
| 111 | [func_6f](./func_refs/func_6f.md) | - | - | 42 |
| 113 | [func_71](./func_refs/func_71.md) | - | - | 7 |
| 115 | [func_73](./func_refs/func_73.md) | - | - | 4 |
| 116 | [func_74](./func_refs/func_74.md) | - | - | 1 |
| 117 | [func_75](./func_refs/func_75.md) | - | - | 1 |
| 122 | [func_7a](./func_refs/func_7a.md) | - | - | 0 |
| 123 | [func_7b](./func_refs/func_7b.md) | - | - | 1 |
| 124 | [func_7c](./func_refs/func_7c.md) | - | - | 4 |
| 125 | [func_7d](./func_refs/func_7d.md) | - | - | 7 |
| 127 | [func_7f](./func_refs/func_7f.md) | - | - | 4 |
| 128 | [func_80](./func_refs/func_80.md) | - | - | 5 |
| 130 | [func_82](./func_refs/func_82.md) | - | - | 7 |
| 131 | [func_83](./func_refs/func_83.md) | - | - | 0 |
| 132 | [func_84](./func_refs/func_84.md) | - | - | 0 |
| 133 | [func_85](./func_refs/func_85.md) | - | - | 4 |
| 134 | [func_86](./func_refs/func_86.md) | - | - | 11 |
| 135 | [func_87](./func_refs/func_87.md) | - | - | 7 |
| 136 | [func_88](./func_refs/func_88.md) | - | - | 39 |
| 137 | [func_89](./func_refs/func_89.md) | - | - | 0 |
| 138 | [func_8a](./func_refs/func_8a.md) | - | - | 87 |
| 139 | [func_8b](./func_refs/func_8b.md) | - | - | 11 |
| 140 | [func_8c](./func_refs/func_8c.md) | - | - | 8 |
| 141 | [func_8d](./func_refs/func_8d.md) | - | - | 4 |
| 142 | [func_8e](./func_refs/func_8e.md) | - | - | 1 |
| 143 | [func_8f](./func_refs/func_8f.md) | - | - | 7 |
| 144 | [func_90](./func_refs/func_90.md) | - | - | 1 |
| 145 | [func_91](./func_refs/func_91.md) | - | - | 0 |
| 146 | [func_92](./func_refs/func_92.md) | - | - | 17 |
| 148 | [func_94](./func_refs/func_94.md) | - | - | 6 |
| 149 | [func_95](./func_refs/func_95.md) | - | - | 11 |
| 152 | [func_98](./func_refs/func_98.md) | - | - | 6 |
| 153 | [func_99](./func_refs/func_99.md) | - | - | 5 |
| 155 | [func_9b](./func_refs/func_9b.md) | - | - | 16 |
| 156 | [func_9c](./func_refs/func_9c.md) | - | - | 11 |
| 157 | [func_9d](./func_refs/func_9d.md) | - | - | 1 |
| 158 | [func_9e](./func_refs/func_9e.md) | - | - | 1 |
| 159 | [func_9f](./func_refs/func_9f.md) | - | - | 9 |
| 161 | [func_a1](./func_refs/func_a1.md) | - | - | 2 |
| 163 | [func_a3](./func_refs/func_a3.md) | - | - | 1 |
| 166 | [func_a6](./func_refs/func_a6.md) | - | - | 11 |
| 167 | [func_a7](./func_refs/func_a7.md) | - | - | 1 |
| 169 | [func_a9](./func_refs/func_a9.md) | - | - | 0 |
| 170 | [func_aa](./func_refs/func_aa.md) | - | - | 40 |
| 171 | [func_ab](./func_refs/func_ab.md) | - | - | 35 |
| 172 | [func_ac](./func_refs/func_ac.md) | - | - | 19 |
| 173 | [func_ad](./func_refs/func_ad.md) | - | - | 0 |
| 175 | [func_af](./func_refs/func_af.md) | - | - | 20 |
| 177 | [func_b1](./func_refs/func_b1.md) | - | - | 2 |
| 179 | [func_b3](./func_refs/func_b3.md) | - | - | 1 |
| 180 | [func_b4](./func_refs/func_b4.md) | - | - | 5 |
| 182 | [func_b6](./func_refs/func_b6.md) | - | - | 5 |
| 185 | [func_b9](./func_refs/func_b9.md) | - | - | 1 |
| 186 | [func_ba](./func_refs/func_ba.md) | - | - | 0 |
| 190 | [func_be](./func_refs/func_be.md) | - | - | 0 |
| 191 | [func_bf](./func_refs/func_bf.md) | - | - | 45 |
| 192 | [func_c0](./func_refs/func_c0.md) | - | - | 13 |
| 193 | [func_c1](./func_refs/func_c1.md) | - | - | 2 |
| 194 | [func_c2](./func_refs/func_c2.md) | - | - | 2 |
| 195 | [func_c3](./func_refs/func_c3.md) | - | - | 1 |
| 196 | [func_c4](./func_refs/func_c4.md) | - | - | 0 |
| 197 | [func_c5](./func_refs/func_c5.md) | - | - | 9 |
| 201 | [func_c9](./func_refs/func_c9.md) | - | - | 4 |
| 202 | [func_ca](./func_refs/func_ca.md) | - | - | 3 |
| 203 | [func_cb](./func_refs/func_cb.md) | - | - | 4 |
| 204 | [func_cc](./func_refs/func_cc.md) | - | - | 2 |
| 207 | [func_cf](./func_refs/func_cf.md) | - | - | 1 |
| 208 | [func_d0](./func_refs/func_d0.md) | - | - | 7 |
| 209 | [func_d1](./func_refs/func_d1.md) | - | - | 22 |
| 210 | [func_d2](./func_refs/func_d2.md) | - | - | 9 |
| 212 | [func_d4](./func_refs/func_d4.md) | - | - | 0 |
| 214 | [func_d6](./func_refs/func_d6.md) | - | - | 18 |
| 215 | [func_d7](./func_refs/func_d7.md) | - | - | 0 |
| 217 | [func_d9](./func_refs/func_d9.md) | - | - | 4 |
| 218 | [func_da](./func_refs/func_da.md) | - | - | 2 |
| 219 | [func_db](./func_refs/func_db.md) | - | - | 20 |
| 220 | [func_dc](./func_refs/func_dc.md) | - | - | 1 |
| 221 | [func_dd](./func_refs/func_dd.md) | - | - | 1 |
| 222 | [func_de](./func_refs/func_de.md) | - | - | 1 |
| 224 | [func_e0](./func_refs/func_e0.md) | - | - | 3 |
| 225 | [func_e1](./func_refs/func_e1.md) | - | - | 13 |
| 226 | [func_e2](./func_refs/func_e2.md) | - | - | 4 |
| 228 | [func_e4](./func_refs/func_e4.md) | - | - | 0 |
| 230 | [func_e6](./func_refs/func_e6.md) | - | - | 46 |
| 231 | [func_e7](./func_refs/func_e7.md) | - | - | 2 |
| 233 | [func_e9](./func_refs/func_e9.md) | - | - | 0 |
| 234 | [func_ea](./func_refs/func_ea.md) | - | - | 1 |
| 235 | [func_eb](./func_refs/func_eb.md) | - | - | 0 |
| 236 | [func_ec](./func_refs/func_ec.md) | - | - | 56 |
| 237 | [func_ed](./func_refs/func_ed.md) | - | - | 2 |
| 238 | [func_ee](./func_refs/func_ee.md) | - | - | 3 |
| 239 | [func_ef](./func_refs/func_ef.md) | - | - | 0 |
| 240 | [func_f0](./func_refs/func_f0.md) | - | - | 1 |
| 241 | [func_f1](./func_refs/func_f1.md) | - | - | 10 |
| 242 | [func_f2](./func_refs/func_f2.md) | - | - | 0 |
| 243 | [func_f3](./func_refs/func_f3.md) | - | - | 2 |
| 245 | [func_f5](./func_refs/func_f5.md) | - | - | 1 |
| 247 | [func_f7](./func_refs/func_f7.md) | - | - | 7 |
| 248 | [func_f8](./func_refs/func_f8.md) | - | - | 6 |
| 249 | [func_f9](./func_refs/func_f9.md) | - | - | 11 |
| 250 | [func_fa](./func_refs/func_fa.md) | - | - | 24 |
| 251 | [func_fb](./func_refs/func_fb.md) | - | - | 9 |
| 252 | [func_fc](./func_refs/func_fc.md) | - | - | 6 |
| 253 | [func_fd](./func_refs/func_fd.md) | - | - | 4 |
| 254 | [func_fe](./func_refs/func_fe.md) | - | - | 7 |
| 256 | [func_100](./func_refs/func_100.md) | - | - | 16 |
| 257 | [func_101](./func_refs/func_101.md) | - | - | 1 |
| 258 | [func_102](./func_refs/func_102.md) | - | - | 9 |
| 259 | [func_103](./func_refs/func_103.md) | - | - | 11 |
| 260 | [func_104](./func_refs/func_104.md) | - | - | 39 |
| 261 | [func_105](./func_refs/func_105.md) | - | - | 0 |
| 262 | [func_106](./func_refs/func_106.md) | - | - | 493 |
| 264 | [func_108](./func_refs/func_108.md) | - | - | 71 |
| 265 | [func_109](./func_refs/func_109.md) | - | - | 10 |
| 268 | [func_10c](./func_refs/func_10c.md) | - | - | 139 |
| 270 | [func_10e](./func_refs/func_10e.md) | - | - | 3 |
| 271 | [func_10f](./func_refs/func_10f.md) | - | - | 8 |
| 272 | [func_110](./func_refs/func_110.md) | - | - | 6 |
| 274 | [func_112](./func_refs/func_112.md) | - | - | 7 |
| 275 | [func_113](./func_refs/func_113.md) | - | - | 25 |
| 276 | [func_114](./func_refs/func_114.md) | - | - | 26 |
| 277 | [func_115](./func_refs/func_115.md) | - | - | 18 |
| 278 | [func_116](./func_refs/func_116.md) | - | - | 58 |
| 280 | [func_118](./func_refs/func_118.md) | - | - | 16 |
| 281 | [func_119](./func_refs/func_119.md) | - | - | 1 |
| 282 | [func_11a](./func_refs/func_11a.md) | - | - | 46 |
| 283 | [func_11b](./func_refs/func_11b.md) | - | - | 0 |
| 285 | [func_11d](./func_refs/func_11d.md) | - | - | 47 |
| 288 | [func_120](./func_refs/func_120.md) | - | - | 3 |
| 289 | [func_121](./func_refs/func_121.md) | - | - | 0 |
| 290 | [func_122](./func_refs/func_122.md) | - | - | 59 |
| 291 | [func_123](./func_refs/func_123.md) | - | - | 5 |
| 292 | [func_124](./func_refs/func_124.md) | - | - | 4 |
| 294 | [func_126](./func_refs/func_126.md) | - | - | 4 |
| 295 | [func_127](./func_refs/func_127.md) | - | - | 81 |
| 296 | [func_128](./func_refs/func_128.md) | - | - | 1 |
| 299 | [func_12b](./func_refs/func_12b.md) | - | - | 22 |
| 301 | [func_12d](./func_refs/func_12d.md) | - | - | 1 |
| 303 | [func_12f](./func_refs/func_12f.md) | - | - | 30 |
| 306 | [func_132](./func_refs/func_132.md) | - | - | 6 |
| 307 | [func_133](./func_refs/func_133.md) | - | - | 2 |
| 308 | [func_134](./func_refs/func_134.md) | - | - | 12 |
| 309 | [func_135](./func_refs/func_135.md) | - | - | 0 |
| 310 | [func_136](./func_refs/func_136.md) | - | - | 0 |
| 311 | [func_137](./func_refs/func_137.md) | - | - | 0 |
| 312 | [func_138](./func_refs/func_138.md) | - | - | 0 |
