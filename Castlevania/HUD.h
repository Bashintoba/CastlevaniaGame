#pragma once
#include "Define.h"
#include "Simon.h"
#include "Animations.h"
#include "Sprites.h"
#include "Textures.h"
#include "Number.h"
#include<string>
#include<time.h>

class HUD
{
	Simon * simon;
	Number number;
	CAnimation * board = CAnimations::GetInstance()->Get(100);
	vector<LPSPRITE> SubWeaponItems;
	vector<LPSPRITE> DoubleTri;
	vector<LPSPRITE> SimonHP;
	vector<LPSPRITE> BossHP;
	vector<LPSPRITE> LoseHP;
	string FillNumber(string s, UINT MaxNumber);
	int Score;
	int Time;
	int Item;
	int Mana;
	int Life;
	int subWeapon;
	int doubletri;
	int simonHP;
	int bossHP = 16;
	bool isTimeover = false;
public:
	HUD(Simon * simon);
	~HUD();
	int RemainingTime;
	void Update(DWORD dt);
	void Render(int map,int CamX, int CamY = 0);
	bool GetisTimeover() { return isTimeover; }
	void SetisTimeover(bool timeover) { isTimeover = timeover; }
	void SetRemainTime(int time) { RemainingTime = time; }
	void SetTime(int time) { Time = time; }
};

