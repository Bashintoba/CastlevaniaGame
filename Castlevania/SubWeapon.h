#pragma once

#include "GameObject.h"
#include "Define.h"

class SubWeapon :public CGameObject
{
	float X;//vi tri ban dau
	bool isHolyWaterShattered = false;
	int holyWaterShatteredCounter = 0;
public:
	SubWeapon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {}
	void Render(int nx, int currentID = -1);
	void SetWeaponPosition(int nx,float simonX,float simonY, bool isStand);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void SetState(int state);
	bool CheckOutCam(float x);
	void StartHolyWater() { isHolyWaterShattered = true; holyWaterShatteredCounter = GetTickCount(); }
};

