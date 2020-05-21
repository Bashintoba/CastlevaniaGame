#pragma once
#include"GameObject.h"
#include"Game.h"
#include"Define.h"

class Zombie : public CGameObject
{
	LPGAMEOBJECT target;
public:
	Zombie(LPGAMEOBJECT target);
	~Zombie();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

