#pragma once
#include "GameObject.h"
#include "Ground.h"

#define PLAT_BBOX_WIDTH			64
#define PLAT_BBOX_HEIGHT		16

#define PLAT_MOVING_SPEED		0.05f	//50% player's speed		

class MovingPlatform : public CGameObject
{
public:
	MovingPlatform();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};