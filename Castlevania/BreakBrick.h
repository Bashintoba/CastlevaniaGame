#pragma once
#include "GameObject.h"


class BreakBrick : public CGameObject
{
	
public:
	BreakBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
