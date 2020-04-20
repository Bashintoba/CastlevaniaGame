#pragma once
#include "GameObject.h"
#include "Define.h"

class Stair : public CGameObject
{
public: 
	Stair();
	~Stair();
	void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
};

