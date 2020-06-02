#pragma once
#include "GameObject.h"
#include "Define.h"

class Stair : public CGameObject
{
public: 
	int type;
	Stair(int type);
	~Stair();
	void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

