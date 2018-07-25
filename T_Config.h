/*
T_Config.h
整个游戏引擎公用的头文件
*/

#ifndef TCONFIG_H_
#define TCONFIG_H_

#include <windows.h>
#include <typeinfo.h>
#include <sstream>
#include <fstream>
#include <iostream>

#include <vector>
#include <gdiplus.h>
#include <string>
#include <gdiplus.h>
#include <ctime>

#include "resource.h"

#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"gdiplus.lib")

using namespace Gdiplus;
using namespace std;

static const int G_ALL_FRAMES = 4; //表示坦克图片每个方向的帧动画数目
static const double G_BULLET_SPEED = 2.5; //表示子弹速度与坦克速度的倍数，bullet_speed == tank_speed * G_BULLET_SPEED
static const int G_EXPLODE_FRAMES = 8; //表示爆炸效果图的动画帧数 
static const int G_TIME_OF_NPC_BULLET = 18; //表示NPC坦克发射每发子弹的平均间隔次数，设置为3时，平均每3*G_GAP_TIME_OF_TANK毫秒发射一发子弹

static const int G_GAP_TIME_OF_TANK = 500;//表示坦克每个子弹的最少时间间隔
static const int G_PLAYER_TANK_SPEED = 5; //玩家坦克的速度
static const int G_NPC_TANK_SPEED = 3;//npc坦克的速度

static const int G_COUNT_NPC_TANK_AT1 = 3;//npc坦克的数目
static const int G_COUNT_NPC_TANK_AT2 = 3;//npc坦克的数目
static const int G_COUNT_NPC_TANK_AT3 = 4;//npc坦克的数目

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?0:1)

enum GAME_STATE
{
	GAME_START,
	GAME_RUN_SINGLE,
	GAME_RUN_DOUBLE,
	GAME_PAUSE,
	GAME_UPGRADE,
	GAME_WIN,
	GAME_OVER,
	GAME_HELP,
	GAME_ABOUT,
	GAME_SETTING,
};

enum KM_ACTION
{
	KEY_SYS_NONE,
	KEY_UP,
	KEY_DOWN,

	MOUSE_LCLICK,
	MOUSE_RCLICK,
	MOUSE_MOVE,

};

enum GAME_DIR
{
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
};

enum MENU_INDEX
{
	MENU_SINGLE_PEOPLE = 0,
	MENU_DOUBLE_PEOPLE,
	MENU_HELP,
	MENU_EXIT,
};

enum SOUND_INDEX
{
	SOUND_MENU,
	SOUND_RUN,
	SOUND_EXPLODE,
	SOUND_TURN, //坦克转向声音
	SOUND_WIN,
	SOUND_FAIL,

};
#endif

