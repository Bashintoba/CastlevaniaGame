#include "Stair.h"

Stair::Stair()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(STAIR_ANIMATION_SET);
	SetAnimationSet(ani_set);
}

Stair::~Stair()
{
}

void Stair::Render()
{
	animation_set->at(state)->Render(-1, x, y);
}

void Stair::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + STAIR_BBOX_WIDTH;
	b = y + STAIR_BBOX_HEIGHT;
}

void Stair::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MAP2Stair:
		break;
	case MAP21Stair:
		break;
	default:
		break;
	}
}
