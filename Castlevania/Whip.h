#pragma once

#include "GameObject.h"
#include "Define.h"

class Whip : public CGameObject
{
	vector<float> sparkX;
	vector<float> sparkY;
	CAnimation * spark = CAnimations::GetInstance()->Get(72);
	int startTimeRenderSpark = 0;
	bool Dame1turn = false;
public:
	Whip();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {}
	void RenderSpark();
	void Render(int nx,int currentID = -1);
	void SetWhipPosition(int nx,float simonX,float simonY, bool isStand);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	void PowerUp();
	void SetDame1Turn(bool D1T) { Dame1turn = D1T; }
};

