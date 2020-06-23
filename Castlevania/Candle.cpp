#include "Candle.h"

Candle::Candle() : CGameObject()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(CANDLE_ANIMATION_SET);
	SetAnimationSet(ani_set);
	isDone = false;
	isEnable = false;
}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (this->isDone == false)
	{
		if (state == CANDLE_DESTROYED && animation_set->at(CANDLE_DESTROYED)->IsOver(TIME_DELAY) == true)
			this->isDone = true;
	}
}

void Candle::Render()
{
	if (this->isDone == false)
	{
		animation_set->at(state)->Render(-1,x, y);	
	}
	else
		return;
	//RenderBoundingBox();
}

void Candle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BIG_CANDLE:
		break;
	case SMALL_CANDLE:
		break;
	case CANDLE_DESTROYED:
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	default:
		break;
	}
}

void Candle::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (state != CANDLE_DESTROYED)
	{
		left = x;
		top = y;

		switch (state)
		{
		case BIG_CANDLE:
			right = left + CANDLE_BBOX_WIDTH;
			bottom = top + CANDLE_BBOX_HEIGHT;
			break;
		case SMALL_CANDLE:
			right = left + SMALL_CANDLE_BBOX_WIDTH;
			bottom = top + SMALL_CANDLE_BBOX_HEIGHT;
			break;
		default:
			right = left;
			bottom = top;
			break;
		}
	}
}