#pragma once
#include "GameObject.h"
#include "Define.h"

class Darkenbat : public CGameObject
{
	int directionY;
	bool Switch = false;
	bool stop = false;
	LPGAMEOBJECT target;
public:
	Darkenbat(LPGAMEOBJECT target);
	~Darkenbat();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

