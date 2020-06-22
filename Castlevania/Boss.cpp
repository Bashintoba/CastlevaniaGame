#include "Boss.h"



#define BOSS_BBOX_WIDTH 70
#define BOSS_BBOX_HEIGHT 46

Boss::Boss(LPGAMEOBJECT target)
{
	isDone = false;
	isEnable = false;
	isStart = false;
	isAtk = false;
	isFindtarget = false;
	this->target = target;
	this->HP = SIMON_MAXHP;
}

Boss::~Boss()
{
}

void Boss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (state == BOSS_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
		this->isDone = true;

	if (stopMovement == true)
		return;

	if (state != BOSS_STATE_DIE)
	{
		CGameObject::Update(dt);
		x += dx;
		y += dy;

		if (isFindtarget == true)
		{
			if (x < target->GetPositionX()) Nx = 1;
			else Nx = -1;
		}

		if (isFindtarget == true)
		{
			if (y < target->GetPositionY()) Ny = 1;
			else Ny = -1;
		}

		if (target != NULL)
		{
			CGame* game = CGame::GetInstance();
			if (x<target->GetPositionX() && (abs(y-target->GetPositionY())<=150))
			{
				if (state == BOSS_STATE_INACTIVE)
				{
					Boss_move_to_postion->Start();
					SetState(BOSS_STATE_ATK);
					vx = (float)(0.1)*-1;
					vy = (float)(0.1)*1;
					isStart = true;
				}
			}
		}

		if (Boss_move_to_postion->IsTimeUp() == true && isStart == true)
		{
			Boss_find_target->Start();
			isStart = false;
			isAtk = true;
			vx = 0;
			vy = 0;
		}

		if (Boss_find_target->IsTimeUp() == true && state!= BOSS_STATE_INACTIVE && isAtk == true)
		{
			isFindtarget = true;
			float dx = abs(x - target->GetPositionX());
			float dy = abs(y - target->GetPositionY());

			vx = Nx * dx *0.0015;
			vy = Ny * dy *0.0015;
			isAtk = false;
			//BossAtk();
		}

		/*if (abs(x - target->GetPositionX()) <= 1.0f)
		{

		}*/

	}
}

void Boss::Render()
{
	if (this->isDone == false)
	{
		animation_set->at(state)->Render(nx, x, y);
		RenderBoundingBox();
	}
	else
		return;
}

void Boss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case BOSS_STATE_ATK:
		break;
	case BOSS_STATE_DIE:
		this->HP = 0;
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

void Boss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != BOSS_STATE_DIE)
	{
		left = x + 13; // 70, 96
		top = y;
		right = left + BOSS_BBOX_WIDTH;
		bottom = top + BOSS_BBOX_HEIGHT;
	}
}

void Boss::BossAtk()
{
	float dx = abs(x - target->GetPositionX());
	float dy = abs(y - target->GetPositionY());

	vx = Nx * dx / 750;
	vy = Ny * dy / 750;
}
