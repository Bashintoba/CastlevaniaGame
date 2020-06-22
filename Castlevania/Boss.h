#pragma once
#ifndef boss_h
#define boss_h

#include"GameObject.h"
#include"Game.h"
#include"Define.h"
#include"debug.h"
#include"Timer.h"

class Boss : public CGameObject
{
	int Nxtemp;
	int Nx, Ny , location;
	bool isStart,isAtk,isFindtarget,infinity,wait,relax;
	LPGAMEOBJECT target;
	Timer* Boss_move_to_postion = new Timer(BOSS_TIMER);
	Timer* Boss_find_target = new Timer(BOSS_TIMER);
	Timer* Boss_relax = new Timer(BOSS_TIMER);
	Timer* Boss_goaway = new Timer(BOSS_TIMER);
	CGame* game = CGame::GetInstance();
public: 
	Boss(LPGAMEOBJECT target);
	~Boss();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

#endif