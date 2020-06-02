#include "Darkenbat.h"

Darkenbat::Darkenbat(LPGAMEOBJECT target)
{
	isDone = false;
	isEnable = false;
	directionY = 1;
	this->target = target;
}

Darkenbat::~Darkenbat()
{
}

void Darkenbat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (state == DARKBAT_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
		this->isDone = true;

	if (stopMovement == true)
		return;

	if (state != DARKBAT_STATE_DIE)
	{
		CGameObject::Update(dt);
		x += dx;
		y += dy;
		//}

		if (target != NULL)
		{
			if (GetDistance(this->x, this->y, target->x, target->y) <= DIST_ACTIVEDB)
			{
				if (target->GetState() != SIMON_STAIRDOWN && target->GetState() != SIMON_STAIRUP && target->GetState() != SIMON_STAIRDOWN_ATK && target->GetState() != SIMON_STAIRUP_ATK)
				{
					SetState(DARKBAT_STATE_FLYING);
				}
			}
		}

		if (vx != 0 && Switch == false)
		{
			vy = DARKBAT_FLYING_SPEED_Y * directionY;
		}
		else
			vy = 0;

		if (abs(this->GetPositionY() - target->y) <= 5)
		{
			Switch = true;
		}
	}
}

void Darkenbat::Render()
{
	if (this->isDone == false)
	{
		animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Darkenbat::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case DARKBAT_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case DARKBAT_STATE_FLYING:
		if (nx > 0)
			vx = DARKBAT_FLYING_SPEED_X;
		else
			vx = -DARKBAT_FLYING_SPEED_X;
		directionY = 1;
		break;
	case DARKBAT_STATE_DIE:
		this->HP = 0;
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;	
	}
}

void Darkenbat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != DARKBAT_STATE_DIE)
	{
		left = x;
		top = y;
		right = left + DARKBAT_BBOX_WIDTH;
		bottom = top + DARKBAT_BBOX_HEIGHT;
	}
}
