#include "Items.h"
#include "Knight.h"
#include "Darkenbat.h"

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
		if (coObject->at(i) != dynamic_cast<Knight*>(coObject->at(i)) && coObject->at(i) != dynamic_cast<Darkenbat*>(coObject->at(i)))
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
				right = left + 16;
				bottom = top + 16;
				break;
			case LARGE_HEART:
				right = left + 24;
				bottom = top + 20;
				break;
			case MONEY_BAG_RED:
			case MONEY_BAG_BLUE:
			case MONEY_BAG_WHITE:
				right = left + 30;
				bottom = top + 30;
				break;
			case PORK_CHOP:
				right = left + 32;
				bottom = top + 26;
				break;
			case CHAIN:
				right = left + 32;
				bottom = top + 32;
				break;
			case STOP_WATCH:
				right = left + 30;
				bottom = top + 32;
				break;
			case ITEM_DAGGER:
				right = left + 32;
				bottom = top + 18;
				break;			
			case ITEM_AXE:
				right = left + 30;
				bottom = top + 28;
				break;
			case ITEM_BOOMERANG:
				right = left + 30;
				bottom = top + 28;
				break;
			case ITEM_HOLY_WATER:
				right = left + 32;
				bottom = top + 28;
				break;					
			case CROSS:
				right = left + 32;
				bottom = top + 32;
				break;
			case INVISIBILITY_POTION:
				right = left + 29;
				bottom = top + 36;
				break;						
			case DOUBLE_SHOT:
			case TRIPLE_SHOT:
				right = left + 28;
				bottom = top + 28;
				break;													
			case CROWN:
				right = left + 32;
				bottom = top + 32;
				break;
			case MAGIC_CRYSTAL:
				right = left + 26;
				bottom = top + 32;
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