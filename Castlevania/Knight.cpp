#include "Knight.h"
#include "Ground.h"
#include "BreakBrick.h"
#include "Candle.h"
#include "Stair.h"
#include "Darkenbat.h"

Knight::Knight(float Xstart, float Xend)
{
	this->Xstart = Xstart;
	this->Xend = Xend;
	HP = KNIGHT_HP;
	isDone = false;
	isEnable = false;
}

Knight::~Knight()
{
}

void Knight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (state == KNIGHT_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
		this->isDone = true;

	if (stopMovement == true)
		return;

	if (state != KNIGHT_STATE_DIE)
	{
		CGameObject::Update(dt);
		vy += SIMON_GRAVITY * dt;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		vector<LPGAMEOBJECT> ListsColl;
		coEvents.clear();
		ListsColl.clear();

		for (UINT i = 0; i < coObject->size(); i++)
		{
			if (coObject->at(i) != dynamic_cast<Candle*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Stair*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Darkenbat*>(coObject->at(i)))
			{
				ListsColl.push_back(coObject->at(i));
			}
		}
		// turn off collision when die 
		//if (state != KNIGHT_STATE_DIE)
		CalcPotentialCollisions(&ListsColl, coEvents);

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			// block 
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0 && ny == 0)
			{
				this->nx *= -1;
				this->vx *= -1;
			}
			else
				if (ny == -1)
					this->vy = 0;
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		if (this->x <= Xstart || this->x >= Xend)
		{
			this->nx *= -1;
			this->vx *= -1;
		}

		/*if ((rand() % 10000 < 100))
		{
			this->nx *= -1;
			this->vx *= -1;
		}*/
	}
}

void Knight::Render()
{
	if (this->isDone == false)
	{
		animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Knight::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KNIGHT_STATE_WALKING:
		if (nx > 0)
			vx = KNIGHT_WALKING_SPEED;
		else
			vx = -KNIGHT_WALKING_SPEED;
		break;
	case KNIGHT_STATE_DIE:
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

void Knight::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != KNIGHT_STATE_DIE)
	{
		left = x;
		top = y;
		right = left + KNIGHT_BBOX_WIDTH;
		bottom = top + KNIGHT_BBOX_HEIGHT;
	}
}
