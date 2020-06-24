#include "Zombie.h"
#include "Candle.h"
#include "Ground.h"
#include "BreakBrick.h"
#include "Stair.h"

Zombie::Zombie(LPGAMEOBJECT target)
{
	isDone = false;
	isEnable = false;
	this->target = target;
}

Zombie::~Zombie()
{
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (state == ZOMBIE_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
		this->isDone = true;

	if (stopMovement == true)
	{
		stop = true;
		return;
	}
	else
	{
		stop = false;
	}

	if (state == ZOMBIE_STATE_INACTIVE)
	{
		vx = vy = 0;
	}
	
	if (state != ZOMBIE_STATE_DIE)
	{
		CGameObject::Update(dt);
		vy += SIMON_GRAVITY * dt;



		CGame* game = CGame::GetInstance();

		if ((x > game->GetCamPosX() && x < (game->GetCamPosX() + SCREEN_WIDTH) && (y >= game->GetCamPosY() && y < (game->GetCamPosY() + SCREEN_HEIGHT))) == true)
		{
			if (state == ZOMBIE_STATE_INACTIVE)
			{
				if (x >= target->GetPositionX())
					nx = -1;
				else
					nx = 1;
				SetState(ZOMBIE_STATE_WALKING);
			}

		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		vector<LPGAMEOBJECT> ListsColl;
		coEvents.clear();
		ListsColl.clear();

		for (UINT i = 0; i < coObject->size(); i++)
		{
			if (coObject->at(i) != dynamic_cast<Candle*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Stair*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Zombie*>(coObject->at(i)))
			{
				ListsColl.push_back(coObject->at(i));
			}
		}

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

			x += min_tx * dx + nx * 0.1f;
			y += min_ty * dy + ny * 0.1f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<BreakBrick*>(e->obj)))
				{
					if (e->ny == -1.0f)
					{
						vy = 0;
					}
					if (nx != 0) x -= nx * 0.1f;
				}
			}
		}

		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void Zombie::Render()
{
	if (this->isDone == false && state != ZOMBIE_STATE_INACTIVE)
	{
		if (stop == true && state != ZOMBIE_STATE_DIE)
		{
			int temp = animation_set->at(state)->GetCurrentFrame();
			animation_set->at(state)->SetFrame(temp);
			animation_set->at(state)->RenderByID(temp, nx, x, y);
		}
		else
			animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Zombie::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ZOMBIE_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case ZOMBIE_STATE_WALKING:
		if (nx > 0) vx = ZOMBIE_WALKING_SPEED;
		else vx = -ZOMBIE_WALKING_SPEED;
		vy = 0;
		break;
	case ZOMBIE_STATE_DIE:
		this->HP = 0;
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

void Zombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != ZOMBIE_STATE_DIE && state != ZOMBIE_STATE_INACTIVE)
	{
		left = x;
		top = y;
		right = left + ZOMBIE_BBOX_WIDTH;
		bottom = top + ZOMBIE_BBOX_HEIGHT;
	}
}
