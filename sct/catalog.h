#ifndef FILE_CAT_INCL
#define FILE_CAT_INCL

#include<stdio.h>
#include<stdlib.h>

#include "utils.h"

#define TOTAL_ENEMIES_NUM 216
#define TOTAL_ITEMS_NUM 115
#define TOTAL_SFX_NUM 528
#define TOTAL_CHARS_NUM 130
#define TOTAL_HANDLES_NUM 94

typedef enum catalog_type { ENEMY_CAT, ITEM_CAT, SOUND_CAT, CHAR_CAT, SFX_CAT, HANDLE_CAT, NO_CAT_USE_PARAM_STRING } cat_type;

typedef struct catalog {
    cat_type type;
    char** items;
    int* items_used;
} catalog;


extern catalog* enemy_cat;
extern catalog* handle_cat;
extern catalog* item_cat;
extern catalog* sound_cat;

void init_enemies_cat(catalog* enemy_cat);
void init_char_handle_cat(catalog* handle_cat);
void init_sound_cat(catalog* sound_cat);
void init_item_cat(catalog* item_cat);
int convert_handle_num_to_index(int handle_num);
void init_sfx_set_length();
int get_sound_index(int set, int number);

#endif