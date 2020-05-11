#pragma once

#include "GameObject.h"
#include "Define.h"

class Gate : public CGameObject
{
	
public:
	Gate();
	~Gate();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Render();

};
