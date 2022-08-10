#ifndef FILE_CAT_INCL
#define FILE_CAT_INCL

#include<stdio.h>
#include<stdlib.h>

#include "utils.h"

#define TOTAL_ENEMIES_NUM 216
#define TOTAL_ITEMS_NUM 115
#define TOTAL_CHARS_NUM 130

typedef enum catalog_type { ENEMY_CAT, ITEM_CAT, SOUND_CAT, CHAR_CAT } cat_type;

typedef struct catalog {
    cat_type type;
    char** items;
} catalog;


extern catalog* enemy_cat;
void init_enemies_cat(catalog* enemy_cat);

#endif