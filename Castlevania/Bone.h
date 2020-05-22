#pragma once
#include "GameObject.h"
#include "Define.h"

class Bone : public CGameObject
{
	LPGAMEOBJECT simon;
	float X;
public:
	Bone(LPGAMEOBJECT simon);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMovement = false);
	virtual void Render();
	void SetWeaponPosition(float X, float Y,int Nx);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	bool CheckOutCam(float x);
};

