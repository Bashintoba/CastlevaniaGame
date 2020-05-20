#pragma once
#include"GameObject.h"
#include "Define.h"

class Ghost : public CGameObject
{
	LPGAMEOBJECT target;
public:
	Ghost(LPGAMEOBJECT target);
	~Ghost();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

