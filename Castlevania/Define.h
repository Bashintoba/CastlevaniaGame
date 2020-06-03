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

#define MAX_FRAME_RATE 120

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

#define GRID_CELL_WIDTH 264
#define GRID_CELL_HEIGHT 250

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_TILEMAP	6
#define SCENE_SECTION_START		7
#define SCENE_SECTION_MAP		8
#define SCENE_SECTION_OBJECTS	9
#define SCENE_SECTION_CLEARMAP	10
#define SCENE_SECTION_CLEARTEXT	11
#define SCENE_SECTION_CLEARSP	12
#define SCENE_SECTION_CLEARANI	13
#define SCENE_SECTION_CLEARANISET	14

#define MAX_SCENE_LINE 1024

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_CANDLE	2
#define OBJECT_TYPE_GATE	3
#define OBJECT_TYPE_STAIR	4
#define OBJECT_TYPE_BREAKBRICK 5
#define OBJECT_TYPE_MOVINGPLATFORM 6
#define OBJECT_TYPE_KNIGHT 7
#define OBJECT_TYPE_DARKENBAT	8
#define OBJECT_TYPE_GHOST	9
#define OBJECT_TYPE_MONKEY	10
#define OBJECT_TYPE_SKELETON	11
#define OBJECT_TYPE_RAVEN	12
#define OBJECT_TYPE_ZOMBIE	13
//define time
#define DEFAULT_TIME_PLAY 300
//define animation_set
#define	SIMON_ANIMATION_SET 1
#define GROUND_ANIMATION_SET 2
#define CANDLE_ANIMATION_SET 3
#define WHIP_ANIMATION_SET 4
#define ITEMS_ANIMATION_SET 5
#define SW_ANIMATION_SET 6
#define STAIR_ANIMATION_SET 7
//define HUD
#define NUM0	100000
#define NUM1	100001
#define NUM2	100002
#define NUM3	100003
#define NUM4	100004
#define NUM5	100005
#define NUM6	100006
#define NUM7	100007
#define NUM8	100008
#define NUM9	100009
#define SHPSPRITE	2
#define BHPSPRITE	3
#define LHPSPRITE	4
#define DAGGERSPRITE	507
#define AXESPRITE	509
#define BOOMERANGSPRITE	510
#define HOLYWATERSPRITE	511
#define WATCHSPRITE	508
#define DOUBLESPRITE	512
#define TRISPRITE	513

//define simon
#define SIMON_MAXHP		16
#define SIMON_MANA		15
#define SIMON_LIFE		3
#define TIME_UNTOUCH	1000
#define TIME_INVIS		5000

#define SIMON_WALKING_SPEED		0.13f
#define SIMON_JUMP_SPEED		0.4f 

#define SIMON_GRAVITY			0.0011f

#define SIMON_BBOX_WIDTH		32
#define SIMON_BBOX_HEIGHT		62

#define SIMON_DEFLECT_SPEED_X		0.1f
#define SIMON_DEFLECT_SPEED_Y		0.2f

#define SIMON_STAIR_SPEED_X		0.08f
#define SIMON_STAIR_SPEED_Y		0.08f

#define TIME_HENSHIN 450
#define TIME_DELAY 300
#define TIME_DELAY2	200

#define SIMON_IDLE				0
#define SIMON_WALKING			1
#define SIMON_JUMP			    2
#define SIMON_SITDOWN			3
#define SIMON_ATK				4
#define SIMON_SIT_ATK			5
#define SIMON_STAIRDOWN			6
#define SIMON_STAIRUP			7
#define SIMON_STAIRDOWN_ATK		8
#define SIMON_STAIRUP_ATK		9
#define SIMON_INV_IDLE				10
#define SIMON_INV_WALKING			11
#define SIMON_INV_JUMP			    12
#define SIMON_INV_SITDOWN			13
#define SIMON_INV_ATK				14
#define SIMON_INV_SIT_ATK			15
#define SIMON_INV_STAIRDOWN			16
#define SIMON_INV_STAIRUP			17
#define SIMON_INV_STAIRDOWN_ATK		18
#define SIMON_INV_STAIRUP_ATK		19
#define SIMON_HENSHIN		20
#define SIMON_DEFLECT		21
#define SIMON_DEAD			22
#define SIMON_BACK			23
//define whip
#define ANI_SPARK	72
#define TIME_FRAME 100
#define NORMAL_WHIP	0
#define SHORT_WHIP	1
#define LONG_WHIP	2
#define WHIP_BBOX_HEIGHT 15
#define WHIP_BBOX_WIDTH 55
#define LONG_CHAIN_BBOX_WIDTH 85
#define DAME1 -1
#define DAME2 -2
//define subweapons
#define WEAPONS_STOP_WATCH	4
#define DAGGER 0
#define DAGGER_BBOX_WIDTH  34
#define DAGGER_BBOX_HEIGHT 18
#define WEAPONS_AXE	1
#define WEAPONS_AXE_BBOX_WIDTH  30
#define WEAPONS_AXE_BBOX_HEIGHT 28
#define WEAPONS_BOOMERANG	2
#define WEAPONS_BOOMERANG_BBOX_WIDTH  28
#define WEAPONS_BOOMERANG_BBOX_HEIGHT 28
#define WEAPONS_HOLY_WATER	3
#define WEAPONS_HOLY_WATER_BBOX_WIDTH  32
#define WEAPONS_HOLY_WATER_BBOX_HEIGHT 28
#define WEAPONS_HOLY_WATER_SHATTERED	5
#define WEAPONS_DAGGER_SPEED 0.3
#define WEAPONS_AXE_SPEED_X 0.2
#define WEAPONS_AXE_SPEED_Y 0.45
#define WEAPONS_AXE_GRAVITY 0.001
#define WEAPONS_HOLY_WATER_SPEED_X 0.25
#define WEAPONS_HOLY_WATER_SPEED_Y 0.1
#define WEAPONS_HOLY_WATER_GRAVITY 0.001
#define TIME_HOLY_WATER_SHATTERED 1000
#define WEAPONS_BOOMERANG_SPEED 0.6
#define WEAPONS_BOOMERANG_TURNBACK_SPEED 0.015
//define items
#define ITEM_TIME_DESTROYED	4000
#define ITEM_FALLING_SPEED_Y 0.15f
#define SMALL_HEART	0
#define SMALL_HEART_BBOX_WIDTH  16
#define SMALL_HEART_BBOX_HEIGHT 16
#define MANA1	1
#define LARGE_HEART 1
#define LARGE_HEART_BBOX_WIDTH  24
#define LARGE_HEART_BBOX_HEIGHT 20
#define MANA5	5
#define MONEY_BAG_RED	2
#define MONEY_BAG_BBOX_WIDTH  30
#define MONEY_BAG_BBOX_HEIGHT 30
#define SCORE300	300
#define SCORE100	100
#define SCORE200	200
#define SCORE400	400
#define MONEY_BAG_BLUE	3
#define SCORE500	500
#define MONEY_BAG_WHITE	4
#define SCORE700	700
#define PORK_CHOP	5
#define PORK_CHOP_BBOX_WIDTH  32
#define PORK_CHOP_BBOX_HEIGHT 26
#define ADDHP	4
#define CHAIN 6
#define CHAIN_BBOX_WIDTH  32
#define CHAIN_BBOX_HEIGHT 32
#define STOP_WATCH	7
#define STOP_WATCH_BBOX_WIDTH  30
#define STOP_WATCH_BBOX_HEIGHT 32
#define ITEM_DAGGER 8
#define ITEM_DAGGER_BBOX_WIDTH  32
#define ITEM_DAGGER_BBOX_HEIGHT 18
#define ITEM_AXE	9
#define ITEM_AXE_BBOX_WIDTH  30
#define ITEM_AXE_BBOX_HEIGHT 28
#define ITEM_BOOMERANG	10
#define ITEM_BOOMERANG_BBOX_WIDTH  30
#define ITEM_BOOMERANG_BBOX_HEIGHT 28
#define ITEM_HOLY_WATER	11
#define ITEM_HOLY_WATER_BBOX_WIDTH  30
#define ITEM_HOLY_WATER_BBOX_HEIGHT 28
#define CROSS	12
#define CROSS_BBOX_WIDTH  32
#define CROSS_BBOX_HEIGHT 32
#define INVISIBILITY_POTION	13
#define INVISIBILITY_POTION_BBOX_WIDTH  29
#define INVISIBILITY_POTION_BBOX_HEIGHT 36
#define DOUBLE_SHOT 14
#define EXTRA_SHOT_BBOX_WIDTH  28
#define EXTRA_SHOT_BBOX_HEIGHT 28
#define TRIPLE_SHOT	15
#define CROWN 16
#define CROWN_BBOX_WIDTH  32
#define CROWN_BBOX_HEIGHT 32
#define SCORE1000	1000
#define MAGIC_CRYSTAL	17
#define MAGIC_CRYSTAL_BBOX_WIDTH  26
#define MAGIC_CRYSTAL_BBOX_HEIGHT 32
//define MF
#define PLAT_BBOX_WIDTH			64
#define PLAT_BBOX_HEIGHT		16
#define PLAT_MOVING_SPEED		0.05f	
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
#define STAIR_BBOX_WIDTH  32
#define STAIR_BBOX_HEIGHT 32

#define STAIR1	30.0f
#define STAIR0	4.0f
//define gate
#define GATE_BBOX_WIDTH		48
#define GATE_BBOX_HEIGHT	96
//define Knight
#define KNIGHT_HP	6
#define KNIGHT_WALKING_SPEED			0.06f
#define KNIGHT_GRAVITY					0.002f

#define KNIGHT_BBOX_WIDTH				32
#define KNIGHT_BBOX_HEIGHT				64

#define KNIGHT_STATE_WALKING			0
#define KNIGHT_STATE_DIE				1
//define Darken bat
#define DIST_ACTIVEDB	170
#define DARKBAT_FLYING_SPEED_X			0.2f
#define DARKBAT_FLYING_SPEED_Y			0.25f

#define DARKBAT_BBOX_WIDTH				32
#define DARKBAT_BBOX_HEIGHT				32

#define DARKBAT_STATE_INACTIVE			0
#define DARKBAT_STATE_FLYING			1
#define DARKBAT_STATE_DIE				2
//define Monkey
#define DIST_ACTIVEMK	200
#define MONKEY_WALKING_SPEED			0.16f
#define MONKEY_JUMP_SPEED_Y			0.4f

#define MONKEY_BBOX_WIDTH			32
#define MONKEY_BBOX_HEIGHT			32

#define MONKEY_STATE_WAIT			0
#define MONKEY_STATE_CHASE			1
#define MONKEY_STATE_JUMP			2
#define MONKEY_STATE_DIE			3
//define Ghost
#define DIST_ACTIVEG	100
#define GHOST_HP	6
#define GHOST_FLYING_SPEED			1.25f

#define GHOST_BBOX_WIDTH			32
#define GHOST_BBOX_HEIGHT			32

#define GHOST_STATE_INACTIVE		-1
#define GHOST_STATE_FLYING			0
#define GHOST_STATE_DIE				1
//define Raven
#define RAVEN_SPEED_1	0.05
#define RAVEN_SPEED_2	0.0015

#define RAVEN_BBOX_WIDTH			32
#define RAVEN_BBOX_HEIGHT			32

#define RAVEN_STATE_INACTIVE		0
#define RAVEN_STATE_FLYING			1
#define RAVEN_STATE_DIE				2

#define RAVEN_TIME_CHANGE_STATE		1000
#define RAVEN_TIME_FIND_TARGER		2000
#define RAVEN_TIME_PLAYAGAIN		700
//define Zombie
#define ZOMBIE_BBOX_WIDTH			32
#define ZOMBIE_BBOX_HEIGHT			64

#define ZOMBIE_WALKING_SPEED		0.1f

#define ZOMBIE_STATE_INACTIVE	-1
#define ZOMBIE_STATE_WALKING	0
#define ZOMBIE_STATE_DIE	1
//define Skeleton
#define SKELETON_BBOX_WIDTH			32
#define SKELETON_BBOX_HEIGHT		63

#define SKELETON_JUMPSPEED	0.4
#define SKELETON_WALKINGSPEED	0.11

#define SKELETON_STATE_INACTIVE	0
#define SKELETON_STATE_ACTIVE	1
#define SKELETON_STATE_DIE		2
//define Bone
#define BONE 0

#define BONE_BBOX_WIDTH 32
#define BONE_BBOX_HEIGHT 32
#endif