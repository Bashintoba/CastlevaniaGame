#pragma once
#ifndef boss_h
#define boss_h

#define BOSS_STATE_INACTIVE 0
#define BOSS_STATE_ATK 1
#define BOSS_STATE_DIE 2

#include"GameObject.h"
#include"Game.h"
#include"Define.h"
#include"debug.h"
#include"Timer.h"

class Boss : public CGameObject
{
	int Nx, Ny;
	bool isStart,isAtk,isFindtarget;
	LPGAMEOBJECT target;
	Timer* Boss_move_to_postion = new Timer(1500);
	Timer* Boss_find_target = new Timer(1500);
public: 
	Boss(LPGAMEOBJECT target);
	~Boss();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void BossAtk();
};

#endif