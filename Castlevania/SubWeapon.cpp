#include "SubWeapon.h"
#include "Candle.h"
#include "Ground.h"
#include "Simon.h"
#include "BreakBrick.h"

SubWeapon::SubWeapon() : CGameObject()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(SW_ANIMATION_SET);
	SetAnimationSet(ani_set);
	SetState(-1);
	//nx = 1;
	vx = 0;
	isDone = true;
}

void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDone == false)
	{
		if (isHolyWaterShattered == true && GetTickCount() - holyWaterShatteredCounter > TIME_HOLY_WATER_SHATTERED)
		{
			isHolyWaterShattered = false;
			holyWaterShatteredCounter = 0;
			this->isDone = true;
			return;
		}

		if (CheckOutCam(X) == true)
		{
			isDone = true;
			isEnable = false;
			return;
		}

		CGameObject::Update(dt);

		switch (state)
		{
		case WEAPONS_AXE:
			vy += WEAPONS_AXE_GRAVITY * dt;
			break;
		case WEAPONS_BOOMERANG:
			if (nx > 0) vx -= WEAPONS_BOOMERANG_TURNBACK_SPEED;
			else vx += WEAPONS_BOOMERANG_TURNBACK_SPEED;
			break;
		case WEAPONS_HOLY_WATER:
			vy += WEAPONS_HOLY_WATER_GRAVITY * dt;
			break;
		default:
			break;
		}

		//if (isDone == false)
		//{

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

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
				if (dynamic_cast<Candle*>(e->obj))
				{
					Candle * candle = dynamic_cast<Candle*> (e->obj);
					candle->SetState(CANDLE_DESTROYED);
					if (state != WEAPONS_BOOMERANG)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<BreakBrick*>(e->obj))
				{
					BreakBrick * BB = dynamic_cast<BreakBrick*> (e->obj);	
					BB->isDone = true;
					if (state != WEAPONS_BOOMERANG)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Ground*>(e->obj))
				{
					if (state == WEAPONS_HOLY_WATER && e->ny == -1)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
					}
					else
					{
						x += dx;
						y += dy;
					}
				}
				else if (dynamic_cast<Simon*>(e->obj))
				{
					if (state == WEAPONS_BOOMERANG)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				//if (this->isDone == true) return;
			}
		}
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void SubWeapon::Render(int nx, int currentID)
{
	if (isDone == false && CheckOutCam(X) == false && state != WEAPONS_STOP_WATCH)
	{
		if (state == -1) return;
		if (state == WEAPONS_HOLY_WATER_SHATTERED)
		{
			animation_set->at(4)->Render(nx, x, y);
		}
		else
		{
			animation_set->at(state)->Render(nx, x, y);
		}
		RenderBoundingBox();
	}
}

void SubWeapon::SetWeaponPosition(int nx,float simonX, float simonY, bool isStand)
{
	if (nx > 0)
	{
		simonX += 45;
	}
	else
		simonX -= 35;
	if (isStand == false)
		simonY += 25;
	else
		simonY += 15;
	X = simonX;
	SetPosition(simonX, simonY);
}

void SubWeapon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case DAGGER:
		if (nx > 0) vx = WEAPONS_DAGGER_SPEED;
		else vx = -WEAPONS_DAGGER_SPEED;
		vy = 0;
		break;
	case WEAPONS_AXE:
		if (nx > 0) vx = WEAPONS_AXE_SPEED_X;
		else vx = -WEAPONS_AXE_SPEED_X;
		vy = -WEAPONS_AXE_SPEED_Y;
		break;
	case WEAPONS_BOOMERANG:
		if (nx > 0) vx = WEAPONS_BOOMERANG_SPEED;
		else vx = -WEAPONS_BOOMERANG_SPEED;
		vy = 0;
		break;
	case WEAPONS_HOLY_WATER:
		vx = nx * WEAPONS_HOLY_WATER_SPEED_X;
		vy = -WEAPONS_HOLY_WATER_SPEED_Y;
		break;
	case WEAPONS_STOP_WATCH:
		break;
	case WEAPONS_HOLY_WATER_SHATTERED:
		vx = 0;
		vy = 0;
		StartHolyWater();
		break;	
	default:
		break;
	}
}

bool SubWeapon::CheckOutCam(float x)
{
	if (vx > 0)	
	{
		if (this->x - x >= (SCREEN_WIDTH /2 ))
			return true;
	}
	else if (vx < 0)
	{
		if (x - this->x >= (SCREEN_WIDTH / 2 ))
			return true;
	}
	return false;
}

void SubWeapon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	switch (state)
	{
	case DAGGER:
		right = left + 34;
		bottom = top + 18;
		break;
	case WEAPONS_AXE:
		right = left + 30;
		bottom = top + 28;
		break;
	case WEAPONS_HOLY_WATER:
		right = left + 32;
		bottom = top + 28;
		break;
	case WEAPONS_BOOMERANG:
		right = left + 28;
		bottom = top + 28;
		break;
	case WEAPONS_STOP_WATCH:
		right = left;
		bottom = top;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}