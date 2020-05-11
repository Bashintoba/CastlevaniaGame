#include "Ground.h"

Ground::Ground()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(GROUND_ANIMATION_SET);
	SetAnimationSet(ani_set);
}


void Ground::Render()
{
	animation_set->at(ani)->Render(-1,x, y);
}

void Ground::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

Ground::~Ground()
{
}

