#include "SubWeapon.h"
#include "Candle.h"
#include "Ground.h"
#include "Simon.h"
#include "BreakBrick.h"
#include "Knight.h"
#include "Darkenbat.h"
#include "MovingPlatform.h"
#include "Monkey.h"
#include "Ghost.h"
#include "Raven.h"
#include "Zombie.h"
#include "Skeleton.h"
#include "Gate.h"

SubWeapon::SubWeapon(LPGAMEOBJECT simon) : CGameObject()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(SW_ANIMATION_SET);
	SetAnimationSet(ani_set);
	SetState(-1);
	vx = 0;
	isDone = true;
	this->simon = simon;
}

void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	if (isDone == false)
	{
		if (isHolyWaterShattered == true && GetTickCount() - holyWaterShatteredCounter > TIME_HOLY_WATER_SHATTERED)
		{
			isHolyWaterShattered = false;
			holyWaterShatteredCounter = 0;
			this->isDone = true;
			this->isEnable = false;
			return;
		}

		if (CheckOutCam(X) == true)
		{
			isDone = true;
			isEnable = false;
			this->DameBoomerang1 = false;
			this->DameBoomerang2 = false;
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

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		vector<LPGAMEOBJECT> ListsColl;			
		coEvents.clear();
		
		ListsColl.clear();
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i) != dynamic_cast<Gate*>(coObjects->at(i)))
			{
				ListsColl.push_back(coObjects->at(i));
			}
		}
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
				if (dynamic_cast<Candle*>(e->obj))
				{
					Candle * candle = dynamic_cast<Candle*> (e->obj);
					candle->AddHP(DAME2);
					if (candle->GetHP() <= 0)
					{
						candle->SetHP(0);
						candle->SetState(CANDLE_DESTROYED);
					}
					
					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = candle->x;
						this->y = candle->y;
					}
					else if (state != WEAPONS_BOOMERANG && state!= WEAPONS_HOLY_WATER_SHATTERED)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<BreakBrick*>(e->obj))
				{
					BreakBrick * BB = dynamic_cast<BreakBrick*> (e->obj);
					BB->AddHP(DAME2);
					if (BB->GetHP() <= 0)
					{
						BB->SetHP(0);
						BB->isDone = true;
					}

					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = BB->x;
						this->y = BB->y;
					}
					else if (state != WEAPONS_BOOMERANG && state != WEAPONS_HOLY_WATER_SHATTERED)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Knight*>(e->obj))
				{
					Knight* knight = dynamic_cast<Knight*> (e->obj);

					if (state == WEAPONS_BOOMERANG)
					{
						x += dx;
						y += dy;
						if (DameBoomerang1 == false)
						{
							knight->AddHP(DAME2);
							DameBoomerang1 = true;
							if (knight->GetHP() <= 0)
							{
								knight->SetHP(0);
								knight->SetState(KNIGHT_STATE_DIE);
							}
							return;
						}
						if (DameBoomerang2 == false)
						{
							knight->AddHP(DAME2);
							DameBoomerang2 = true;
							if (knight->GetHP() <= 0)
							{
								knight->SetHP(0);
								knight->SetState(KNIGHT_STATE_DIE);
							}
							return;
						}
					}
					else
					{
						knight->AddHP(DAME2);
					}
						
					if (knight->GetHP() <= 0)
					{
						knight->SetHP(0);
						knight->SetState(KNIGHT_STATE_DIE);
					}

					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = knight->x;
						this->y = knight->y;
					}
					else if (state != WEAPONS_BOOMERANG && state != WEAPONS_HOLY_WATER_SHATTERED)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Darkenbat*>(e->obj))
				{
					Darkenbat* db = dynamic_cast<Darkenbat*> (e->obj);
					db->AddHP(DAME2);
					if (db->GetHP() <= 0)
					{
						db->SetHP(0);
						db->SetState(DARKBAT_STATE_DIE);
					}

					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = db->x;
						this->y = db->y;
					}
					else if (state != WEAPONS_BOOMERANG && state != WEAPONS_HOLY_WATER_SHATTERED)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Monkey*>(e->obj))
				{
					Monkey* monkey = dynamic_cast<Monkey*> (e->obj);
					monkey->AddHP(DAME2);
					if (monkey->GetHP() <= 0)
					{
						monkey->SetHP(0);
						monkey->SetState(MONKEY_STATE_DIE);
					}

					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = monkey->x;
						this->y = monkey->y;
					}
					else if (state != WEAPONS_BOOMERANG)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Ghost*>(e->obj))
				{
					Ghost* ghost = dynamic_cast<Ghost*> (e->obj);

					if (state == WEAPONS_BOOMERANG)
					{
						x += dx;
						y += dy;
						if (DameBoomerang1 == false)
						{
							ghost->AddHP(DAME2);
							DameBoomerang1 = true;
							if (ghost->GetHP() <= 0)
							{
								ghost->SetHP(0);
								ghost->SetState(GHOST_STATE_DIE);
							}
							return;
						}
						if (DameBoomerang2 == false)
						{
							ghost->AddHP(DAME2);
							DameBoomerang2 = true;
							if (ghost->GetHP() <= 0)
							{
								ghost->SetHP(0);
								ghost->SetState(GHOST_STATE_DIE);
							}
							return;
						}
					}
					else
					{
						ghost->AddHP(DAME2);
					}

					if (ghost->GetHP() <= 0)
					{
						ghost->SetHP(0);
						ghost->SetState(GHOST_STATE_DIE);
					}

					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = ghost->x;
						this->y = ghost->y;
					}
					else if (state != WEAPONS_BOOMERANG && state != WEAPONS_HOLY_WATER_SHATTERED)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Raven*>(e->obj))
				{
					Raven* raven = dynamic_cast<Raven*> (e->obj);
					raven->AddHP(DAME2);
					if (raven->GetHP() <= 0)
					{
						raven->SetHP(0);
						raven->SetState(RAVEN_STATE_DIE);
					}
						
					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = raven->x;
						this->y = raven->y;
					}
					else if (state != WEAPONS_BOOMERANG && state != WEAPONS_HOLY_WATER_SHATTERED)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Zombie*>(e->obj))
				{
					Zombie* zombie = dynamic_cast<Zombie*> (e->obj);
					zombie->AddHP(DAME2);
					if (zombie->GetHP() <= 0)
					{
						zombie->SetHP(0);
						zombie->SetState(ZOMBIE_STATE_DIE);
					}

					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = zombie->x;
						this->y = zombie->y;
					}
					else if (state != WEAPONS_BOOMERANG && state != WEAPONS_HOLY_WATER_SHATTERED)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Skeleton*>(e->obj))
				{
					Skeleton* skeleton = dynamic_cast<Skeleton*> (e->obj);
					skeleton->AddHP(DAME2);
					if (skeleton->GetHP() <= 0)
					{
						skeleton->SetHP(0);
						skeleton->SetState(SKELETON_STATE_DIE);
					}

					if (state == WEAPONS_HOLY_WATER)
					{
						SetState(WEAPONS_HOLY_WATER_SHATTERED);
						this->x = skeleton->x;
						this->y = skeleton->y;
					}
					else if (state != WEAPONS_BOOMERANG && state != WEAPONS_HOLY_WATER_SHATTERED)
					{
						this->isDone = true;
						this->isEnable = false;
					}
				}
				else if (dynamic_cast<Ground*>(e->obj)||dynamic_cast<MovingPlatform*>(e->obj))
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
						this->DameBoomerang1 = false;
						this->DameBoomerang2 = false;
					}
				}
			}
		}
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void SubWeapon::Render(int currentID)
{
	if (isDone == false && CheckOutCam(X) == false && state != WEAPONS_STOP_WATCH)
	{
		if (state == -1) return;
		if (state == WEAPONS_HOLY_WATER_SHATTERED)
		{
			animation_set->at((WEAPONS_HOLY_WATER_SHATTERED-1))->Render(nx, x, y);
		}
		else
		{
			animation_set->at(state)->Render(nx, x, y);
		}
		//RenderBoundingBox();
	}
}

void SubWeapon::SetWeaponPosition(float simonX, float simonY, bool isStand)
{
	if (nx > 0)
	{
		simonX += 25;
	}
	else
		simonX -= 15;
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
		vx = nx*WEAPONS_DAGGER_SPEED;
		vy = 0;
		break;
	case WEAPONS_AXE:
		vx = nx*WEAPONS_AXE_SPEED_X;
		vy = -WEAPONS_AXE_SPEED_Y;
		break;
	case WEAPONS_BOOMERANG:
		vx = nx*WEAPONS_BOOMERANG_SPEED;
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
		right = left + DAGGER_BBOX_WIDTH;
		bottom = top + DAGGER_BBOX_HEIGHT;
		break;
	case WEAPONS_AXE:
		right = left + WEAPONS_AXE_BBOX_WIDTH;
		bottom = top + WEAPONS_AXE_BBOX_HEIGHT;
		break;
	case WEAPONS_BOOMERANG:
		right = left + WEAPONS_BOOMERANG_BBOX_WIDTH;
		bottom = top + WEAPONS_BOOMERANG_BBOX_HEIGHT;
		break;
	case WEAPONS_HOLY_WATER:
		right = left + WEAPONS_HOLY_WATER_BBOX_WIDTH;
		bottom = top + WEAPONS_HOLY_WATER_BBOX_HEIGHT;
		break;
	case WEAPONS_STOP_WATCH:
		right = left;
		bottom = top;
		break;
	case WEAPONS_HOLY_WATER_SHATTERED:
		right = left + WEAPONS_HOLY_WATER_BBOX_WIDTH;
		bottom = top + WEAPONS_HOLY_WATER_BBOX_HEIGHT;
		break;
	default:
		right = left;
		bottom = top;
		break;
	}
}