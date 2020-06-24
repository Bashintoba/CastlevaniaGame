#pragma once
#include "GameObject.h"
#include "Define.h"

class Monkey : public CGameObject
{
	bool isJumping = false;
	bool isChase = false;
	bool stop = false;
	LPGAMEOBJECT target;
public:
	Monkey(LPGAMEOBJECT target);
	~Monkey();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

