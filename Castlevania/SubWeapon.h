#pragma once

#include "GameObject.h"
#include "Define.h"

class SubWeapon :public CGameObject
{
	float X;//vi tri ban dau
	LPGAMEOBJECT simon;
	bool isHolyWaterShattered = false;
	int holyWaterShatteredCounter = 0;
	bool DameBoomerang1 = false; 
	bool DameBoomerang2 = false;
public:
	SubWeapon(LPGAMEOBJECT simon);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {}
	void Render(int currentID = -1);
	void SetWeaponPosition(float simonX,float simonY, bool isStand);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void SetState(int state);
	bool CheckOutCam(float x);
	void StartHolyWater() { isHolyWaterShattered = true; holyWaterShatteredCounter = GetTickCount(); }
};

