#pragma once
#include "GameObject.h"


class BreakBrick : public CGameObject
{
	
public:
	BreakBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
