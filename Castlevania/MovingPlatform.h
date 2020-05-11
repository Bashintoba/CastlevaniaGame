#pragma once
#include "GameObject.h"
#include "Ground.h"	

class MovingPlatform : public CGameObject
{
public:
	MovingPlatform();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	//void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};