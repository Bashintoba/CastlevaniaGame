#include "Ghost.h"

Ghost::Ghost(LPGAMEOBJECT target)
{
	this->HP = 6;
	isDone = false;
	isEnable = false;
	this->target = target;
}

Ghost::~Ghost()
{
}

void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (stopMovement == true)
		return;

	if (isDone == false)
	{
		if (state == GHOST_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
			this->isDone = true;
	}

	if (target != NULL)
	{
		if (GetDistance(this->x, this->y, target->x, target->y) > 100)
		{
			SetState(GHOST_STATE_FLYING);
		}
	}

	D3DXVECTOR2 position = D3DXVECTOR2(x, y);

	position += Radia(D3DXVECTOR2(target->GetPositionX(), target->GetPositionY()), position, GHOST_FLYING_SPEED);


	x = position.x;
	y = position.y;
	if (target->GetPositionX() < x)
		nx = -1;
	else
		nx = 1;

	CGameObject::Update(dt);
}

void Ghost::Render()
{
	if (this->isDone == false && state != GHOST_STATE_INACTIVE)
	{
		animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Ghost::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GHOST_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case GHOST_STATE_FLYING:
		break;
	case GHOST_STATE_DIE:
		this->HP = 0;
		vx = 0;
		vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != GHOST_STATE_DIE && state!= GHOST_STATE_INACTIVE)
	{
		left = x;
		top = y;
		right = left + GHOST_BBOX_WIDTH;
		bottom = top + GHOST_BBOX_HEIGHT;
	}
}
