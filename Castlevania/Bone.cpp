#include "Bone.h"
#include "Game.h"
#include "Simon.h"

Bone::Bone(LPGAMEOBJECT simon)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(25);
	SetAnimationSet(ani_set);
	isDone = false;
	isEnable = true;
	this->simon = simon;
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	CGame* game = CGame::GetInstance();

	if((x >= game->GetCamPosX() && x < (game->GetCamPosX() + SCREEN_WIDTH) && (y >= game->GetCamPosY() && y < (game->GetCamPosY() + SCREEN_HEIGHT)))== false)
	{
		this->isEnable = true;
		this->isDone = true;
		return;

	}

	CGameObject::Update(dt);
	vy += WEAPONS_AXE_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT> ListsColl;
	coEvents.clear();
	ListsColl.clear();

	ListsColl.push_back(simon);

	CalcPotentialCollisions(&ListsColl, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Simon*>(e->obj))
			{
				Simon* s = dynamic_cast<Simon*>(e->obj);
				if (s->state != SIMON_DEAD && s->state != SIMON_HENSHIN && s->untouchableTimer->IsTimeUp() == true && s->invisibilityTimer->IsTimeUp() == true)
				{
					s->untouchableTimer->Start();
					s->AddHP(-1);
					if (s->isOnStair == false || s->HP == 0)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// đặt trạng thái deflect cho simon
						if (nx != 0)
						{
							if (this->nx == 1.0f && s->nx == 1) s->nx = -1;
							else if (this->nx == -1.0f && s->nx == -1) s->nx = 1;
						}
						s->SetState(SIMON_DEFLECT);
					}
				}
				this->isDone = true;
				this->isEnable = false;
				x += dx;
				y += dy;
			}
		}
	}

}

void Bone::Render()
{
	if (isDone == false && CheckOutCam(X) == false)
	{
		animation_set->at(0)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Bone::SetWeaponPosition(float X, float Y, int Nx)
{
	X += 5;
	Y += 5;
	nx = Nx;
	this->X = X;
	SetPosition(X, Y);
}

void Bone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BONE_BBOX_WIDTH;
	bottom = top + BONE_BBOX_HEIGHT;
}

void Bone::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BONE:
		if (nx > 0) vx = 0.15;
		else vx = -0.15;
		vy = -WEAPONS_AXE_SPEED_Y;
		break;
	default:
		break;
	}
}

bool Bone::CheckOutCam(float x)
{
	if (vx > 0)
	{
		if (this->x - x >= (SCREEN_WIDTH / 2))
			return true;
	}
	else if (vx < 0)
	{
		if (x - this->x >= (SCREEN_WIDTH / 2))
			return true;
	}
	return false;
}
