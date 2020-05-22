#include "Skeleton.h"
#include "Ground.h"
#include "BreakBrick.h"
#include "Candle.h"
#include "Stair.h"

Skeleton::Skeleton(LPGAMEOBJECT target)
{
	this->target = target;
	isDone = false;
	isEnable = false;
	isJumping = false;
	canjump = false;
	bone = new Bone(this->target);
}

Skeleton::~Skeleton()
{
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (stopMovement == true)
		return;

	if (isDone == false)
	{
		if (state == SKELETON_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
			this->isDone = true;
	}

	if (x >= target->GetPositionX())
		nx = -1;
	else
		nx = 1;

	CGame* game = CGame::GetInstance();

	if (GetDistance(this->x, this->y, target->x, target->y) < 200 && (x > game->GetCamPosX() && x < (game->GetCamPosX() + SCREEN_WIDTH) && (y >= game->GetCamPosY() && y < (game->GetCamPosY() + SCREEN_HEIGHT))) == true)
	{
		if (state == SKELETON_STATE_INACTIVE)
		{
			SetState(SKELETON_STATE_ACTIVE);
		}
	}

	if (abs(target->GetPositionX() - x) > 180 && state != SKELETON_STATE_INACTIVE)
	{
		if (isJumping == false)
			vx = SKELETON_WALKINGSPEED * nx;
	}

	if (abs(target->GetPositionX() - x) < 60 && state != SKELETON_STATE_INACTIVE)
	{
		if (isJumping == false)
			vx = -SKELETON_WALKINGSPEED * nx;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT> ListsColl;
	coEvents.clear();
	ListsColl.clear();

	for (UINT i = 0; i < coObject->size(); i++)
	{
		if (coObject->at(i) == dynamic_cast<Ground*>(coObject->at(i)) || coObject->at(i) == dynamic_cast<BreakBrick*>(coObject->at(i)))
		{
			ListsColl.push_back(coObject->at(i));
		}
	}

	CalcPotentialCollisions(&ListsColl, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if(canjump)
			isJumping = true;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.4f;
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
						canjump = true;
					}
					else
						y += dy;
				}

				if (e->nx != 0)
				{
					vx *= -1;
				}
			}
		}	
	}

	CGameObject::Update(dt);
	vy += SIMON_GRAVITY * dt;

	if (isJumping)
	{
		vy = -SKELETON_JUMPSPEED;
		canjump = isJumping = false;
	}

	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (bone->isDone == false && state!= SKELETON_STATE_INACTIVE)
	{
		if (bone->isEnable == true)
		{
			bone->SetWeaponPosition(this->x,this->y,this->nx);
			bone->SetState(BONE);
			bone->SetEnable(false);
		}

		bone->Update(dt, coObject);
	}

	if (bone->isDone == true)
		bone->isDone = false;
}

void Skeleton::Render()
{
	bone->Render();
	if (this->isDone == false)
	{
		animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Skeleton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKELETON_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case SKELETON_STATE_ACTIVE:
		break;
	case SKELETON_STATE_DIE:
		this->HP = 0;
		vx = 0;
		vy = 0;
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != SKELETON_STATE_DIE)
	{
		left = x;
		top = y;
		right = left + SKELETON_BBOX_WIDTH;
		bottom = top + SKELETON_BBOX_HEIGHT;
	}
}


