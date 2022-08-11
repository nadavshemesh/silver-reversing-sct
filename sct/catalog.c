#include "sct\catalog.h"


void init_enemies_cat(catalog* enemy_cat) {
    char* items[TOTAL_ENEMIES_NUM];
    int items_used[TOTAL_ENEMIES_NUM] = { 0 };
    enemy_cat->type = ENEMY_CAT;

    items[0] = aapts("fireboss");
    items[1] = aapts("fire_sprite");
    items[2] = aapts("tree_sprite");
    items[3] = aapts("grow_tree_golem");
    items[4] = aapts("grow_tree_sprite");
    items[5] = aapts("grow_wamper");
    items[6] = aapts("wamper");
    items[7] = aapts("bear");
    items[8] = aapts("grunt");
    items[9] = aapts("devious_grunts");
    items[10] = aapts("heroic_grunt");
    items[11] = aapts("lieutenant");
    items[12] = aapts("devious_lieutenant");
    items[13] = aapts("heroic_lieutenant");
    items[14] = aapts("captain");
    items[15] = aapts("devious_captain");
    items[16] = aapts("heroic_captain");
    items[17] = aapts("archer");
    items[18] = aapts("hound");
    items[19] = aapts("devious_hound");
    items[20] = aapts("heroic_hound");
    items[21] = aapts("handler");
    items[22] = aapts("devious_handler");
    items[23] = aapts("heroic_handler");
    items[24] = aapts("grow_snow_sprite");
    items[25] = aapts("snow_sprite");
    items[26] = aapts("viking");
    items[27] = aapts("bee");
    items[28] = aapts("longjohn");
    items[29] = aapts("pirate_captain");
    items[30] = aapts("muscles");
    items[31] = aapts("dick");
    items[32] = aapts("finnegan");
    items[33] = aapts("grow_sguard");
    items[34] = aapts("sguard");
    items[35] = aapts("grow_sarcher");
    items[36] = aapts("sarcher");
    items[37] = aapts("grow_sgboss");
    items[38] = aapts("sgboss");
    items[39] = aapts("groad");
    items[40] = aapts("lizmant");
    items[41] = aapts("sprat");
    items[42] = aapts("teleport_maim");
    items[43] = aapts("grow_maim");
    items[44] = aapts("maim");
    items[45] = aapts("lesser_gimp");
    items[46] = aapts("greater_gimpv");
    items[47] = aapts("bat");
    items[48] = aapts("plagueman");
    items[49] = aapts("cyclops");
    items[50] = aapts("stag");
    items[51] = aapts("fishman");
    items[52] = aapts("webgrow_sguard");
    items[53] = aapts("werewolf_grow");
    items[54] = aapts("maim");
    items[55] = aapts("rat_raptor");
    items[56] = aapts("sentryg");
    items[57] = aapts("sentry_swordres");
    items[58] = aapts("skel");
    items[59] = aapts("deathip");
    items[60] = aapts("wraithp");
    items[61] = aapts("boneyfx");
    items[62] = aapts("npc_glass7");
    items[63] = aapts("bstump");
    items[64] = aapts("lobster");
    items[65] = aapts("vikingb");
    items[66] = aapts("vikingc");
    items[67] = aapts("scruttoc");
    items[68] = aapts("grow_skel");
    items[69] = aapts("grow_boney");
    items[70] = aapts("spider");
    items[71] = aapts("npc_skel");
    items[72] = aapts("leopard");
    items[73] = aapts("gonchfx");
    items[74] = aapts("ganitos");
    items[75] = aapts("fastmonk");
    items[76] = aapts("ogre");
    items[77] = aapts("ronnie");
    items[78] = aapts("orc");
    items[79] = aapts("ogre2");
    items[80] = aapts("ronnie2");
    items[81] = aapts("orc2");
    items[82] = aapts("mystmonk");
    items[83] = aapts("plague_boss");
    items[84] = aapts("fall_tree_spriterip");
    items[85] = aapts("fall_fastmonk");
    items[86] = aapts("rat_raptor_max3");
    items[87] = aapts("rat_raptor_max5");
    items[88] = aapts("drop_skel");
    items[89] = aapts("drop_boney");
    items[90] = aapts("drop_wraith");
    items[91] = aapts("fall_mystmonk");
    items[92] = aapts("spin_in_skel");
    items[93] = aapts("spin_in_fastmonk");
    items[94] = aapts("golem_grow");
    items[95] = aapts("plagueman_drops");
    items[96] = aapts("jennifer");
    items[97] = aapts("fuge");
    items[98] = aapts("grandad");
    items[99] = aapts("grunt_wooden_shield");
    items[100] = aapts("devious_imp");
    items[101] = aapts("cowardly_imp");
    items[102] = aapts("neutral_imp");
    items[103] = aapts("bezerk_imp");
    items[104] = aapts("catapult_imp");
    items[105] = aapts("flying_imps");
    items[106] = aapts("shaman_imp");
    items[107] = aapts("grow_neutral_imprip");
    items[108] = aapts("horn_impefs");
    items[109] = aapts("drop_bezerk_imp");
    items[110] = aapts("drop_swampy");
    items[111] = aapts("grow_swampy");
    items[112] = aapts("grow_big_swampy");
    items[113] = aapts("drop_lesser_gimp");
    items[114] = aapts("grow_devious_imp");
    items[115] = aapts("grow_apple");
    items[116] = aapts("ogre_jump");
    items[117] = aapts("ronnie_jump");
    items[118] = aapts("orc_jump");
    items[119] = aapts("grow_big_yellow_imp");
    items[120] = aapts("lizard_boss");
    items[121] = aapts("rend");
    items[122] = aapts("slieutenant");
    items[123] = aapts("devious_slieutenant");
    items[124] = aapts("heroic_slieutenantr");
    items[125] = aapts("scaptainpts");
    items[126] = aapts("devious_scaptain");
    items[127] = aapts("heroic_scaptain");
    items[128] = aapts("khan");
    items[129] = aapts("imp_boss");
    items[130] = aapts("swampy");
    items[131] = aapts("drop_greater_gimp");
    items[132] = aapts("drop_spider");
    items[133] = aapts("grow_cheese");
    items[134] = aapts("grow_golems");
    items[135] = aapts("resurrect_shaman");
    items[136] = aapts("treasure");
    items[137] = aapts("golem");
    items[138] = aapts("big_swampy");
    items[139] = aapts("plagueman_earth");
    items[140] = aapts("plagueman_times");
    items[141] = aapts("plagueman_3way");
    items[142] = aapts("plagueman_sword");
    items[143] = aapts("hellcat");
    items[144] = aapts("grow2_wamper");
    items[145] = aapts("viking");
    items[146] = aapts("ratgod");
    items[147] = aapts("drop_fishman");
    items[148] = aapts("fire_fishman");
    items[149] = aapts("web_fishman");
    items[150] = aapts("drop_fire_fishman");
    items[151] = aapts("draco_wyvern");
    items[152] = aapts("viking1");
    items[153] = aapts("vikingb1def");
    items[154] = aapts("beholder_red1");
    items[155] = aapts("beholder_purple");
    items[156] = aapts("start_grunt");
    items[157] = aapts("spires_captain");
    items[158] = aapts("spires_lieutenant");
    items[159] = aapts("silver");
    items[160] = aapts("tarrasque");
    items[161] = aapts("ratgod3");
    items[162] = aapts("flayer");
    items[163] = aapts("iwarrior_low");
    items[164] = aapts("iwarrior_med");
    items[165] = aapts("iwarrior_highs");
    items[166] = aapts("drop_iwarrior_low");
    items[167] = aapts("drop_iwarrior_med");
    items[168] = aapts("drop_iwarrior_high");
    items[169] = aapts("drop_flayer");
    items[170] = aapts("earth_flayer");
    items[171] = aapts("acid_flayer");
    items[172] = aapts("lightning_flayer");
    items[173] = aapts("drop_earth_flayer");
    items[174] = aapts("drop_acid_flayer");
    items[175] = aapts("drop_lightning_flayer");
    items[176] = aapts("flatnose");
    items[177] = aapts("flatnosr");
    items[178] = aapts("fade_chronicler");
    items[179] = aapts("wyverne");
    items[180] = aapts("ice4_grow_wamper_sc");
    items[181] = aapts("ice4_grow_bstumpfxd");
    items[182] = aapts("fade_greater_gimpg");
    items[183] = aapts("plagueman_catapult");
    items[184] = aapts("apple_fade_drop");
    items[185] = aapts("drop_rat_raptor");
    items[186] = aapts("triplick_lizman");
    items[187] = aapts("swamp_tarra");
    items[188] = aapts("npc_sentry");
    items[189] = aapts("grow_spider");
    items[190] = aapts("devious_little_lizmans");
    items[191] = aapts("cowardly_little_lizmang");
    items[192] = aapts("neutral_little_lizman");
    items[193] = aapts("bezerk_little_lizman");
    items[194] = aapts("grow_devious_little_lizman");
    items[195] = aapts("silv_orb");
    items[196] = aapts("fadein_mimic");
    items[197] = aapts("apocalypse");
    items[198] = aapts("drop_little_lizmans");
    items[199] = aapts("warden");
    items[200] = aapts("battle_fuge");
    items[201] = aapts("nemesis");
    items[202] = aapts("f_in_jennifer");
    items[203] = aapts("f_in_bethts");
    items[204] = aapts("f_in_bonnie");
    items[205] = aapts("f_in_sekune");
    items[206] = aapts("dead_queen");
    items[207] = aapts("fetch");
    items[208] = aapts("aqua_dragon");
    items[209] = aapts("captain_knives");
    items[210] = aapts("teleport_mini_maim");
    items[211] = aapts("mini_maimip");
    items[212] = aapts("moonmine");
    items[213] = aapts("npc_golem");
    items[214] = aapts("grow_boney_dont_drop_bombs");
    items[215] = aapts("fade_rat_raptor");

    enemy_cat->items = w_malloc(TOTAL_ENEMIES_NUM*sizeof(char*));
    memcpy(enemy_cat->items, items, TOTAL_ENEMIES_NUM*sizeof(char*));

    enemy_cat->items_used = w_malloc(TOTAL_ENEMIES_NUM*sizeof(int));
    memcpy(enemy_cat->items_used, items_used, TOTAL_ENEMIES_NUM*sizeof(int));
}

void init_char_handle_cat(catalog* handle_cat) {
    char* items[TOTAL_HANDLES_NUM];
    int items_used[TOTAL_HANDLES_NUM] = { 0 };
    handle_cat->type = HANDLE_CAT;

    items[0] = "VHATRED";
    items[1] = "BETH";
    items[2] = "FRINK";
    items[3] = "TERRY";
    items[4] = "MANNY";
    items[5] = "PEDRO";
    items[6] = "SGRUNT";
    items[7] = "WILLIAM";
    items[8] = "OLDWOMAN";
    items[9] = "CHRONICL";
    items[10] = "PROFESS";
    items[11] = "GOLEM";
    items[12] = "IMPNERD";
    items[13] = "IMP";
    items[14] = "IMPBOSS";
    items[15] = "WINGIMP";
    items[16] = "YELIMP";
    items[17] = "YIMPBOSS";
    items[18] = "TYRONIS";
    items[19] = "WHITTLE";
    items[20] = "MOONMINE";
    items[21] = "DAVID";
    items[22] = "GRANDAD";
    items[23] = "CAGEN";
    items[24] = "VIVIENNE";
    items[25] = "CHIARO";
    items[26] = "SILVER";
    items[27] = "BEARDMAN";
    items[28] = "BSTUMP";
    items[29] = "DUKE";
    items[30] = "FATBOB";
    items[31] = "FETCH";
    items[32] = "FUGE";
    items[33] = "GARGOYLE";
    items[34] = "GLASS";
    items[35] = "GROAD";
    items[36] = "GULL";
    items[37] = "JAG";
    items[38] = "JENNIFER";
    items[39] = "BUZUKI";
    items[40] = "CLAY";
    items[41] = "JANITOR";
    items[42] = "JUG";
    items[43] = "LOBSTER";
    items[44] = "MIMIC";
    items[45] = "MUSCLES";
    items[46] = "ORC";
    items[47] = "PIG";
    items[48] = "PIGPEA";
    items[49] = "REBEL";
    items[50] = "REBELA";
    items[51] = "RICHARD";
    items[52] = "SEKUNE";
    items[53] = "SKEL";
    items[54] = "SPRAT";
    items[55] = "STAG";
    items[56] = "SWAMPY";
    items[57] = "TAVMAN";
    items[58] = "TREESPRT";
    items[59] = "VILLAGER";
    items[60] = "VILLBOY";
    items[61] = "VILLHAT";
    items[62] = "OLDVILL";
    items[63] = "VILLOW";
    items[64] = "VILOWFAT";
    items[65] = "WEREWOLF";
    items[66] = "WYVERN";
    items[67] = "MAYOR";
    items[68] = "OLDBEARD";
    items[69] = "JOHN";
    items[70] = "BEN";
    items[71] = "ORACLE";
    items[72] = "MEDMONK";
    items[73] = "OTHIAS";
    items[74] = "PROF_VEL";
    items[75] = "GRUMPLE";
    items[76] = "FLATNOSE";
    items[77] = "DRUNKWIZ";
    items[78] = "CAPTAIN";
    items[79] = "BOATMAN";
    items[80] = "GONCH";
    items[81] = "RCAPTAIN";
    items[82] = "JEREMIAH";
    items[83] = "MOSS";
    items[84] = "SCRUTTOC";
    items[85] = "GANITOS";
    items[86] = "PRIEST";
    items[87] = "NEMESIS";
    items[88] = "STIPE";
    items[89] = "THADDEUS";
    items[90] = "KHAN";
    items[91] = "BTREASURE";
    items[92] = "SARCHER";
    items[93] = "HOUND";

    handle_cat->items = w_malloc(TOTAL_HANDLES_NUM*sizeof(char*));
    memcpy(handle_cat->items, items, TOTAL_HANDLES_NUM*sizeof(char*));

    handle_cat->items_used = w_malloc(TOTAL_HANDLES_NUM*sizeof(int));
    memcpy(handle_cat->items_used, items_used, TOTAL_HANDLES_NUM*sizeof(int));
}

int convert_handle_num_to_index(int handle_num) {
    int range = 10000;
    if(handle_num == 2293760 || (handle_num > 2293760-range && handle_num < 2293760+range)) return 0;
    if(handle_num == 6029312 || (handle_num > 6029312-range && handle_num < 6029312+range)) return 1;
    if(handle_num == 4390912 || (handle_num > 4390912-range && handle_num < 4390912+range)) return 2;
    if(handle_num == 4980736 || (handle_num > 4980736-range && handle_num < 4980736+range)) return 3;
    if(handle_num == 4915200 || (handle_num > 4915200-range && handle_num < 4915200+range)) return 4;
    if(handle_num == 5046272 || (handle_num > 5046272-range && handle_num < 5046272+range)) return 5;
    if(handle_num == 4128768 || (handle_num > 4128768-range && handle_num < 4128768+range)) return 6;
    if(handle_num == 4784128 || (handle_num > 4784128-range && handle_num < 4784128+range)) return 7;
    if(handle_num == 4259840 || (handle_num > 4259840-range && handle_num < 4259840+range)) return 8;
    if(handle_num == 6291456 || (handle_num > 6291456-range && handle_num < 6291456+range)) return 9;
    if(handle_num == 983040 || (handle_num > 983040-range && handle_num < 983040+range)) return 10;
    if(handle_num == 655360 || (handle_num > 655360-range && handle_num < 655360+range)) return 11;
    if(handle_num == 1900544 || (handle_num > 1900544-range && handle_num < 1900544+range)) return 12;
    if(handle_num == 917504 || (handle_num > 917504-range && handle_num < 917504+range)) return 13;
    if(handle_num == 4194304 || (handle_num > 4194304-range && handle_num < 4194304+range)) return 14;
    if(handle_num == 1376256 || (handle_num > 1376256-range && handle_num < 1376256+range)) return 15;
    if(handle_num == 1769472 || (handle_num > 1769472-range && handle_num < 1769472+range)) return 16;
    if(handle_num == 1703936 || (handle_num > 1703936-range && handle_num < 1703936+range)) return 17;
    if(handle_num == 2031616 || (handle_num > 2031616-range && handle_num < 2031616+range)) return 18;
    if(handle_num == 2359296 || (handle_num > 2359296-range && handle_num < 2359296+range)) return 19;
    if(handle_num == 1572864 || (handle_num > 1572864-range && handle_num < 1572864+range)) return 20;
    if(handle_num == 65536 || (handle_num > 65536-range && handle_num < 65536+range)) return 21;
    if(handle_num == 393216 || (handle_num > 393216-range && handle_num < 393216+range)) return 22;
    if(handle_num == 262144 || (handle_num > 262144-range && handle_num < 262144+range)) return 23;
    if(handle_num == 196608 || (handle_num > 196608-range && handle_num < 196608+range)) return 24;
    if(handle_num == 131072 || (handle_num > 131072-range && handle_num < 131072+range)) return 25;
    if(handle_num == 1245184 || (handle_num > 1245184-range && handle_num < 1245184+range)) return 26;
    if(handle_num == 2818048 || (handle_num > 2818048-range && handle_num < 2818048+range)) return 27;
    if(handle_num == 7929856 || (handle_num > 7929856-range && handle_num < 7929856+range)) return 28;
    if(handle_num == 4456448 || (handle_num > 4456448-range && handle_num < 4456448+range)) return 29;
    if(handle_num == 1638400 || (handle_num > 1638400-range && handle_num < 1638400+range)) return 30;
    if(handle_num == 8781824 || (handle_num > 8781824-range && handle_num < 8781824+range)) return 31;
    if(handle_num == 4063232 || (handle_num > 4063232-range && handle_num < 4063232+range)) return 32;
    if(handle_num == 3145728 || (handle_num > 3145728-range && handle_num < 3145728+range)) return 33;
    if(handle_num == 8257536 || (handle_num > 8257536-range && handle_num < 8257536+range)) return 34;
    if(handle_num == 8060928 || (handle_num > 8060928-range && handle_num < 8060928+range)) return 35;
    if(handle_num == 1966080 || (handle_num > 1966080-range && handle_num < 1966080+range)) return 36;
    if(handle_num == 3080192 || (handle_num > 3080192-range && handle_num < 3080192+range)) return 37;
    if(handle_num == 1310720 || (handle_num > 1310720-range && handle_num < 1310720+range)) return 38;
    if(handle_num == 4325376 || (handle_num > 4325376-range && handle_num < 4325376+range)) return 39;
    if(handle_num == 4718592 || (handle_num > 4718592-range && handle_num < 4718592+range)) return 40;
    if(handle_num == 5832704 || (handle_num > 5832704-range && handle_num < 5832704+range)) return 41;
    if(handle_num == 327680 || (handle_num > 327680-range && handle_num < 327680+range)) return 42;
    if(handle_num == 9109504 || (handle_num > 9109504-range && handle_num < 9109504+range)) return 43;
    if(handle_num == 1441792 || (handle_num > 1441792-range && handle_num < 1441792+range)) return 44;
    if(handle_num == 2490368 || (handle_num > 2490368-range && handle_num < 2490368+range)) return 45;
    if(handle_num == 7995392 || (handle_num > 7995392-range && handle_num < 7995392+range)) return 46;
    if(handle_num == 4849664 || (handle_num > 4849664-range && handle_num < 4849664+range)) return 47;
    if(handle_num == 720896 || (handle_num > 720896-range && handle_num < 720896+range)) return 48;
    if(handle_num == 1114112 || (handle_num > 1114112-range && handle_num < 1114112+range)) return 49;
    if(handle_num == 1048576 || (handle_num > 1048576-range && handle_num < 1048576+range)) return 50;
    if(handle_num == 4653056 || (handle_num > 4653056-range && handle_num < 4653056+range)) return 51;
    if(handle_num == 786432 || (handle_num > 786432-range && handle_num < 786432+range)) return 52;
    if(handle_num == 8126464 || (handle_num > 8126464-range && handle_num < 8126464+range)) return 53;
    if(handle_num == 9043968 || (handle_num > 9043968-range && handle_num < 9043968+range)) return 54;
    if(handle_num == 8912896 || (handle_num > 8912896-range && handle_num < 8912896+range)) return 55;
    if(handle_num == 7667712 || (handle_num > 7667712-range && handle_num < 7667712+range)) return 56;
    if(handle_num == 2555904 || (handle_num > 2555904-range && handle_num < 2555904+range)) return 57;
    if(handle_num == 9371648 || (handle_num > 9371648-range && handle_num < 9371648+range)) return 58;
    if(handle_num == 2097152 || (handle_num > 2097152-range && handle_num < 2097152+range)) return 59;
    if(handle_num == 2424832 || (handle_num > 2424832-range && handle_num < 2424832+range)) return 60;
    if(handle_num == 2228224 || (handle_num > 2228224-range && handle_num < 2228224+range)) return 61;
    if(handle_num == 3932160 || (handle_num > 3932160-range && handle_num < 3932160+range)) return 62;
    if(handle_num == 2686976 || (handle_num > 2686976-range && handle_num < 2686976+range)) return 63;
    if(handle_num == 2883584 || (handle_num > 2883584-range && handle_num < 2883584+range)) return 64;
    if(handle_num == 2621440 || (handle_num > 2621440-range && handle_num < 2621440+range)) return 65;
    if(handle_num == 851968 || (handle_num > 851968-range && handle_num < 851968+range)) return 66;
    if(handle_num == 8519680 || (handle_num > 8519680-range && handle_num < 8519680+range)) return 67;
    if(handle_num == 3997696 || (handle_num > 3997696-range && handle_num < 3997696+range)) return 68;
    if(handle_num == 11599872 || (handle_num > 11599872-range && handle_num < 11599872+range)) return 69;
    if(handle_num == 4587520 || (handle_num > 4587520-range && handle_num < 4587520+range)) return 70;
    if(handle_num == 10944512 || (handle_num > 10944512-range && handle_num < 10944512+range)) return 71;
    if(handle_num == 11468800 || (handle_num > 11468800-range && handle_num < 11468800+range)) return 72;
    if(handle_num == 9764864 || (handle_num > 9764864-range && handle_num < 9764864+range)) return 73;
    if(handle_num == 5963776 || (handle_num > 5963776-range && handle_num < 5963776+range)) return 74;
    if(handle_num == 3342336 || (handle_num > 3342336-range && handle_num < 3342336+range)) return 75;
    if(handle_num == 6815744 || (handle_num > 6815744-range && handle_num < 6815744+range)) return 76;
    if(handle_num == 9437184 || (handle_num > 9437184-range && handle_num < 9437184+range)) return 77;
    if(handle_num == 6356992 || (handle_num > 6356992-range && handle_num < 6356992+range)) return 78;
    if(handle_num == 10158080 || (handle_num > 10158080-range && handle_num < 10158080+range)) return 79;
    if(handle_num == 8847360 || (handle_num > 8847360-range && handle_num < 8847360+range)) return 80;
    if(handle_num == 6881280 || (handle_num > 6881280-range && handle_num < 6881280+range)) return 81;
    if(handle_num == 12255232 || (handle_num > 12255232-range && handle_num < 12255232+range)) return 82;
    if(handle_num == 10682368 || (handle_num > 10682368-range && handle_num < 10682368+range)) return 83;
    if(handle_num == 10813440 || (handle_num > 10813440-range && handle_num < 10813440+range)) return 84;
    if(handle_num == 11534336 || (handle_num > 11534336-range && handle_num < 11534336+range)) return 85;
    if(handle_num == 10616832 || (handle_num > 10616832-range && handle_num < 10616832+range)) return 86;
    if(handle_num == 11337728 || (handle_num > 11337728-range && handle_num < 11337728+range)) return 87;
    if(handle_num == 6946816 || (handle_num > 6946816-range && handle_num < 6946816+range)) return 88;
    if(handle_num == 11206656 || (handle_num > 11206656-range && handle_num < 11206656+range)) return 89;
    if(handle_num == 12058624 || (handle_num > 12058624-range && handle_num < 12058624+range)) return 90;
    if(handle_num == 1507337 || (handle_num > 1507337-range && handle_num < 1507337+range)) return 91;
    if(handle_num == 11010048 || (handle_num > 11010048-range && handle_num < 11010048+range)) return 92;
    if(handle_num == 7241760 || (handle_num > 7241760-range && handle_num < 7241760+range)) return 93;

    char err[256];
    sprintf(err, "Error, couldnt find char handle %ld.\n", handle_num);
    print_warn(err, -2);

    return -1;
}