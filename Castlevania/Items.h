#pragma once
#include "GameObject.h"
#include "Define.h"

class Items : public CGameObject
{
	DWORD timeAppear;  
public:
	Items();

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void SetState(int state);
};

