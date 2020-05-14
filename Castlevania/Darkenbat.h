#pragma once
#include "GameObject.h"
#include "Define.h"

#define DARKBAT_FLYING_SPEED_X			0.2f
#define DARKBAT_FLYING_SPEED_Y			0.25f
#define DARKBAT_AMPLITUDE_HORIZONTAL	40	

#define DARKBAT_BBOX_WIDTH				32
#define DARKBAT_BBOX_HEIGHT				32

#define DARKBAT_STATE_INACTIVE			0
#define DARKBAT_STATE_FLYING			1
#define DARKBAT_STATE_DIE				2

class Darkenbat : public CGameObject
{
	float tempX;
	float tempY;
	int directionY;
	bool Switch = false;
	LPGAMEOBJECT target;
public:
	Darkenbat(LPGAMEOBJECT target);
	~Darkenbat();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

