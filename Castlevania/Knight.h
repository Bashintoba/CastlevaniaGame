#pragma once
#include"GameObject.h"
#include "Define.h"

class Knight : public CGameObject
{
	float Xstart;
	float Xend;
public:
	Knight(float Xstart,float Xend);
	~Knight();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

