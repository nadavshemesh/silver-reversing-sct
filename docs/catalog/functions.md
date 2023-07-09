# Game Functions (Incomplete, 94/313)
### Description
The game function calls are the main ingredient in the scripts, thats how we actually use the game's inner logic.
         The important thing to remember is that these functions take arguments that are essential for the function to work properly.
         If one of the arguments is wrong the game will produce an unexpected result or most likely will just crash.

#

*Note: the information provided here can be very inaccurate or even wrong,         these are based on experiments and guesswork. If you notice a mistake, please contact me to correct it.*
| #ID | String ID | N.o Arguments | Arguments | References |
| --- | --- | --- | --- | --- |
| 6 | [add_item_to_char](./func_refs/6.md) | 4 | (var char_ptr, int item_type, int is_equiped, int is_dropped) | 563 |
| 7 | [save_pos_to_room_memory](./func_refs/7.md) | 4 | (int id, int x, int y, int z | 47 |
| 8 | [create_waypoint](./func_refs/8.md) | 3 | (int x, int y, int z) | 31 |
| 13 | [char_play_attack_anim](./func_refs/13.md) | 4 | (var char_ptr, int attack, var pos_ptr, int unknown) | 11 |
| 14 | [remove_face](./func_refs/14.md) | 1 | (var pos_ptr) | 25 |
| 15 | [set_char_type](./func_refs/15.md) | 2 | (var char_ptr, int char_type [hero, foe, neutral etc..]) | 38 |
| 16 | [add_waypoint_to_char](./func_refs/16.md) | 2 | (var char_ptr, int waypoint) | 35 |
| 22 | [is_char_handle_in_level](./func_refs/22.md) | 1 | (int char_handle_id) | 767 |
| 24 | [char_walk_to](./func_refs/24.md) | 4 | (var char_ptr, int x, int y, int z) | 243 |
| 26 | [char_walk_to_pos](./func_refs/26.md) | 2 | (var char_ptr, var position_ptr) | 318 |
| 28 | [char_shoot_char](./func_refs/28.md) | 2 | (var char1_ptr, var char2_ptr) | 2 |
| 29 | [char_shoot_bow](./func_refs/29.md) | 2 | (var char_ptr, var position_ptr) | 3 |
| 32 | [set_char_running](./func_refs/32.md) | 1 | (var char_ptr) | 377 |
| 34 | [set_char_walking](./func_refs/34.md) | 1 | (var char_ptr) | 83 |
| 35 | [char_approach_char_pos](./func_refs/35.md) | 2 | (var src_char_ptr, var dest_char_ptr, int radius) | 3 |
| 37 | [is_char_standing](./func_refs/37.md) | 1 | (var char_ptr) | 352 |
| 44 | [char_exit_door](./func_refs/44.md) | 2 | (var char_ptr, int door_number) | 22 |
| 48 | [create_item](./func_refs/48.md) | 1 | (int item_type) | 41 |
| 51 | [create_char_game_history_link](./func_refs/51.md) | 1 | (var char_ptr) | 3 |
| 53 | [char_turn_to_pos](./func_refs/53.md) | 2 | (var char_ptr, var pos_ptr) | 416 |
| 63 | [clear_char_waypoints](./func_refs/63.md) | 0 | () | 7 |
| 64 | [set_door_particle_effect](./func_refs/64.md) | 4 | (int door_num, int intensity, int type [0, 1, 2 or 3], int time_length[-1 for inf] | 15 |
| 67 | [kill_char](./func_refs/67.md) | 1 | (var char_ptr) | 9 |
| 72 | [freeze_enemy](./func_refs/72.md) | 1 | (var char_ptr) | 158 |
| 75 | [start_dialog](./func_refs/75.md) | 2 | (var start_dialog, var end_dialog) | 398 |
| 79 | [set_char_init_state](./func_refs/79.md) | 1 | (int init_state) | 262 |
| 81 | [set_char_template](./func_refs/81.md) | 2 | (var char_ptr, int char_type) | 78 |
| 88 | [stop_bgm](./func_refs/88.md) | 1 | (int unknown) | 60 |
| 94 | [is_not_force_scrolling](./func_refs/94.md) | 0 | () | 36 |
| 95 | [char_stop](./func_refs/95.md) | 1 | (var char_ptr) | 85 |
| 97 | [char_remove_magic_and_weapons](./func_refs/97.md) | 1 | (var char_ptr) | 33 |
| 98 | [alloc_space_in_general_heap](./func_refs/98.md) | 0 | () | 5 |
| 102 | [get_char_handle_id](./func_refs/102.md) | 1 | (var char_ptr) | 160 |
| 103 | [get_char_pos](./func_refs/103.md) | 1 | (var char_ptr) | 293 |
| 105 | [get_char_handle](./func_refs/105.md) | 1 | (int char_handle_id) | 317 |
| 106 | [get_char_movement_ptr](./func_refs/106.md) | 1 | (var char_ptr) | 47 |
| 110 | [load_next_dialog](./func_refs/110.md) | 2 | (var dialog_name_ptr_1, var dialog_name_ptr_2) | 283 |
| 112 | [get_david_char](./func_refs/112.md) | 0 | () | 205 |
| 114 | [get_selected_hero](./func_refs/114.md) | 0 | () | 77 |
| 118 | [get_input_sys](./func_refs/118.md) | 0 | () | 297 |
| 119 | [get_char_x](./func_refs/119.md) | 1 | (var char_ptr) | 3 |
| 120 | [get_char_y](./func_refs/120.md) | 1 | (var char_ptr) | 11 |
| 121 | [get_char_z](./func_refs/121.md) | 1 | (var char_ptr) | 1 |
| 126 | [get_next_char_from_list](./func_refs/126.md) | 0 | () | 240 |
| 129 | [get_num_of_enemies](./func_refs/129.md) | 0 | () | 170 |
| 147 | [create_char](./func_refs/147.md) | 4 | (var pos_ptr, string char_string_id, int face_direction_radius_counter_clockwise, int init_state) | 436 |
| 150 | [create_enemy_through_door](./func_refs/150.md) | 4 | (int enemy_type, int id, int door, int face_direction_radius_counter_clockwise) | 49 |
| 151 | [create_enemy](./func_refs/151.md) | 4 | (int enemy_type, int id, var position_ptr, int face_direction_radius_counter_clockwise) | 664 |
| 154 | [put_item_in_position](./func_refs/154.md) | 4 | (var position_ptr, int item_id, int flag1, int flag2) | 27 |
| 160 | [play_2d_anim](./func_refs/160.md) | 4 | (var anim_name_ptr, int on_repeat, unknown but always 1, var play_flag_ptr or 0 to always play) | 323 |
| 162 | [play_loaded_sound](./func_refs/162.md) | 4 | (int sound_num, int mode[0 -> once -1 -> repeat], int sound_set, var pos_ptr or int 0) | 1074 |
| 164 | [run_enemy_generator](./func_refs/164.md) | 2 | (var runtime_script_ptr , int time_delay) | 263 |
| 165 | [run_wandering_generator](./func_refs/165.md) | 2 | (var runtime_script_ptr , int time_delay) | 104 |
| 168 | [is_char_at_pos](./func_refs/168.md) | 3 | (var char_ptr, var pos_ptr, int radius) | 54 |
| 174 | [get_char_item_if_equiped](./func_refs/174.md) | 2 | (var char_ptr , int item_type) | 10 |
| 176 | [get_char_item_if_exists](./func_refs/176.md) | 2 | (var char_ptr, int item_type) | 3 |
| 178 | [remove_char](./func_refs/178.md) | 1 | (var char_ptr) | 183 |
| 181 | [remove_item_from_pie](./func_refs/181.md) | 1 | (var item_ptr) | 6 |
| 183 | [load_anim](./func_refs/183.md) | 1 | (var anim_name_ptr) | 238 |
| 184 | [load_item](./func_refs/184.md) | 1 | (int item_type) | 79 |
| 187 | [load_char_anim](./func_refs/187.md) | 2 | (var char_ptr, var anim_table) | 28 |
| 188 | [load_char](./func_refs/188.md) | 1 | (var char_name_ptr) | 522 |
| 189 | [load_sound_set](./func_refs/189.md) | 1 | (int sound_num) | 443 |
| 198 | [fly_to_pos](./func_refs/198.md) | 3 | (var char_ptr, int is_run, var pos_ptr) | 26 |
| 199 | [char_attack](./func_refs/199.md) | 2 | (var src_char, var dest_char) | 76 |
| 200 | [play_bgm](./func_refs/200.md) | 1 | (var name_ptr) | 180 |
| 205 | [log_string](./func_refs/205.md) | 1 | (string some_string) | 780 |
| 206 | [log_var](./func_refs/206.md) | 1 | (var some_var) | 171 |
| 211 | [randomize_int](./func_refs/211.md) | 2 | (int from, int to) | 854 |
| 213 | [reset_local_timer](./func_refs/213.md) | 0 | () | 303 |
| 216 | [load_pos_from_room_memory](./func_refs/216.md) | 2 | (int id, var unpack_memory_location | 24 |
| 223 | [resume_conversation](./func_refs/223.md) | 0 | () | 100 |
| 227 | [chars_face_opposite](./func_refs/227.md) | 2 | (var char1_ptr, var char2_ptr) | 18 |
| 229 | [toggle_or_use_char_item](./func_refs/229.md) | 2 | (var char_ptr , var item_ptr) | 14 |
| 232 | [play_tele_effect_at_pos](./func_refs/232.md) | 2 | (var position_ptr, int num) | 20 |
| 244 | [play_3d_fx](./func_refs/244.md) | 4 | (var fx_name, var pos_ptr1, var pos_ptr2 or int 0, int duration) | 99 |
| 246 | [apply_effect_on_char](./func_refs/246.md) | 4 | (var src_fx_ptr, int set, int num, var char_ptr) | 138 |
| 255 | [force_scroll_to_pos](./func_refs/255.md) | 1 | (var pos_ptr) | 57 |
| 263 | [play_char_ai_script](./func_refs/263.md) | 2 | (var char_ptr, var anim_table_ptr) | 302 |
| 266 | [set_char_lvl](./func_refs/266.md) | 2 | (var char_ptr, int lvl) | 30 |
| 267 | [char_teleport_to_pos](./func_refs/267.md) | 3 | (var char_ptr, var position_ptr, int unknown) | 62 |
| 269 | [load_dialog_by_number](./func_refs/269.md) | 3 | (var dialog_name_ptr_1, var dialog_name_ptr_2, int dialog number) | 284 |
| 273 | [set_input_system](./func_refs/273.md) | 1 | (int type) | 564 |
| 279 | [set_scene_overwrite_act](./func_refs/279.md) | 3 | (var zone_name, var room_name, int door) | 72 |
| 284 | [set_sound_volume](./func_refs/284.md) | 2 | (var sound_ptr, int volume) | 396 |
| 286 | [remove_effect](./func_refs/286.md) | 1 | (var effect_ptr) | 10 |
| 287 | [set_input_sys_2_if_pos_occupied](./func_refs/287.md) | 2 | (var position_ptr, int boolean_val) | 13 |
| 293 | [stop_playing_sound](./func_refs/293.md) | 1 | (var sound_ptr) | 33 |
| 297 | [is_local_timer_greater_than](./func_refs/297.md) | 1 | (int time) | 247 |
| 298 | [is_local_timer_between](./func_refs/298.md) | 2 | (int min_time, int max_time) | 2 |
| 300 | [play_char_anim](./func_refs/300.md) | 3 | (var char_ptr, var anim_name_ptr, int unknown) | 301 |
| 302 | [open_save_screen](./func_refs/302.md) | 1 | (var unknown_int_ptr) | 49 |
| 304 | [char_unfollow_char](./func_refs/304.md) | 2 | (var char_ptr) - to unfollow david or (var char1_ptr, var char2_ptr) | 11 |
| 305 | [char_follow_char](./func_refs/305.md) | 2 | (var char1_ptr, var char2_ptr) | 8 |

#

# Unnamed functions (yet to be identified and cataloged)
| #ID | String ID | N.o Arguments | Arguments | References |
| --- | --- | --- | --- | --- |
| 0 | [func_0](./func_refs/0.md) | - | - | 1 |
| 1 | [func_1](./func_refs/1.md) | - | - | 48 |
| 2 | [func_2](./func_refs/2.md) | - | - | 45 |
| 3 | [func_3](./func_refs/3.md) | - | - | 2 |
| 4 | [func_4](./func_refs/4.md) | - | - | 9 |
| 5 | [func_5](./func_refs/5.md) | - | - | 0 |
| 9 | [func_9](./func_refs/9.md) | - | - | 27 |
| 10 | [func_a](./func_refs/10.md) | - | - | 1 |
| 11 | [func_b](./func_refs/11.md) | - | - | 1 |
| 12 | [func_c](./func_refs/12.md) | - | - | 7 |
| 17 | [func_11](./func_refs/17.md) | - | - | 0 |
| 18 | [func_12](./func_refs/18.md) | - | - | 1 |
| 19 | [func_13](./func_refs/19.md) | - | - | 5 |
| 20 | [func_14](./func_refs/20.md) | - | - | 7 |
| 21 | [func_15](./func_refs/21.md) | - | - | 1 |
| 23 | [func_17](./func_refs/23.md) | - | - | 63 |
| 25 | [func_19](./func_refs/25.md) | - | - | 56 |
| 27 | [func_1b](./func_refs/27.md) | - | - | 0 |
| 30 | [func_1e](./func_refs/30.md) | - | - | 9 |
| 31 | [func_1f](./func_refs/31.md) | - | - | 1 |
| 33 | [func_21](./func_refs/33.md) | - | - | 2 |
| 36 | [func_24](./func_refs/36.md) | - | - | 1 |
| 38 | [func_26](./func_refs/38.md) | - | - | 1 |
| 39 | [func_27](./func_refs/39.md) | - | - | 3 |
| 40 | [func_28](./func_refs/40.md) | - | - | 2 |
| 41 | [func_29](./func_refs/41.md) | - | - | 12 |
| 42 | [func_2a](./func_refs/42.md) | - | - | 48 |
| 43 | [func_2b](./func_refs/43.md) | - | - | 81 |
| 45 | [func_2d](./func_refs/45.md) | - | - | 3 |
| 46 | [func_2e](./func_refs/46.md) | - | - | 1 |
| 47 | [func_2f](./func_refs/47.md) | - | - | 2 |
| 49 | [func_31](./func_refs/49.md) | - | - | 0 |
| 50 | [func_32](./func_refs/50.md) | - | - | 3 |
| 52 | [func_34](./func_refs/52.md) | - | - | 52 |
| 54 | [func_36](./func_refs/54.md) | - | - | 52 |
| 55 | [func_37](./func_refs/55.md) | - | - | 21 |
| 56 | [func_38](./func_refs/56.md) | - | - | 5 |
| 57 | [func_39](./func_refs/57.md) | - | - | 0 |
| 58 | [func_3a](./func_refs/58.md) | - | - | 3 |
| 59 | [func_3b](./func_refs/59.md) | - | - | 5 |
| 60 | [func_3c](./func_refs/60.md) | - | - | 20 |
| 61 | [func_3d](./func_refs/61.md) | - | - | 3 |
| 62 | [func_3e](./func_refs/62.md) | - | - | 1 |
| 65 | [func_41](./func_refs/65.md) | - | - | 22 |
| 66 | [func_42](./func_refs/66.md) | - | - | 72 |
| 68 | [func_44](./func_refs/68.md) | - | - | 1 |
| 69 | [func_45](./func_refs/69.md) | - | - | 5 |
| 70 | [func_46](./func_refs/70.md) | - | - | 109 |
| 71 | [func_47](./func_refs/71.md) | - | - | 41 |
| 73 | [func_49](./func_refs/73.md) | - | - | 3 |
| 74 | [func_4a](./func_refs/74.md) | - | - | 12 |
| 76 | [func_4c](./func_refs/76.md) | - | - | 1 |
| 77 | [func_4d](./func_refs/77.md) | - | - | 12 |
| 78 | [func_4e](./func_refs/78.md) | - | - | 21 |
| 80 | [func_50](./func_refs/80.md) | - | - | 381 |
| 82 | [func_52](./func_refs/82.md) | - | - | 0 |
| 83 | [func_53](./func_refs/83.md) | - | - | 18 |
| 84 | [func_54](./func_refs/84.md) | - | - | 5 |
| 85 | [func_55](./func_refs/85.md) | - | - | 29 |
| 86 | [func_56](./func_refs/86.md) | - | - | 2 |
| 87 | [func_57](./func_refs/87.md) | - | - | 0 |
| 89 | [func_59](./func_refs/89.md) | - | - | 8 |
| 90 | [func_5a](./func_refs/90.md) | - | - | 25 |
| 91 | [func_5b](./func_refs/91.md) | - | - | 0 |
| 92 | [func_5c](./func_refs/92.md) | - | - | 133 |
| 93 | [func_5d](./func_refs/93.md) | - | - | 5 |
| 96 | [func_60](./func_refs/96.md) | - | - | 4 |
| 99 | [func_63](./func_refs/99.md) | - | - | 10 |
| 100 | [func_64](./func_refs/100.md) | - | - | 1 |
| 101 | [func_65](./func_refs/101.md) | - | - | 17 |
| 104 | [func_68](./func_refs/104.md) | - | - | 7 |
| 107 | [func_6b](./func_refs/107.md) | - | - | 3 |
| 108 | [func_6c](./func_refs/108.md) | - | - | 4 |
| 109 | [func_6d](./func_refs/109.md) | - | - | 94 |
| 111 | [func_6f](./func_refs/111.md) | - | - | 42 |
| 113 | [func_71](./func_refs/113.md) | - | - | 7 |
| 115 | [func_73](./func_refs/115.md) | - | - | 4 |
| 116 | [func_74](./func_refs/116.md) | - | - | 1 |
| 117 | [func_75](./func_refs/117.md) | - | - | 1 |
| 122 | [func_7a](./func_refs/122.md) | - | - | 0 |
| 123 | [func_7b](./func_refs/123.md) | - | - | 1 |
| 124 | [func_7c](./func_refs/124.md) | - | - | 4 |
| 125 | [func_7d](./func_refs/125.md) | - | - | 7 |
| 127 | [func_7f](./func_refs/127.md) | - | - | 4 |
| 128 | [func_80](./func_refs/128.md) | - | - | 5 |
| 130 | [func_82](./func_refs/130.md) | - | - | 7 |
| 131 | [func_83](./func_refs/131.md) | - | - | 0 |
| 132 | [func_84](./func_refs/132.md) | - | - | 0 |
| 133 | [func_85](./func_refs/133.md) | - | - | 4 |
| 134 | [func_86](./func_refs/134.md) | - | - | 11 |
| 135 | [func_87](./func_refs/135.md) | - | - | 7 |
| 136 | [func_88](./func_refs/136.md) | - | - | 39 |
| 137 | [func_89](./func_refs/137.md) | - | - | 0 |
| 138 | [func_8a](./func_refs/138.md) | - | - | 87 |
| 139 | [func_8b](./func_refs/139.md) | - | - | 11 |
| 140 | [func_8c](./func_refs/140.md) | - | - | 8 |
| 141 | [func_8d](./func_refs/141.md) | - | - | 4 |
| 142 | [func_8e](./func_refs/142.md) | - | - | 1 |
| 143 | [func_8f](./func_refs/143.md) | - | - | 7 |
| 144 | [func_90](./func_refs/144.md) | - | - | 1 |
| 145 | [func_91](./func_refs/145.md) | - | - | 0 |
| 146 | [func_92](./func_refs/146.md) | - | - | 17 |
| 148 | [func_94](./func_refs/148.md) | - | - | 6 |
| 149 | [func_95](./func_refs/149.md) | - | - | 11 |
| 152 | [func_98](./func_refs/152.md) | - | - | 6 |
| 153 | [func_99](./func_refs/153.md) | - | - | 5 |
| 155 | [func_9b](./func_refs/155.md) | - | - | 16 |
| 156 | [func_9c](./func_refs/156.md) | - | - | 11 |
| 157 | [func_9d](./func_refs/157.md) | - | - | 1 |
| 158 | [func_9e](./func_refs/158.md) | - | - | 1 |
| 159 | [func_9f](./func_refs/159.md) | - | - | 9 |
| 161 | [func_a1](./func_refs/161.md) | - | - | 2 |
| 163 | [func_a3](./func_refs/163.md) | - | - | 1 |
| 166 | [func_a6](./func_refs/166.md) | - | - | 11 |
| 167 | [func_a7](./func_refs/167.md) | - | - | 1 |
| 169 | [func_a9](./func_refs/169.md) | - | - | 0 |
| 170 | [func_aa](./func_refs/170.md) | - | - | 40 |
| 171 | [func_ab](./func_refs/171.md) | - | - | 35 |
| 172 | [func_ac](./func_refs/172.md) | - | - | 19 |
| 173 | [func_ad](./func_refs/173.md) | - | - | 0 |
| 175 | [func_af](./func_refs/175.md) | - | - | 20 |
| 177 | [func_b1](./func_refs/177.md) | - | - | 2 |
| 179 | [func_b3](./func_refs/179.md) | - | - | 1 |
| 180 | [func_b4](./func_refs/180.md) | - | - | 5 |
| 182 | [func_b6](./func_refs/182.md) | - | - | 5 |
| 185 | [func_b9](./func_refs/185.md) | - | - | 1 |
| 186 | [func_ba](./func_refs/186.md) | - | - | 0 |
| 190 | [func_be](./func_refs/190.md) | - | - | 0 |
| 191 | [func_bf](./func_refs/191.md) | - | - | 45 |
| 192 | [func_c0](./func_refs/192.md) | - | - | 13 |
| 193 | [func_c1](./func_refs/193.md) | - | - | 2 |
| 194 | [func_c2](./func_refs/194.md) | - | - | 2 |
| 195 | [func_c3](./func_refs/195.md) | - | - | 1 |
| 196 | [func_c4](./func_refs/196.md) | - | - | 0 |
| 197 | [func_c5](./func_refs/197.md) | - | - | 9 |
| 201 | [func_c9](./func_refs/201.md) | - | - | 4 |
| 202 | [func_ca](./func_refs/202.md) | - | - | 3 |
| 203 | [func_cb](./func_refs/203.md) | - | - | 4 |
| 204 | [func_cc](./func_refs/204.md) | - | - | 2 |
| 207 | [func_cf](./func_refs/207.md) | - | - | 1 |
| 208 | [func_d0](./func_refs/208.md) | - | - | 7 |
| 209 | [func_d1](./func_refs/209.md) | - | - | 22 |
| 210 | [func_d2](./func_refs/210.md) | - | - | 9 |
| 212 | [func_d4](./func_refs/212.md) | - | - | 0 |
| 214 | [func_d6](./func_refs/214.md) | - | - | 18 |
| 215 | [func_d7](./func_refs/215.md) | - | - | 0 |
| 217 | [func_d9](./func_refs/217.md) | - | - | 4 |
| 218 | [func_da](./func_refs/218.md) | - | - | 2 |
| 219 | [func_db](./func_refs/219.md) | - | - | 20 |
| 220 | [func_dc](./func_refs/220.md) | - | - | 1 |
| 221 | [func_dd](./func_refs/221.md) | - | - | 1 |
| 222 | [func_de](./func_refs/222.md) | - | - | 1 |
| 224 | [func_e0](./func_refs/224.md) | - | - | 3 |
| 225 | [func_e1](./func_refs/225.md) | - | - | 13 |
| 226 | [func_e2](./func_refs/226.md) | - | - | 4 |
| 228 | [func_e4](./func_refs/228.md) | - | - | 0 |
| 230 | [func_e6](./func_refs/230.md) | - | - | 46 |
| 231 | [func_e7](./func_refs/231.md) | - | - | 2 |
| 233 | [func_e9](./func_refs/233.md) | - | - | 0 |
| 234 | [func_ea](./func_refs/234.md) | - | - | 1 |
| 235 | [func_eb](./func_refs/235.md) | - | - | 0 |
| 236 | [func_ec](./func_refs/236.md) | - | - | 56 |
| 237 | [func_ed](./func_refs/237.md) | - | - | 2 |
| 238 | [func_ee](./func_refs/238.md) | - | - | 3 |
| 239 | [func_ef](./func_refs/239.md) | - | - | 0 |
| 240 | [func_f0](./func_refs/240.md) | - | - | 1 |
| 241 | [func_f1](./func_refs/241.md) | - | - | 10 |
| 242 | [func_f2](./func_refs/242.md) | - | - | 0 |
| 243 | [func_f3](./func_refs/243.md) | - | - | 2 |
| 245 | [func_f5](./func_refs/245.md) | - | - | 1 |
| 247 | [func_f7](./func_refs/247.md) | - | - | 7 |
| 248 | [func_f8](./func_refs/248.md) | - | - | 6 |
| 249 | [func_f9](./func_refs/249.md) | - | - | 11 |
| 250 | [func_fa](./func_refs/250.md) | - | - | 24 |
| 251 | [func_fb](./func_refs/251.md) | - | - | 9 |
| 252 | [func_fc](./func_refs/252.md) | - | - | 6 |
| 253 | [func_fd](./func_refs/253.md) | - | - | 4 |
| 254 | [func_fe](./func_refs/254.md) | - | - | 7 |
| 256 | [func_100](./func_refs/256.md) | - | - | 16 |
| 257 | [func_101](./func_refs/257.md) | - | - | 1 |
| 258 | [func_102](./func_refs/258.md) | - | - | 9 |
| 259 | [func_103](./func_refs/259.md) | - | - | 11 |
| 260 | [func_104](./func_refs/260.md) | - | - | 39 |
| 261 | [func_105](./func_refs/261.md) | - | - | 0 |
| 262 | [func_106](./func_refs/262.md) | - | - | 493 |
| 264 | [func_108](./func_refs/264.md) | - | - | 71 |
| 265 | [func_109](./func_refs/265.md) | - | - | 10 |
| 268 | [func_10c](./func_refs/268.md) | - | - | 139 |
| 270 | [func_10e](./func_refs/270.md) | - | - | 3 |
| 271 | [func_10f](./func_refs/271.md) | - | - | 8 |
| 272 | [func_110](./func_refs/272.md) | - | - | 6 |
| 274 | [func_112](./func_refs/274.md) | - | - | 7 |
| 275 | [func_113](./func_refs/275.md) | - | - | 25 |
| 276 | [func_114](./func_refs/276.md) | - | - | 26 |
| 277 | [func_115](./func_refs/277.md) | - | - | 18 |
| 278 | [func_116](./func_refs/278.md) | - | - | 58 |
| 280 | [func_118](./func_refs/280.md) | - | - | 16 |
| 281 | [func_119](./func_refs/281.md) | - | - | 1 |
| 282 | [func_11a](./func_refs/282.md) | - | - | 46 |
| 283 | [func_11b](./func_refs/283.md) | - | - | 0 |
| 285 | [func_11d](./func_refs/285.md) | - | - | 47 |
| 288 | [func_120](./func_refs/288.md) | - | - | 3 |
| 289 | [func_121](./func_refs/289.md) | - | - | 0 |
| 290 | [func_122](./func_refs/290.md) | - | - | 59 |
| 291 | [func_123](./func_refs/291.md) | - | - | 5 |
| 292 | [func_124](./func_refs/292.md) | - | - | 4 |
| 294 | [func_126](./func_refs/294.md) | - | - | 4 |
| 295 | [func_127](./func_refs/295.md) | - | - | 81 |
| 296 | [func_128](./func_refs/296.md) | - | - | 1 |
| 299 | [func_12b](./func_refs/299.md) | - | - | 22 |
| 301 | [func_12d](./func_refs/301.md) | - | - | 1 |
| 303 | [func_12f](./func_refs/303.md) | - | - | 30 |
| 306 | [func_132](./func_refs/306.md) | - | - | 6 |
| 307 | [func_133](./func_refs/307.md) | - | - | 2 |
| 308 | [func_134](./func_refs/308.md) | - | - | 12 |
| 309 | [func_135](./func_refs/309.md) | - | - | 0 |
| 310 | [func_136](./func_refs/310.md) | - | - | 0 |
| 311 | [func_137](./func_refs/311.md) | - | - | 0 |
| 312 | [func_138](./func_refs/312.md) | - | - | 0 |
