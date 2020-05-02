#pragma once
#ifndef def_h
#define def_h

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024
#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 500

#define MAX_FRAME_RATE 150

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_TILEMAP	6
#define SCENE_SECTION_START		7
#define SCENE_SECTION_MAP		8
#define SCENE_SECTION_OBJECTS	9

#define MAX_SCENE_LINE 1024

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_CANDLE	2
#define OBJECT_TYPE_GATE	3
#define OBJECT_TYPE_STAIR	4
//define time
#define DEFAULT_TIME_PLAY 300
//define map
#define MAP_1	1
#define MAP_2_1	2
//define animation_set
#define	SIMON_ANIMATION_SET 1
#define GROUND_ANIMATION_SET 2
#define CANDLE_ANIMATION_SET 3
#define WHIP_ANIMATION_SET 4
#define ITEMS_ANIMATION_SET 5
#define SW_ANIMATION_SET 6
#define STAIR_ANIMATION_SET 7
//define simon
#define SIMON_WALKING_SPEED		0.13f
#define SIMON_JUMP_SPEED		0.4f 

#define SIMON_GRAVITY			0.0011f

#define SIMON_BBOX_WIDTH		32
#define SIMON_BBOX_HEIGHT		62

#define TIME_HENSHIN 450
#define TIME_DELAY 300

#define SIMON_IDLE				0
#define SIMON_WALKING			1
#define SIMON_JUMP			    2
#define SIMON_SITDOWN			3
#define SIMON_ATK				4
#define SIMON_SIT_ATK			5
#define SIMON_HENSHIN			6
#define SIMON_STAIRDOWN			7
#define SIMON_STAIRUP			8
#define SIMON_STAIRDOWN_ATK		9
#define SIMON_STAIRUP_ATK		10
//define whip
#define TIME_FRAME 100
#define NORMAL_WHIP	0
#define SHORT_WHIP	1
#define LONG_WHIP	2
#define WHIP_BBOX_HEIGHT 15
#define WHIP_BBOX_WIDTH 55
#define LONG_CHAIN_BBOX_WIDTH 85
//define subweapons
#define WEAPONS_STOP_WATCH	4
#define DAGGER 0
#define WEAPONS_AXE	1
#define WEAPONS_HOLY_WATER	3
#define WEAPONS_HOLY_WATER_SHATTERED	5
#define WEAPONS_BOOMERANG	2
#define WEAPONS_DAGGER_SPEED 0.3
#define WEAPONS_AXE_SPEED_X 0.2
#define WEAPONS_AXE_SPEED_Y 0.45
#define WEAPONS_AXE_GRAVITY 0.001
#define WEAPONS_HOLY_WATER_SPEED_X 0.25
#define WEAPONS_HOLY_WATER_SPEED_Y 0.1
#define WEAPONS_HOLY_WATER_GRAVITY 0.001
#define TIME_HOLY_WATER_SHATTERED 1000
#define WEAPONS_BOOMERANG_SPEED 0.5
#define WEAPONS_BOOMERANG_TURNBACK_SPEED 0.01
//define items
#define ITEM_TIME_DESTROYED	4000
#define ITEM_FALLING_SPEED_Y 0.15f
#define SMALL_HEART	0
#define LARGE_HEART 1
#define MONEY_BAG_RED	2
#define MONEY_BAG_BLUE	3
#define MONEY_BAG_WHITE	4
#define PORK_CHOP	5
#define CHAIN 6
#define STOP_WATCH	7
#define ITEM_DAGGER 8
#define ITEM_AXE	9
#define ITEM_BOOMERANG	10
#define ITEM_HOLY_WATER	11
#define CROSS	12
#define INVISIBILITY_POTION	13
#define DOUBLE_SHOT 14
#define TRIPLE_SHOT	15
#define CROWN 16
#define MAGIC_CRYSTAL	17
//define candle
#define BIG_CANDLE 0
#define SMALL_CANDLE 1
#define CANDLE_DESTROYED 2
#define CANDLE_BBOX_WIDTH 32
#define CANDLE_BBOX_HEIGHT 64
#define SMALL_CANDLE_BBOX_WIDTH 16
#define SMALL_CANDLE_BBOX_HEIGHT 32
//define ground
#define MAP1BRICK 0
#define MAP2BRICK 1
#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 32
//define stair
#define MAP2Stair 0
#define MAP21Stair 1
#define STAIR_BBOX_WIDTH  32
#define STAIR_BBOX_HEIGHT 32
//define gate
#define GATE_BBOX_WIDTH		48
#define GATE_BBOX_HEIGHT	96

#endif