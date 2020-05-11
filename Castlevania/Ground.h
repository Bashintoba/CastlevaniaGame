#pragma once
#include "GameObject.h"
#include "Define.h"
class Ground : public CGameObject
{
public:
	Ground();
	~Ground();
	void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};