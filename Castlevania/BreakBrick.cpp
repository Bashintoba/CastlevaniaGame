#include "BreakBrick.h"

BreakBrick::BreakBrick()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(GROUND_ANIMATION_SET);
	SetAnimationSet(ani_set);
	isDone = false;
	isEnable = false;
}

void BreakBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{

}

void BreakBrick::Render()
{
	if (this->isDone == false)
	{
		animation_set->at(ani)->Render(-1, x, y);
	}
	else
		return;
	//RenderBoundingBox();
}

void BreakBrick::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (this->isDone == false)
	{
		left = x;
		top = y;
		right = left + BRICK_BBOX_WIDTH;
		bottom = top + BRICK_BBOX_HEIGHT;
	}
	else
		return;
}
