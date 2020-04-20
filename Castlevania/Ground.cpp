#include "Ground.h"

Ground::Ground()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(GROUND_ANIMATION_SET);
	SetAnimationSet(ani_set);
}


void Ground::Render()
{
	animation_set->at(state)->Render(-1,x, y);
}

void Ground::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void Ground::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MAP1BRICK:
		break;
	case MAP2BRICK:
		break;
	default:
		break;
	}
}

Ground::~Ground()
{
}

