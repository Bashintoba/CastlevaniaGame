#include "Gate.h"

Gate::Gate()
{

}

Gate::~Gate() {}

void Gate::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	
}

void Gate::Render()
{
	RenderBoundingBox();
	//animation_set->at(0)->Render(-1, x, y);
}

void Gate::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + GATE_BBOX_WIDTH;
	bottom = top + GATE_BBOX_HEIGHT;
}