#include "MovingPlatform.h"

MovingPlatform::MovingPlatform()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set_items = animation_sets->Get(ITEMS_ANIMATION_SET);
	SetAnimationSet(ani_set_items);
	nx = 1;
	vx = PLAT_MOVING_SPEED;
}

void MovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj))
			{
				if(nx != 0 && ny == 0)
				{
					this->nx *= -1;
					vx *= -1;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void MovingPlatform::Render()
{
	animation_set->at(ani)->Render(-1, x, y);
	//RenderBoundingBox();
}

void MovingPlatform::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + PLAT_BBOX_WIDTH;
	bottom = top + PLAT_BBOX_HEIGHT;
}
