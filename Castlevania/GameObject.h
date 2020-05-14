#ifndef go_h
#define go_h
#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Define.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"

using namespace std;



class CGameObject;
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};



class CGameObject
{
public:

	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;

	int ani;// dành cho những obj chỉ có 1 state

	int state;

	int IDitems;

	int IdNextMap;

	int HP;

	bool isEnable = false;
	bool isDone = false;
	DWORD dt;

	LPANIMATION_SET animation_set;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	float Getvx() { return vx; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	float GetPositionX() { return x; }
	float GetPositionY() { return y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void SetEnable(bool enable) { this->isEnable = enable; }//
	bool IsEnable() { return this->isEnable; }//
	int GetState() { return this->state; }
	int GetIDitems() { return this->IDitems; }
	int GetIdNextMap() { return this->IdNextMap; }
	void AddHP(int hp) { HP += hp; }
	int GetHP() { return this->HP; }
	void SetHP(int hp) { HP = hp; }

	void RenderBoundingBox();
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }//

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny);

	bool AABB(
		float left_a, float top_a, float right_a, float bottom_a,
		float left_b, float top_b, float right_b, float bottom_b
	);

	void SweptAABB(
		float ml, float mt, float mr, float mb,
		float dx, float dy,
		float sl, float st,
		float sr, float sb,
		float & t, float & nx, float & ny);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }


	~CGameObject();
};

#endif