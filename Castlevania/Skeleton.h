#pragma once
#include"GameObject.h"
#include"Game.h"
#include"Define.h"
#include"Bone.h"
#include"debug.h"

class Skeleton : public CGameObject
{
	bool isJumping,canjump;
	Bone* bone;
	LPGAMEOBJECT target;
public:
	Skeleton(LPGAMEOBJECT target);
	~Skeleton();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	Bone* GetBone() { return bone; }
};
