#include "Items.h"
#include "Candle.h"
#include "Knight.h"
#include "Darkenbat.h"
#include "Monkey.h"
#include "Ghost.h"
#include "Raven.h"
#include "Zombie.h"
#include "Skeleton.h"
#include "Bone.h"

Items::Items() : CGameObject()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set_items = animation_sets->Get(ITEMS_ANIMATION_SET);
	SetAnimationSet(ani_set_items);
	timeAppear = -1;
}

void Items::Render()
{
	if (isDone == false)
	{
		animation_set->at(state)->Render(-1, x, y);
		RenderBoundingBox();
	}
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (timeAppear == -1)
		timeAppear = GetTickCount();
	else
	{
		DWORD now = GetTickCount();

		if (now - timeAppear > ITEM_TIME_DESTROYED)
		{
			isEnable = false;
			isDone = true;
			return;
		}
	}

	CGameObject::Update(dt);
	// Check collision between item and ground (falling on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT> ListsColl;
	coEvents.clear();
	ListsColl.clear();

	for (UINT i = 0; i < coObject->size(); i++)
	{
		if (coObject->at(i) != dynamic_cast<Candle*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Knight*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Darkenbat*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Ghost*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Monkey*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Skeleton*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Raven*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Zombie*>(coObject->at(i)))
		{
			ListsColl.push_back(coObject->at(i));
		}
	}

	CalcPotentialCollisions(&ListsColl, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		y += min_ty * dy + ny * 0.4f;
		if (ny != 0)
		{
			vx = 0;
			vy = 0;
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Items::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (isDone == false)
	{
		left = x;
		top = y;

		switch (state)
		{
			case SMALL_HEART:
				right = left + SMALL_HEART_BBOX_WIDTH;
				bottom = top + SMALL_HEART_BBOX_HEIGHT;
				break;
			case LARGE_HEART:
				right = left + LARGE_HEART_BBOX_WIDTH;
				bottom = top + LARGE_HEART_BBOX_HEIGHT;
				break;
			case MONEY_BAG_RED:
			case MONEY_BAG_BLUE:
			case MONEY_BAG_WHITE:
				right = left + MONEY_BAG_BBOX_WIDTH;
				bottom = top + MONEY_BAG_BBOX_HEIGHT;
				break;
			case PORK_CHOP:
				right = left + PORK_CHOP_BBOX_WIDTH;
				bottom = top + PORK_CHOP_BBOX_HEIGHT;
				break;
			case CHAIN:
				right = left + CHAIN_BBOX_WIDTH;
				bottom = top + CHAIN_BBOX_HEIGHT;
				break;
			case STOP_WATCH:
				right = left + STOP_WATCH_BBOX_WIDTH;
				bottom = top + STOP_WATCH_BBOX_HEIGHT;
				break;
			case ITEM_DAGGER:
				right = left + ITEM_DAGGER_BBOX_WIDTH;
				bottom = top + ITEM_DAGGER_BBOX_HEIGHT;
				break;			
			case ITEM_AXE:
				right = left + ITEM_AXE_BBOX_WIDTH;
				bottom = top + ITEM_AXE_BBOX_HEIGHT;
				break;
			case ITEM_BOOMERANG:
				right = left + ITEM_BOOMERANG_BBOX_WIDTH;
				bottom = top + ITEM_BOOMERANG_BBOX_HEIGHT;
				break;
			case ITEM_HOLY_WATER:
				right = left + ITEM_HOLY_WATER_BBOX_WIDTH;
				bottom = top + ITEM_HOLY_WATER_BBOX_HEIGHT;
				break;					
			case CROSS:
				right = left + CROSS_BBOX_WIDTH;
				bottom = top + CROSS_BBOX_HEIGHT;
				break;
			case INVISIBILITY_POTION:
				right = left + INVISIBILITY_POTION_BBOX_WIDTH;
				bottom = top + INVISIBILITY_POTION_BBOX_HEIGHT;
				break;						
			case DOUBLE_SHOT:
			case TRIPLE_SHOT:
				right = left + EXTRA_SHOT_BBOX_WIDTH;
				bottom = top + EXTRA_SHOT_BBOX_HEIGHT;
				break;													
			case CROWN:
				right = left + CROWN_BBOX_WIDTH;
				bottom = top + CROWN_BBOX_HEIGHT;
				break;
			case MAGIC_CRYSTAL:
				right = left + MAGIC_CRYSTAL_BBOX_WIDTH;
				bottom = top + MAGIC_CRYSTAL_BBOX_HEIGHT;
				break;
		default:
			right = left;
			bottom = top;
			break;
		}
	}
}

void Items::SetState(int state)
{
	CGameObject::SetState(state);

	vx = 0;
	vy = ITEM_FALLING_SPEED_Y;
}