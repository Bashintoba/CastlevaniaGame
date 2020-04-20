#pragma once

#include "GameObject.h"
#include"Define.h"
class Candle : public CGameObject
{
public:
	Candle();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
