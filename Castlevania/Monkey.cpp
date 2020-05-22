#include "Monkey.h"
#include "Candle.h"
#include "Stair.h"
#include "Ground.h"
#include "BreakBrick.h"
#include "Ghost.h"

Monkey::Monkey(LPGAMEOBJECT target)
{
	isDone = false;
	isEnable = false;
	this->target = target;
}

Monkey::~Monkey()
{
}

void Monkey::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (stopMovement == true)
		return;

	CGameObject::Update(dt);
	if(state!= MONKEY_STATE_DIE)
		vy += SIMON_GRAVITY * dt;

	if (isDone == false)
	{
		if (state == MONKEY_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
			this->isDone = true;
	}

	if (target != NULL)
	{
		if (GetDistance(this->x, this->y, target->x, target->y) <= 200)
		{
			if (isChase == false)
			{
				isChase = true;
				SetState(MONKEY_STATE_JUMP);
			}
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT> ListsColl;
	coEvents.clear();
	ListsColl.clear();

	for (UINT i = 0; i < coObject->size(); i++)
	{
		if (coObject->at(i) != dynamic_cast<Candle*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Stair*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Ghost*>(coObject->at(i)))
		{
			ListsColl.push_back(coObject->at(i));
		}
	}
	// turn off collision when die 
	if (state != MONKEY_STATE_DIE)
		CalcPotentialCollisions(&ListsColl, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<BreakBrick*>(e->obj)))
			{
				if (e->ny != 0)
				{
					if (e->ny == -1)
					{
						vy = 0;
						if (isJumping == true)
						{
							isJumping = false;
						}
					}
					else
						y += dy;
				}
			}
		}
	}

	if (state != MONKEY_STATE_DIE && state != MONKEY_STATE_WAIT)
	{
		if (x < target->GetPositionX())
		{
			if (abs(target->GetPositionX() - x) > 100)
			{
				//if (isJumping == false)
				//{
					nx = 1;
				//}
				SetState(MONKEY_STATE_CHASE);
			}
		}
		else if (x > target->GetPositionX())
		{
			if ((abs(target->GetPositionX() - x) > 100))
			{
				//if (isJumping == false)
				//{
					nx = -1;
				//}
				SetState(MONKEY_STATE_CHASE);
			}
		}

		if ((rand() % 10000 < 300))
		{
			if (isJumping == false && (abs(target->GetPositionX() - x) < 100))
			{
				SetState(MONKEY_STATE_JUMP);
			}
		}
	}

	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

void Monkey::Render()
{
	if (this->isDone == false)
	{
		animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Monkey::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MONKEY_STATE_WAIT:
		vx = 0;
		vy = 0;
		break;
	case MONKEY_STATE_CHASE:
		if (nx > 0)
			vx = MONKEY_WALKING_SPEED;
		else
			vx = -MONKEY_WALKING_SPEED;
		break;
	case MONKEY_STATE_JUMP:
		isJumping = true;
		vy = -MONKEY_JUMP_SPEED_Y;
		break;
	case MONKEY_STATE_DIE:
		this->HP = 0;
		isChase = false;
		vx = 0;
		vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

void Monkey::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != MONKEY_STATE_DIE)
	{
		left = x;
		top = y;
		right = left + MONKEY_BBOX_WIDTH;
		bottom = top + MONKEY_BBOX_HEIGHT;
	}
}
