﻿#include "Whip.h"
#include "Candle.h"
#include "BreakBrick.h"
#include "Knight.h"
#include "Darkenbat.h"
#include "Monkey.h"
#include "Ghost.h"
#include "Raven.h"
#include "Zombie.h"
#include "Skeleton.h"
#include "Boss.h"

Whip::Whip() : CGameObject()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(WHIP_ANIMATION_SET);
	SetAnimationSet(ani_set);
	SetState(NORMAL_WHIP);
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<Candle*>(obj))
		{
			Candle * e = dynamic_cast<Candle*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) // va chạm giữa roi và nến
			{
				e->AddHP(DAME2);
				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(CANDLE_DESTROYED);
					sparkX.push_back(left);
					sparkY.push_back(top);
				}				
			}
		}
		else if (dynamic_cast<BreakBrick*>(obj))
		{
			BreakBrick * e = dynamic_cast<BreakBrick*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) 
			{
				e->AddHP(DAME2);
				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->isDone = true;
					sparkX.push_back(left);
					sparkY.push_back(top);
				}
			}
		}
		else if (dynamic_cast<Knight*>(obj))
		{
			Knight* e = dynamic_cast<Knight*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) 
			{
				if (Dame1turn == false)
				{
					e->AddHP(DAME2);
					Dame1turn = true;
				}

				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(KNIGHT_STATE_DIE);
				}

				sparkX.push_back(left);
				sparkY.push_back(top);
			}
		}
		else if (dynamic_cast<Darkenbat*>(obj))
		{
			Darkenbat* e = dynamic_cast<Darkenbat*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->AddHP(DAME2);
				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(DARKBAT_STATE_DIE);
					sparkX.push_back(left);
					sparkY.push_back(top);
				}
			}
		}
		else if (dynamic_cast<Monkey*>(obj))
		{
			Monkey* e = dynamic_cast<Monkey*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);
			top = top - 32;
			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->AddHP(DAME2);
				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(MONKEY_STATE_DIE);
					sparkX.push_back(left);
					sparkY.push_back(top);
				}
			}
		}
		else if (dynamic_cast<Ghost*>(obj))
		{
			Ghost* e = dynamic_cast<Ghost*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) 
			{
				if (Dame1turn == false)
				{
					e->AddHP(DAME2);
					Dame1turn = true;
				}

				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(GHOST_STATE_DIE);
				}
				sparkX.push_back(left);
				sparkY.push_back(top);
			}
		}
		else if (dynamic_cast<Raven*>(obj))
		{
			Raven* e = dynamic_cast<Raven*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) 
			{
				e->AddHP(DAME2);
				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(RAVEN_STATE_DIE);
					sparkX.push_back(left);
					sparkY.push_back(top);
				}
			}
		}
		else if (dynamic_cast<Zombie*>(obj))
		{
			Zombie* e = dynamic_cast<Zombie*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true) 
			{
				e->AddHP(DAME2);
				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(ZOMBIE_STATE_DIE);
					sparkX.push_back(left);
					sparkY.push_back(top);
				}
			}
		}
		else if (dynamic_cast<Skeleton*>(obj))
		{
			Skeleton* e = dynamic_cast<Skeleton*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				e->AddHP(DAME2);
				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(SKELETON_STATE_DIE);
					sparkX.push_back(left);
					sparkY.push_back(top);
				}
			}
		}
		else if (dynamic_cast<Boss*>(obj))
		{
			Boss* e = dynamic_cast<Boss*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				if (Dame1turn == false)
				{
					e->AddHP(DAME2);
					Dame1turn = true;
				}

				if (e->GetHP() <= 0)
				{
					e->SetHP(0);
					e->SetState(BOSS_STATE_DIE);
				}

				sparkX.push_back(left);
				sparkY.push_back(top);
			}
		}
	}
}

void Whip::RenderSpark()
{
	if (sparkX.size() > 0)
	{
		if (startTimeRenderSpark == 0)
			startTimeRenderSpark = GetTickCount();
		else if (GetTickCount() - startTimeRenderSpark > TIME_FRAME)
		{
			startTimeRenderSpark = 0;
			sparkX.clear();
			sparkY.clear();
		}

		for (int i = 0; i < sparkX.size(); i++)
		{
			spark->Render(-1, sparkX[i], sparkY[i]);
		}
	}
}

void Whip::Render( int nx,int currentID)
{
	RenderSpark();

	if (currentID >= 0) {
		animation_set->at(state)->RenderByID(currentID, nx, x, y);
	}
	//RenderBoundingBox();
	
}

void Whip::SetWhipPosition(int nx,float simonX, float simonY, bool isStand)
{
	this->nx = nx;
	simonX -= 90;
	if (isStand == false)
		simonY += 15;

	SetPosition(simonX, simonY);
}

bool Whip::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,whip_top,whip_right,whip_bottom;
	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);
	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void Whip::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	top = y + WHIP_BBOX_HEIGHT;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		if (state != LONG_WHIP)
			left = x + 50;
		else left = x + 20;
	}
	else if (nx > 0)
	{
		if (state != LONG_WHIP)
			left = (240 - 50) - WHIP_BBOX_WIDTH + x;
		else left = (240 - 20) - LONG_CHAIN_BBOX_WIDTH + x;
	}

	if (state != LONG_WHIP)
		right = left + WHIP_BBOX_WIDTH;
	else  right = left + LONG_CHAIN_BBOX_WIDTH;

}

void Whip::PowerUp()
{
	if (state == NORMAL_WHIP) SetState(SHORT_WHIP);
	else if (state == SHORT_WHIP) SetState(LONG_WHIP);
}