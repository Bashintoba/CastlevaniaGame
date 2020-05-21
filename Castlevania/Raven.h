#pragma once
#include"GameObject.h"
#include"Game.h"
#include"Timer.h"
#include"Define.h"

class Raven : public CGameObject
{
	int directionY;
	float dist;
	bool findtarget;
	bool isbuffspeed;
	LPGAMEOBJECT target;
	Timer* raven_change_state = new Timer(RAVEN_TIME_CHANGE_STATE);
	Timer* raven_find_target = new Timer(RAVEN_TIME_FIND_TARGER);
public:
	Raven(LPGAMEOBJECT target,float distance);
	~Raven();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

